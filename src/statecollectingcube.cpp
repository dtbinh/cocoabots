#include "statecollectingcube.h"
#include "statelookingforblocks.h"
stateCollectingCube::stateCollectingCube(states *previousState):states(previousState)
{
}

void stateCollectingCube::processData(){
    startProcessData();

    collectBlock(myImageProcessor->getNearestCubeColor()); //THIS IS WRONG. THIS IS FOR THE MOCK COMPETITION

    if (finishedCollectingBlock){
        nextState = new stateLookingForBlocks(this);
    }
    finishProcesData();
}