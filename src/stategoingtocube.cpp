#include "stategoingtocube.h"
#include "statecollectingcube.h"
#include "statelookingforblocks.h"


stateGoingToCube::stateGoingToCube(states *previousState):states(previousState)
{
    name= "State Going to Cube";
}

void stateGoingToCube::processData(){
    //Wait to get a still image
    if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 > GO_TO_CUBE_WAIT_TIME_MS){
        if(foundCube()){
            static double distance = myImageProcessor->getNearestCubeDist();
            static double angle = myImageProcessor->getNearestCubeAngle();
            static int color = myImageProcessor->getNearestCubeColor();
            startProcessData();

            goToPoint(distance,angle);

            if (finishedGoingToPoint){
                nextState = new stateCollectingCube(this,color);
            }
        }
        else{
            nextState=new stateLookingForBlocks;
        }
        finishProcessData();
    }
}
