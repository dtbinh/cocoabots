#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_CM 1
#define ULTRASONIC_INC 0

#include <sys/time.h>
#include "mraa.hpp"

class ultrasonic
{
  public:
    ultrasonic(int TP, int EP);
    long timing();
    long ranging(int sys);
    void echo_handler(void* args);

  private:
    mraa::Gpio TP_gpio, EP_gpio;
    int Trig_pin;
    int Echo_pin;
    // added this structure, I believe it's needed
    struct timeval end,start;
    long duration,distance_cm,distance_in;
    
};

#endif