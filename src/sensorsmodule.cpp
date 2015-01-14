﻿#include "sensorsmodule.h"
sensorsModule::sensorsModule():
    //Here is the initializer. It defines initial values for all the variables and initializes all the objects
    frontUltrasonicData(0),
    rightUltrasonicData(0),
    leftUltrasonicData(0),

    rightEncoderData(0),
    leftEncoderData(0),

    frontUltraShortIRData(0),
    rightUltraShortIRData(0),
    leftUltraShortIRData(0),
    backUltraShortIRData(0),

    frontShortIRData(0),
    rightShortIRData(0),
    leftShortIRData(0),
    backShortIRData(0),
    ultrasonicAlpha(0.7),
    ultraShortIRAlpha(0.5),
    shortIRAlpha(0.7)
    #if FRONT_ULTRASONIC
    ,frontUltrasonic(FRONT_ULTRASONIC_TR,FRONT_ULTRASONIC_EC)
    #endif

    #if RIGHT_ULTRASONIC
    ,rightUltrasonic(RIGHT_ULTRASONIC_TR,RIGHT_ULTRASONIC_EC)
    #endif

    #if LEFT_ULTRASONIC
    ,leftUltrasonic(LEFT_ULTRASONIC_TR,LEFT_ULTRASONIC_EC)
    #endif


    #if RIGHT_ENCODER
    ,rightEncoder(RIGHT_ENCODER_DIR, RIGHT_ENCODER_ENC)
    #endif

    #if LEFT_ENCODER
    ,leftEncoder(LEFT_ENCODER_DIR, LEFT_ENCODER_ENC)
    #endif



    #if FRONT_ULTRASHORTIR
    ,frontUltraShortIR(FRONT_ULTRASHORTIR_PIN)
    #endif

    #if BACK_ULTRASHORTIR
    ,backUltraShortIR(BACK_ULTRASHORTIR_PIN)
    #endif

    #if RIGHT_ULTRASHORTIR
    ,rightUltraShortIR(RIGHT_ULTRASHORTIR_PIN)
    #endif

    #if LEFT_ULTRASHORTIR
    ,leftUltraShortIR(RIGHT_ULTRASHORTIR_PIN)

    #endif



    #if FRONT_SHORTIR
    ,frontShortIR(FRONT_SHORTIR_PIN)
    #endif

    #if BACK_SHORTIR
    ,backShortIR(BACK_SHORTIR_PIN)
    #endif

    #if RIGHT_SHORTIR
    ,rightShortIR(RIGHT_SHORTIR_PIN)
    #endif

    #if LEFT_SHORTIR
    ,leftShortIR(LEFT_SHORTIR_PIN)
    #endif

    #if GYROSCOPE
    ,mygyroscope(GYROSCOPE_CHIP_PIN, GYROSCOPE_SPI_PIN)
    #endif
{
    running=1;
    runThread = new std::thread(run,this);
}


sensorsModule::~sensorsModule(){
    running=0;
    runThread->join();
    delete runThread;
}


void sensorsModule::run(sensorsModule * sensors){
    int started =0;

    while (sensors->running){
        #if FRONT_ULTRASONIC
        update(&sensors->frontUltrasonic,&sensors->frontUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if RIGHT_ULTRASONIC
        update(&sensors->rightUltrasonic,&sensors->rightUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if LEFT_ULTRASONIC
        update(&sensors->leftUltrasonic,&sensors->leftUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif


        #if RIGHT_ENCODER
        //update somehow the encoder
        #endif

        #if LEFT_ENCODER
        //update somehow the encoder
        #endif



        #if FRONT_ULTRASHORTIR
        update(&sensors->frontUltraShortIR,&sensors->frontUltraShortIR, sensors->ultraShortIRAlpha,started);
        #endif

        #if BACK_ULTRASHORTIR
        update(&sensors->backUltraShortIR,&sensors->backUltraShortIR, sensors->ultraShortIRAlpha,started);
        #endif

        #if RIGHT_ULTRASHORTIR
        update(&sensors->backUltraShortIR,&sensors->backUltraShortIR, sensors->ultraShortIRAlpha,started);
        #endif

        #if LEFT_ULTRASHORTIR
        update(&sensors->leftUltraShortIR,&sensors->leftUltraShortIR, sensors->ultraShortIRAlpha,started);
        #endif



        #if FRONT_SHORTIR
        update(&sensors->frontShortIR,&sensors->frontShortIR, sensors->shortIRAlpha,started);
        #endif

        #if BACK_SHORTIR
        update(&sensors->backShortIR,&sensors->backShortIR, sensors->backIRAlpha,started);
        #endif

        #if RIGHT_SHORTIR
        update(&sensors->rightShortIR,&sensors->rightShortIR, sensors->shortIRAlpha,started);
        #endif

        #if LEFT_SHORTIR
        update(&sensors->leftShortIR,&sensors->leftShortIR, sensors->shortIRAlpha,started);
        #endif

        #if GYROSCOPE
        //update the gyroscope somehow
        #endif

        started=1;
    }

}

void sensorsModule::update(sensorsSuperClass *sensor, double *data, float alpha, int started){
    //We want to have a time out here on the getData
    double newData = sensor->getData();
    if (started){
        *data = kalmanFilter(*data,newData, alpha);
    }
    else{
        *data=newData;
    }
}

double sensorsModule::kalmanFilter(double previousData, double newData, float alpha){
    return (previousData*alpha + newData * (1-alpha));
}

