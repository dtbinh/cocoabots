#include <cstdio>
#include <cstring>
#include <cmath>
#include "sensors/ultrasonic.h"
#include "actuators/pwmutils.h"
#include "actuators/actuator.h"
#include "sensorsmodule.h"
#include "sensors/encoderquadrature.h"
#include "actuators/motorscontrol.h"
#include "mraa.hpp"
#include <signal.h>
#include <sys/time.h>
#include  <thread>
#include <stdlib.h>     /* atoi */
#include "imageProcessing/ImageProcessor.h"
//#include "localization/particlefilter.h"
//#include "mapping/map.h"



actuator * actPointer;
int RUNNING;
void stopMotors(int signo)
{
    if (signo == SIGINT) {
        RUNNING =0;
        double a =0;
        actPointer->leftWheelPower = &a;
        actPointer->rightWheelPower =&a;
        actPointer->setPowerLeftWheel(0);
        actPointer->setPowerRightWheel(0);
        RUNNING =0;
    }
}

int main(int argc, char** argv){

    //if no parameter was passed
    if (argc==1){
        printf("Missing Arguments\n The usage is: test_run arguments");
    }
    else{
        if(strcmp(argv[1],"ultrasonic")==0){
            printf("Running ultrasonic test\n");
            ultrasonic testUltrasonic(11,12);
            while(1)
            {
                usleep(200000.0);
                printf("%f\n", testUltrasonic.getDistance(0));
            }

        }
        else if (strcmp(argv[1],"ultrasonicpwm")==0){
            ultrasonic testUltrasonic(2,3);
            pwmUtils pwm;
            //We have a servo in 0
            while(1)
            {
                pwm.setServoPosition(0,testUltrasonic.getDistance(0)/40.0);


            }
        }

        else if (strcmp(argv[1],"pwmservo")==0){
            pwmUtils pwm;
            //We have a servo in 0
            while(1)
            {
                usleep(200000.0);
                pwm.setServoPosition(0,0.3);
                usleep(200000.0);
                pwm.setServoPosition(0,0.5);
            }
        }
        else if (strcmp(argv[1],"sensormoduleultrasonic")==0){
            sensorsModule mysensors;
            while(1)
            {
                printf("%f\n", mysensors.frontUltrasonicData);
                usleep(200000.0);

            }
        }

        else if (strcmp(argv[1],"testtime")==0){
            sensorsModule mysensors;
            while(1)
            {
                printf("%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"straightline")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=1;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.realSpeed);
                printf("angularspeed =%f\n", control.realAngularSpeed);
                printf("angle =%f\n", mysensors.gyroscopeAngle);
                printf("rightRotation =%f\n", mysensors.rightEncoderMovement );
                printf("leftRotation =%f\n", mysensors.leftEncoderMovement );
                printf("control Position =%f\n", control.previousPosition );
                printf("control Angle =%f\n", control.previousAngle);
                printf("control time =%f\n", control.previousTime);


                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"straightline2")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle =90 ;
            control.desiredPosition=1;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.realSpeed);
                printf("angularspeed =%f\n", control.realAngularSpeed);
                printf("angle =%f\n", mysensors.gyroscopeAngle);
                printf("rightRotation =%f\n", mysensors.rightEncoderMovement );
                printf("leftRotation =%f\n", mysensors.leftEncoderMovement );
                printf("control Position =%f\n", control.previousPosition );
                printf("control Angle =%f\n", control.previousAngle);
                printf("control time =%f\n", control.previousTime);


                usleep(50000.0);

            }
        }
        else if (strcmp(argv[1],"motorcrazy")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(0.3);
                myactuator.setPowerRightWheel(0.3);


                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"motorcrazy2")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(0.3);
                myactuator.setPowerRightWheel(-0.3);


                usleep(200000.0);
                myactuator.setPowerLeftWheel(1);
                myactuator.setPowerRightWheel(1);
                usleep(200000.0);
            }
        }
        else if (strcmp(argv[1],"motorstop")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(0);
                myactuator.setPowerRightWheel(0);


                usleep(200000.0);

            }
        }

        else if (strcmp(argv[1],"encoder")==0){
            encoder myenc(RIGHT_ENCODER_ENC);
            while (1){
                usleep(200000.0);
                printf("right encoder counts =%lld\n", myenc.edgeCount);


            }
        }
        else if (strcmp(argv[1],"stayStraight")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);


                usleep(10000.0);

            }
        }
        else if (strcmp(argv[1],"stayStraightDebug")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            RUNNING=1;
            while(RUNNING)
            {
                printf("leftWheelSpeed =%f and power =%f\n", control.normalizedLeftWheelSpeed ,control.leftMotorPower );
                printf("rightWheelSpeed =%f and power =%f\n", control.normalizedRightWheelSpeed ,control.rightMotorPower );

                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);


                usleep(100000.0);

            }
        }
        else if (strcmp(argv[1],"findProportionalGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double proportionalGain =0.001;
            control.angSpeedGain=0;
            control.angErrorGain = proportionalGain;
            control.fwdErrorGain =0;
            control.fwdSpeedGain =0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.angErrorGain = proportionalGain;
                printf("proportionalGain =%f\n", proportionalGain);
                control.desiredAngle=90;
                usleep(3000000.0);
                control.desiredAngle=0;
                proportionalGain +=0.001;
                printf("proportionalGain =%f\n", proportionalGain);
                control.angErrorGain = proportionalGain;
                usleep(3000000.0);
                proportionalGain +=0.001;


            }
        }
        else if (strcmp(argv[1],"findDerivativeGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double derivativeGain =-0.0001;
            control.angSpeedGain=derivativeGain;
            control.fwdErrorGain =0;
            control.fwdSpeedGain =0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.angSpeedGain = derivativeGain;
                printf("derivativeGain =%f\n", derivativeGain);
                control.desiredAngle=90;
                usleep(3000000.0);
                control.desiredAngle=0;
                derivativeGain +=-0.0001;
                printf("derivativeGain =%f\n", derivativeGain);
                control.angSpeedGain = derivativeGain;
                usleep(3000000.0);
                derivativeGain +=-0.0001;


            }
        }
        else if (strcmp(argv[1],"fwdGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double fwdGain =0.01;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.fwdErrorGain = fwdGain;
                printf("fwdGain =%f\n", fwdGain);
                control.desiredPosition=2.5;
                usleep(3000000.0);
                control.desiredPosition=0;
                fwdGain +=0.01;
                printf("fwdGain =%f\n", fwdGain);
                control.fwdErrorGain = fwdGain;
                usleep(3000000.0);
                fwdGain +=0.01;


            }
        }
        else if (strcmp(argv[1],"square")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            double desiredPos;
            desiredPos =0;
            double desiredAngle =90;
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {

                control.desiredPosition+=24;
                control.desiredAngle+=90;
                usleep(3000000.0);


            }
        }
        else if (strcmp(argv[1],"ultrasonicPing")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                if(control.previousPosition-control.desiredPosition<5){
                    control.desiredPosition+=12;
                }
                if (mysensors.frontUltrasonicData < 30){
                    control.desiredAngle+=90;
                }
                usleep(2000000.0);


            }
        }
        else if (strcmp(argv[1],"ultrasonicPingState")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            int state=1;
            while(RUNNING)
            {
                if (state){
                    if(control.previousPosition-control.desiredPosition<5&&-control.previousPosition+control.desiredPosition<05){
                        control.desiredPosition+=12;
                    }
                    if (mysensors.frontUltrasonicData < 30){
                        control.desiredAngle+=90;
                        control.desiredPosition=control.previousPosition;
                        state=0;
                    }
                }
                else{
                    double angleError = control.getAngleError(control.previousAngle,control.desiredAngle);

                    if ( (angleError <7) && (-angleError <7)) {
                        state=1;
                    }
                }
                usleep(20000.0);


            }
        }
        else if (strcmp(argv[1],"moveStraight")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            double desiredPosition;
            while(RUNNING)
            {

                scanf("%lf", &desiredPosition);
                printf("my position =%lf\n", control.getNewPosition());
                control.desiredPosition+=desiredPosition;
                usleep(20000.0);


            }
        }
        else if (strcmp(argv[1],"remoteControl")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            double desiredPosition;
            double desiredAngle;
            while(RUNNING)
            {

                printf("define the distance and angle to move\n My position=%lf \n", control.getNewPosition());
                scanf("%lf %lf", &desiredPosition, &desiredAngle);
                printf("my position =%lf\n", control.getNewPosition());
                printf("Distance left wheel measured by the encoders =%lf\n", mysensors.leftEncoderMovement);
                printf("Distance right wheel  measured by the encoders =%lf\n", mysensors.rightEncoderMovement);
                printf("Angle measured by the encoders =%lf\n", control.getNewAngleFromEncoders());
                printf("Angle measured by the gyroscope =%lf\n", control.getNewAngleFromGyroscope());
                control.desiredPosition+=desiredPosition;
                control.desiredAngle+=desiredAngle;
                usleep(20000.0);


            }
        }
        else if (strcmp(argv[1],"encoderQuadrature")==0){
            encoderQuadrature leftEncoder(LEFT_ENCODER_ENC_A,LEFT_ENCODER_ENC_B,1);
            encoderQuadrature rightEncoder(RIGHT_ENCODER_ENC_A,RIGHT_ENCODER_ENC_B,0);
            RUNNING =1;
            while(RUNNING)
            {

                printf("left encoder distance =%lf, right encoder distance = %lf\n", leftEncoder.getData(), rightEncoder.getData());
                usleep(200000.0);


            }
        }

        else if (strcmp(argv[1],"remoteControlServo")==0){
            //signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            double hook = HOOK_START;
            double arm = ARM_START;
            double sort = SORT_START;
            myactuator.armServoAngle=&arm;
            myactuator.hookServoAngle=&hook;
            myactuator.sortServoAngle=&sort;
            RUNNING =1;

            while(RUNNING)
            {
                printf("define angle for the servos\n Hook=%lf, Arm=%lf, Sort=%lf \n", myactuator.getHookServoAngle(), myactuator.getArmServoAngle(), myactuator.getSortServoAngle());
                scanf("%lf %lf %lf", &hook, &arm, &sort);

                usleep(20000.0);


            }
        }

        else if (strcmp(argv[1],"actuatorInitializer")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            servosControl myservos;
            motorsControl control(&mysensors);
            actuator myactuator(control,myservos);
            actPointer= &myactuator;
            control.desiredAngle=0;
            control.desiredPosition=0;
            RUNNING =1;
            double desiredPosition;
            double desiredAngle;
            while(RUNNING)
            {

                printf("define the distance and angle to move\n My position=%lf \n", control.getNewPosition());
                scanf("%lf %lf", &desiredPosition, &desiredAngle);
                printf("my position =%lf\n", control.getNewPosition());
                printf("Distance left wheel measured by the encoders =%lf\n", mysensors.leftEncoderMovement);
                printf("Distance right wheel  measured by the encoders =%lf\n", mysensors.rightEncoderMovement);
                printf("Angle measured by the encoders =%lf\n", control.getNewAngleFromEncoders());
                printf("Angle measured by the gyroscope =%lf\n", control.getNewAngleFromGyroscope());
                control.setNewDesiredRelativePositionInRadialCordinates(desiredPosition,desiredAngle);
                usleep(20000.0);


            }
        }
        else if (strcmp(argv[1],"imageProcessorTest")==0){
            ImageProcessor myImageProcessor;
            RUNNING=1;
            while(RUNNING){
                printf("found cube =%d, cubePosition= %lf, cubeAngle=%lf, cubeColor=%d\n", myImageProcessor.getFoundCube(), myImageProcessor.getNearestCubeDist(), myImageProcessor.getNearestCubeAngle(), myImageProcessor.getNearestCubeColor());
                usleep(2000000);
            }
        }

        else if (strcmp(argv[1],"calibrateServo") == 0){
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            double hook = 100;
            double arm = 7;
            double sort = 45;
            int raise, unhook, sweep;
            myactuator.armServoAngle=&arm;
            myactuator.hookServoAngle=&hook;
            myactuator.sortServoAngle=&sort;
            RUNNING =1;
            while(RUNNING)
            {
                printf("define wait times (us) for starting raise, unhooking, and sweeping");
                scanf("%d %d %d", &raise, &unhook, &sweep);
                hook = 170;
                usleep(raise);
                arm = 150;
                usleep(unhook);
                hook = 100;
                usleep(sweep);
                sweep = 135;
                hook = 100, arm = 7, sweep = 45;


            }
        }
        else if (strcmp(argv[1],"servoControl") == 0){
            servosControl myServos;
            actuator myactuator;
            actPointer= &myactuator;

            int raise, hook,sort, sweep, reset;
            myactuator.armServoAngle=&myServos.armAngle;
            myactuator.hookServoAngle=&myServos.hookAngle;
            myactuator.sortServoAngle=&myServos.sortAngle;
            RUNNING =1;
            while(RUNNING)
            {
                printf("raise, hook, sort, sweep, reset\n");
                scanf(" %d %d %d %d %d ", &raise, &hook, &sort, &sweep, &reset);
                if(raise)
                    myServos.raiseBlock();
                if(hook)
                    myServos.hookBlock();
                else
                    myServos.unHookBlock();
                if(sort)
                    myServos.sortRight();
                else
                    myServos.sortLeft();
                if(sweep)
                    myServos.sweep();
                else
                    myServos.stopSweep();
                if (reset){
                    myServos.stopSweep();
                    myServos.reset();
                }

            }
        }

        else if (strcmp(argv[1],"goToCube")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;

            ImageProcessor myImageProcessor;
            RUNNING =1;
            double desiredPosition;
            double desiredAngle;
            int answer=0;
            while(RUNNING)
            {
                if(!myImageProcessor.getFoundCube()){

                }
                else{
                    desiredAngle=myImageProcessor.getNearestCubeAngle();
                    desiredPosition=myImageProcessor.getNearestCubeDist();
                    printf("I found a cube at %lf in, %lf, degrees.\n Go to cube? 1, 0\n", desiredPosition,desiredAngle);
                    scanf("%d", &answer);
                    if (answer){
                        control.setNewDesiredRelativePositionInRadialCordinates(desiredPosition,desiredAngle);
                    }
                }

                usleep(20000.0);


            }
        }
        else if (strcmp(argv[1],"testIr")==0){
            sensorsModule mysensors;
            RUNNING =1;
            while(RUNNING){
                printf("front Ir =%lf in, right IR = %lf in\n", mysensors.frontShortIRData, mysensors.rightShortIRData);
                usleep(200000.0);
            }

        }

        else if (strcmp(argv[1],"callibrateIr")==0){
            shortIR right(RIGHT_SHORTIR_PIN);
            shortIR front(FRONT_SHORTIR_PIN);
            RUNNING =1;
            while(RUNNING){
                printf("front Ir =%lf in, right IR = %lf in\n",front.timing(), right.timing());
                usleep(200000.0);
            }

        }

        else if (strcmp(argv[1],"remoteControlAll")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            servosControl myServos;
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            myactuator.armServoAngle=&myServos.armAngle;
            myactuator.hookServoAngle=&myServos.hookAngle;
            myactuator.sortServoAngle=&myServos.sortAngle;
            RUNNING =1;
            double desiredPosition, desiredAngle;
            int option;
            while(RUNNING)
            {
                printf("Option Menu: \n 0: Change Position \n 1: Hook \n 2: Raise \n 3: Sort \n 4: Sweep \n 5: Reset \n");
                scanf("%d",&option);
                if (option == 0){
                    printf("define the distance and angle to move\n My position=%lf \n", control.getNewPosition());
                    scanf("%lf %lf", &desiredPosition, &desiredAngle);
                    printf("my position =%lf\n", control.getNewPosition());
                    control.desiredPosition+=desiredPosition;
                    control.desiredAngle+=desiredAngle;
                }

                if (option == 1){
                    myServos.hookBlock();
                }
                else
                    myServos.unHookBlock();
                if (option == 2){
                    myServos.raiseBlock();
                }
                else
                    myServos.unHookBlock();
                if (option == 3){
                    myServos.sortLeft();
                }
                else
                    myServos.sortRight();
                if (option == 4){
                    myServos.sweep();
                }
                else
                    myServos.stopSweep();
                if (option == 5){
                    myServos.stopSweep();
                    myServos.reset();
                }

                usleep(20000.0);

            }
        }

