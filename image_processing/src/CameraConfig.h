#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

#include <math.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>


/*********************************
** set to 0 if running on edison *
**********************************/
static const int DEBUG = 1;
/********************************/


/********************************************************
 * NEED TO DIVIDE THINGS BY RESCALE FACTOR IF RESCALE ***
 * *****************************************************/

// translation vector of camera w.r.t to robot center
static const double CAM_ROBOT_X = 4;
static const double CAM_ROBOT_Y = 0;

// horizontal rotation of camera w.r.t. to front direction
// counter-clockwise is positive
static const double CAM_ANGLE_HOR = 0;
// vertical rotation of camera w.r.t. horizontal plane
// downwards is positive
static const double CAM_ANGLE_VERT = (23.0977 / 180 * M_PI); // in case we put it at an angle -- in radians
// height of camera
// in inches
static const double CAM_HEIGHT = 11; // inches

static const double FRAME_RESIZE_SCALE = 0.5;

static const int FRAME_SIZE_X = (640*FRAME_RESIZE_SCALE);
static const int FRAME_SIZE_Y = (480*FRAME_RESIZE_SCALE);

// camera matrix elements
static const double CAM_MAT_fx = 685.3;
static const double CAM_MAT_cx = 295.5;
static const double CAM_MAT_fy = 683.9;
static const double CAM_MAT_cy = 236.4;
/*
static Eigen::Matrix3d ROT_MAT;
static Eigen::Matrix3d CAM_MAT;
static Eigen::Matrix3d CAM_MAT_INV;
*/

static Eigen::Matrix3d ROT_MAT = ((Eigen::Matrix3d() << 1,        0,                    0,
                                                        0, cos(CAM_ANGLE_VERT), -sin(CAM_ANGLE_VERT),
                                                        0, sin(CAM_ANGLE_VERT), cos(CAM_ANGLE_VERT)).finished());

static Eigen::Matrix3d CAM_MAT = (((Eigen::Matrix3d() << (FRAME_RESIZE_SCALE*CAM_MAT_fx), 0, (FRAME_RESIZE_SCALE*CAM_MAT_cx),
                                                    0, (FRAME_RESIZE_SCALE*CAM_MAT_fy), (FRAME_RESIZE_SCALE*CAM_MAT_cy),
                                                    0, 0, 1).finished())*ROT_MAT);

static Eigen::Matrix3d CAM_MAT_INV = (CAM_MAT.inverse());

/*
// with rotation
static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << (FRAME_RESIZE_SCALE*685), (FRAME_RESIZE_SCALE*115.7287), (FRAME_RESIZE_SCALE*271.3519), 0, (FRAME_RESIZE_SCALE*720.8317), (FRAME_RESIZE_SCALE*(-50.8599)), 0, (FRAME_RESIZE_SCALE*0.3923), (FRAME_RESIZE_SCALE*0.9198)).finished());
static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << (0.00145/FRAME_RESIZE_SCALE), 0, ((-0.43065)/FRAME_RESIZE_SCALE), 0, (0.0013346/FRAME_RESIZE_SCALE), (0.074465/FRAME_RESIZE_SCALE), 0, ((-0.0005743)/FRAME_RESIZE_SCALE), (1.055390/FRAME_RESIZE_SCALE)).finished());
// without rotation
//static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1).finished());
//static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.00146412,-0.34553440,0,0,1).finished());
*/

// update rate for camera thread
static const int UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS = 10000;

#endif
