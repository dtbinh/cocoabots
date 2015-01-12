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
    static void echo_handler(int lol);

  private:
    int Trig_pin, Echo_pin;
    // added this structure, I believe it's needed
    struct timeval end,start;
    long duration,distance_cm,distance_in;
    
};

#endif