//        else if(strcmp(argv[1],"colorsensor")==0){
//            printf("Running color sensor test\n");
//            jankyColorSensor myColorSensor(COLOR_DETECTOR_PIN);
//            double min = 900;
//            double max = 0;
//            RUNNING =1;
//            while(RUNNING){
//                double data = myColorSensor.rawData();
//                if (data < min && data > 10) {

//                    min = data;
//                    printf("%lf min\n", min);
//                }
//                if (data > max) {

//                    max = data;
//                    printf("%lf max\n", max);
//                }
//                usleep(200000.0);
//            }
//    }
        /*
        else if(strcmp(argv[1],"particleFilter1")==0){
            printf("ParticleFilterTest\n");
            RUNNING =1;
            particleFilter myParticleFilter(50,50);
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html");
                myParticleFilter.updateParticles(10,5);
                myParticleFilter.resample();
                usleep(200000.0);
            }

        }
        else if(strcmp(argv[1],"particleFilter2")==0){
            printf("ParticleFilterTest\n");
            RUNNING =1;
            sensorsModule mySensors;
            motorsControl myMotorControl(&mySensors);
            particleFilter myParticleFilter(50,50,&mySensors);
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html");
                usleep(200000.0);
            }

        }

        else if(strcmp(argv[1],"mapSonar")==0){
            RUNNING =1;
            map myMap(MAP_FILE_NAME);
            double positionX;
            double positionY;
            double angle;
            while(RUNNING){
                printf("Put the desired positionX, desired positionY, and angle(clockwise postive)\n"
                       "to print the sonar readings\n");
                scanf("%lf %lf %lf", &positionX, &positionY, &angle);
                printf("right = %lf\n left = %lf \n front = %lf \n back = %lf",
                       myMap.getSonarReadingRight(positionX,positionY,angle),
                       myMap.getSonarReadingLeft(positionX,positionY,angle),
                       myMap.getSonarReadingFront(positionX,positionY,angle),
                       myMap.getSonarReadingBack(positionX,positionY,angle));

                usleep(200000.0);
            }

        }
        else if(strcmp(argv[1],"particleFilterMock2Map")==0){
            printf("ParticleFilterTest with map\n");
            RUNNING =1;
            sensorsModule mySensors;
            map myMap(MAP_DEBUG_MOCK2_FILE_NAME);
            double x = myMap.getStartLocationX();
            double y = myMap.getStartLocationY();
            particleFilter myParticleFilter(x,y,&mySensors, &myMap);
            myMap.printMapFile("myMapDebug.txt");
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html", "mapMock2.png");
                usleep(200000.0);
            }

        }
        else if(strcmp(argv[1],"particleFilterPracticeMap")==0){
            printf("ParticleFilterTest with map\n");
            RUNNING =1;
            sensorsModule mySensors;
            map myMap(MAP_DEBUG_PRACTICE_FILE_NAME);
            double x = myMap.getStartLocationX();
            double y = myMap.getStartLocationY();
            particleFilter myParticleFilter(x,y,&mySensors, &myMap);
            myMap.printMapFile("myMapDebug.txt");
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html", "practiceMap.png");
                usleep(200000.0);
            }

        }
        else if(strcmp(argv[1],"particleFilterPracticeMap2")==0){
            printf("ParticleFilterTest with map\n");
            RUNNING =1;
            sensorsModule mySensors;
            map myMap("myPracticeMapStartLocation2.txt");
            double x = myMap.getStartLocationX();
            double y = myMap.getStartLocationY();
            particleFilter myParticleFilter(x,y,&mySensors, &myMap);
            myMap.printMapFile("myMapDebug.txt");
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html", "practiceMap.png");
                usleep(200000.0);
            }

        }
        else if(strcmp(argv[1],"particleFilterPracticeMap3")==0){
            printf("ParticleFilterTest with map\n");
            RUNNING =1;
            sensorsModule mySensors;
            map myMap("myPracticeMapStartLocation3.txt");
            double x = myMap.getStartLocationX();
            double y = myMap.getStartLocationY();
            particleFilter myParticleFilter(x,y,&mySensors, &myMap);
            myMap.printMapFile("myMapDebug.txt");
            while(RUNNING){
                myParticleFilter.createSimpleWebpageView("particleFilter.html", "practiceMap.png");
                usleep(200000.0);
            }

        }
        */
        else if(strcmp(argv[1],"ultraShortIR")==0){
            printf("Running ultraShortIR sensor test\n");
            sensorsModule mySensors;

            RUNNING =1;
            while(RUNNING){
                double data = mySensors.frontUltraShortIRData;
                printf("%lf\n", data);
                usleep(200000.0);
            }
        }

        else if(strcmp(argv[1],"ultraShortIRCatch")==0){
            printf("Running ultraShortIR sensor test\n");
            sensorsModule mySensors;
            servosControl myServos;
            actuator myActuator;
            myActuator.hookServoAngle=&myServos.hookAngle;
            RUNNING =1;
            while(RUNNING){
                if(mySensors.frontUltraShortIRData < 0.95){
                    printf("%lf\n", mySensors.frontUltraShortIRData );
                    myServos.hookBlock();
                    usleep(500000.0);
                    myServos.unHookBlock();
                    usleep(500000.0);
                }
            }
        }

