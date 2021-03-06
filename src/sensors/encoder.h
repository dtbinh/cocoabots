#ifndef ENCODER_H
#define ENCODER_H

#include "mraa.hpp"
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include "../configFile.h"
#include "sensorssuperclass.h"

class encoder: public sensorsSuperClass
{
  public:
    encoder(int encPin);
    ~encoder();
    long long getCounts();
    double getData();
    double getRotations();
    static void edge_handler(void* encoderSensorPointer);

    //std::thread *runThread;
//    mraa::Gpio dirGpio;
    mraa::Gpio encGpio;

    volatile int dir; //This will be written by the actuator

    int running;
    long long edgeCount;
    double rotations;

    private:

};


#endif
