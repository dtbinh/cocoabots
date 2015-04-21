import cgi
import cv2
import Image
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
import StringIO
import time
from SocketServer import ThreadingMixIn
import sys
import threading
import base64
import creepybot

capture=None

key = ""
mycreepybot = 0
class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""

class CamHandler(BaseHTTPRequestHandler):
	global mycreepybot
        def getVars(self):
                ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))
                if ctype == 'multipart/form-data':
                        postvars = cgi.parse_multipart(self.rfile, pdict)
                elif ctype == 'application/x-www-form-urlencoded':
                        length = int(self.headers.getheader('content-length'))
                        postvars = cgi.parse_qs(self.rfile.read(length), keep_blank_values=1)
                else:
                        postvars = {}
                return postvars
        def do_POST(self):
                if self.headers.getheader('Authorization') == 'Basic '+key:
                    vars=self.getVars();
                    mykeys=["distance","angle","raiseArm","sweep","grab","servoAngle1","servoAngle2","wallFollow"]
                    for k in mykeys:
                        vars.setdefault(k, [0])    
                    distance=vars['distance']
                    angle=vars['angle']
                    raiseArm=vars['raiseArm']
                    sweep=vars['sweep']
                    grab=vars['grab']
                    servoAngle1=vars['servoAngle1']
                    servoAngle2=vars['servoAngle2']
                    wallFollow=vars['wallFollow']
                    mycreepybot.sendInstructions(float(distance[0]),
                                                 float(angle[0]),
                                                 int(raiseArm[0]),
                                                 int(sweep[0]),
                                                 int(grab[0]),
                                                 int(servoAngle1[0]),
                                                 int(servoAngle2[0]),
                                                 int(wallFollow[0]))
                return
        def do_AUTHHEAD(self):
                self.send_response(401)
                self.send_header('WWW-Authenticate', 'Basic realm=\"Test\"')
                self.send_header('Content-type', 'text/html')
                self.end_headers()
        def do_GET(self):
                global key
                ''' Present frontpage with user authentication. '''
                if self.headers.getheader('Authorization') == None:
                        self.do_AUTHHEAD()
                        self.wfile.write('no auth header received')
                        
                elif self.headers.getheader('Authorization') == 'Basic '+key:
                        self.do_GET_good()
                        
                else:
                        print(self.headers.getheader('Authorization'))
                        print('Basic '+key)
                        print(self.headers.getheader('Authorization') == 'Basic '+key)
                        self.do_AUTHHEAD()
                        self.wfile.write(self.headers.getheader('Authorization'))
                        self.wfile.write('not authenticated')
                        
        def do_GET_good(self):
		if self.path.endswith('.mjpg'):
			self.send_response(200)
			self.send_header('Content-type','multipart/x-mixed-replace; boundary=--jpgboundary')
			self.end_headers()
			while True:
				try:
					rc,img = capture.read()
					if not rc:
						continue
					imgRGB=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
					jpg = Image.fromarray(imgRGB)
					tmpFile = StringIO.StringIO()
					jpg.save(tmpFile,'JPEG')
					self.wfile.write("--jpgboundary")
					self.send_header('Content-type','image/jpeg')
					self.send_header('Content-length',str(tmpFile.len))
					self.end_headers()
					jpg.save(self.wfile,'JPEG')
					time.sleep(0.05)
				except KeyboardInterrupt:
					break
			return
		if self.path.endswith('.html') or self.path == '/':
			self.send_response(200)
			self.send_header('Content-type','text/html')
			self.end_headers()
	

			self.wfile.write('<html><head></head><body>')
                        self.wfile.write('<a href="/cam.mjpg">')
			self.wfile.write('<img src="/cam.mjpg" />')
                        self.wfile.write('</a>')
                        self.wfile.write('Please click on the image and return, if it is not showing up.')
                        self.wfile.write('<form action="move.html" method="POST"target="my_iframe">\
                        Angle:<br>\
                        <input type="number" name="angle">\
                        <br>\
                        Distance:<br>\
                        <input type="number" name="distance">\
                        <input type="submit" value="Submit">\
                        </form>')
			self.wfile.write('<iframe name="my_iframe" src="about:blank width="1" height="1"></iframe>')
                        self.wfile.write('</body></html>')
			return
 
def main():
        if len(sys.argv)<2:
                print "usage SimpleAuthServer.py [username:password]"
                sys.exit()
        global key
        key = base64.b64encode(sys.argv[1])
        print(key)
	global capture
	global mycreepybot
	capture = cv2.VideoCapture(0)
        mycreepybot=creepybot.creepybot()
	capture.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, 320); 
	capture.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 240);
	capture.set(cv2.cv.CV_CAP_PROP_SATURATION,0.2);
	global img
	try:
		server = ThreadedHTTPServer(('',80),CamHandler)
		print "server started"
		server.serve_forever()
	except KeyboardInterrupt:
		capture.release()
		server.socket.close()
 
if __name__ == '__main__':
	main()
 