//        else if(strcmp(argv[1],"colorsensorraw")==0){
//            printf("Running color sensor test\n");
//            jankyColorSensor myColorSensor(COLOR_DETECTOR_PIN);
//            RUNNING =1;
//            while(RUNNING){
//                double data = myColorSensor.rawData();

//                printf("%lf \n", data);
//                usleep(200000.0);
//            }


//        }


        else if(strcmp(argv[1],"arewered")==0){
            printf("Running are we red test\n");
            sensorsModule mysensors;
            RUNNING =1;
            while(RUNNING){
                double data = mysensors.teamData;
                printf("%lf \n", data);
                usleep(200000.0);
            }
        }

        else if(strcmp(argv[1],"areweron")==0){
            printf("Running are we on test\n");
            sensorsModule mysensors;
            RUNNING =1;
            while(RUNNING){
                double data = mysensors.onData;
                printf("%lf \n", data);
                usleep(200000.0);
            }

        }

        else if(strcmp(argv[1],"testrobot")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            motorsControl mymotorsControl(&mysensors);
            servosControl myServoControl;
            ImageProcessor myImageProcessor;
            RUNNING =1;
            while(RUNNING){
                double variable1;
                double variable2;
                int answer;
                printf("Testing the robot. Testing all the sensors of the robot.\n\n");

                printf("Testing the sensors.\n");

                printf("Testing the front short IR.\n");
                printf("Put something near the front short IR and press enter.\n");
                getchar();
                variable1 = mysensors.frontShortIRData;
                printf("Distance read = %lf.\n", variable1);
                printf("Put something far from the front short IR and press enter.\n");
                getchar();
                variable2 = mysensors.frontShortIRData;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT FRONT SHORT IR TO ANALOG PIN %d.\n", FRONT_SHORTIR_PIN);
                }

                printf("Testing the right short IR.\n");
                printf("Put something near the right short IR and press enter.\n");
                getchar();
                variable1 = mysensors.rightShortIRData;
                printf("Distance read = %lf.\n", variable1);
                printf("Put something far from the right short IR and press enter.\n");
                getchar();
                variable2 = mysensors.rightShortIRData;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT RIGHT SHORT IR TO ANALOG PIN %d.\n", RIGHT_SHORTIR_PIN);
                }

                printf("Testing the left short IR.\n");
                printf("Put something near the left short IR and press enter.\n");
                getchar();
                variable1 = mysensors.leftShortIRData;
                printf("Distance read = %lf.\n", variable1);
                printf("Put something far from the left short IR and press enter.\n");
                getchar();
                variable2 = mysensors.leftShortIRData;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT LEFT SHORT IR TO ANALOG PIN %d.\n", LEFT_SHORTIR_PIN);
                }

                printf("Testing the left encoder.\n");
                printf("Turn the left wheel backwards and press enter.\n");
                getchar();
                variable1 = mysensors.leftEncoderMovement;
                printf("Distance read = %lf.\n", variable1);
                printf("Turn the left wheel forward and press enter.\n");
                getchar();
                variable2 = mysensors.leftEncoderMovement;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT LEFT ENCODER TO PINS: %d (blue) %d (white).\n", LEFT_ENCODER_ENC_A, LEFT_ENCODER_ENC_B);
                }

                printf("Testing the right encoder.\n");
                printf("Turn the right wheel backwards and press enter.\n");
                getchar();
                variable1 = mysensors.rightEncoderMovement;
                printf("Distance read = %lf.\n", variable1);
                printf("Turn the right wheel forward and press enter.\n");
                getchar();
                variable2 = mysensors.rightEncoderMovement;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT LEFT ENCODER TO PINS: %d (blue) %d (white).\n", RIGHT_ENCODER_ENC_A, RIGHT_ENCODER_ENC_B);
                }

                printf("Testing the front ULTRASHORT IR.\n");
                printf("put something in front of it and press enter.\n");
                getchar();
                variable1 = mysensors.frontUltraShortIRData;
                printf("Distance read = %lf.\n", variable1);
                printf("put NOTHING in front of it and press enter.\n");
                getchar();
                variable2 = mysensors.frontUltraShortIRData;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT FRONT ULTRASHORT IR TO PIN: %d .\n", FRONT_ULTRASHORTIR_PIN);
                }

                printf("Testing COLOR DETECTOR.\n");
                printf("put a GREEN cube in front of it, wait 2 seconds and press enter.\n");
                getchar();
                variable1 = mysensors.colorSensorData;
                printf("read = %lf.\n", variable1);
                printf("put RED BLOCK in front of it and press enter.\n");
                getchar();
                variable2 = mysensors.colorSensorData;
                printf("Distance read = %lf.\n", variable2);
                if(variable2 < variable1){
                    printf("ERROR. CONNECT COLOR DETECTOR TO PIN: %d .\n", COLOR_DETECTOR_PIN);
                }

                printf("Tested all the sensors\n\n");
                printf("Testing the motors\n");
                actuator myactuator(mymotorsControl, myServoControl);
                actPointer= &myactuator;
                printf("The robot will move 10 inches to the front when you press enter\n");
                getchar();
                mymotorsControl.setNewDesiredRelativePositionInRadialCordinates(10,0);
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT RIGHT MOTOR TO PINS: %d (DIR) %d (PWM).\n", RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM);
                    printf("ERROR. CONNECT LEFT MOTOR TO PINS: %d (DIR) %d (PWM).\n", LEFT_WHEEL_DIR, LEFT_WHEEL_PWM);

                }

                printf("The robot will move 10 inches to the back when you press enter\n");
                getchar();
                mymotorsControl.setNewDesiredRelativePositionInRadialCordinates(-10,0);
                printf("Did it work? 1 0\n");
                scanf("%d\n", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT RIGHT MOTOR TO PINS: %d (DIR) %d (PWM).\n", RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM);
                    printf("ERROR. CONNECT LEFT MOTOR TO PINS: %d (DIR) %d (PWM).\n", LEFT_WHEEL_DIR, LEFT_WHEEL_PWM);

                }

                printf("The robot will turn 45 degrees when you press enter\n");
                getchar();
                mymotorsControl.setNewDesiredRelativePositionInRadialCordinates(0,45);
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT RIGHT MOTOR TO PINS: %d (DIR) %d (PWM).\n", RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM);
                    printf("ERROR. CONNECT LEFT MOTOR TO PINS: %d (DIR) %d (PWM).\n", LEFT_WHEEL_DIR, LEFT_WHEEL_PWM);

                }

                printf("The robot will hook when you press enter\n");
                getchar();
                myServoControl.hookBlock();
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT HOOK TO PINS: %d (PWM)).\n", HOOK_SERVO_PWM);

                }

                printf("The robot will UNhook when you press enter\n");
                getchar();
                myServoControl.unHookBlock();
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT HOOK TO PINS: %d (PWM)).\n", HOOK_SERVO_PWM);

                }

                printf("The robot will lift its arm when you press enter\n");
                getchar();
                myServoControl.raiseBlock();
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT ARM TO PINS: %d (PWM)).\n", ARM_SERVO_PWM);

                }

                printf("The robot will put the sort wood to the right when you press enter\n");
                getchar();
                myServoControl.sortLeft();
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. CONNECT SORT TO PINS: %d (PWM)).\n", SORT_SERVO_PWM);

                }

                printf("The robot will now reset the servos when you press enter\n");
                getchar();
                myServoControl.reset();
                myServoControl.reset();

                printf("Now we will test the camera.\n");
                printf("Put a cube in front of the camera, wait and press enter.\n");
                getchar();
                int foundCube = myImageProcessor.getFoundCube();
                double cubePosition = myImageProcessor.getNearestCubeDist();
                double cubeAngle = myImageProcessor.getNearestCubeAngle();
                int cubeColor = myImageProcessor.getNearestCubeColor();
                if (foundCube)
                    printf("I found a cube at %lf in, %lf, degrees.\n It's color is %d\n' \n", cubePosition,cubeAngle, cubeColor);
                else{
                    printf("I DIDN'T find a cube at %lf in, %lf, degrees.\n It's color is %d\n' \n", cubePosition,cubeAngle, cubeColor);

                }
                printf("Did it work? 1 0\n");
                scanf("%d ", &answer);
                if (answer==0){
                    printf("ERROR. RESET EDISON FOR THE CAMERA TO WORK");

                }

                printf("YOU HAVE SUCESSFULLY TESTED THE ROBOT,\n\n");

                printf("Reset the robot now, before the competition to free memory and make sure that the camera works");
                RUNNING=0;
            }

        }
        return 0;
    }
}


