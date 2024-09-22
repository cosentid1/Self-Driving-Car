# Self-Driving Car

This is a custom built vehicle that can travel around an area and detect obstacles using an ultrasonic sensor. If an object is detected, an interrupt will cause the car to stop and back away from it. The car is controlled by an MSP-EXP432E401Y microcontroller and the code is written in C.

## Table of Contents

  1) Design Overview
  2) Demo Video
  3) Design Process
  4) Next Steps

## Design Overview
The aim of this project was to develop a more fundamental understanding of sensors and how they interact with both a microcontroller and the envrionment. It was also an attempt to create a device that is constantly moving and interacting with its envrionment in real-time.

This vehicle is powered by an MSP-EXP432E401Y microcontroller that communicates with an ultrasonic sensor through interrupts. The controller will send out a signal to activate the sensor. At this point, an internal timer module on the processor is started. The sensor will send out a signal and once it returns, will tell the processor to stop the timer through an interrupt. Based on the processors internal clock cycle, which is 120Mhz, the time recorded by the module and the speed of sound, the distance of the nearest obstacle is calcualted. If this value is less than 35, then the car will stop and drive backwards for about 8 full rotations of the stepper motor. The body of the car was made using K'nex and was designed to evenly distribute the amount of increased friction that putting the processor and the battery pack on top would cause.
  
## Demo Video
https://github.com/user-attachments/assets/04cecd91-b700-4dd8-a754-1bb1cae70725

## Design Process
To build this car, first the frame and the drving mechanics were made. The entire frame of the car was made using K'nex as this was the easiest way to attach a piece to the rotating part of the stepper motor. The motors were not very strong on their own so a gear system was used to increase the torque generated by the motor. Once this was working, the frame was extended and the system was tested to make sure it would actually move when left on its own.


https://github.com/user-attachments/assets/2b4f0393-d715-4e5f-99ad-8616dd054645



## Next Steps
As seen in the design process, the car was originally supposed to turn to avoid obstacles but the motor at the front made it too heavy and the car would not move. To be able to reimplement the turning mechanism, a stronger motor would have to be used at the back, or a more complex gear system to increase torque could also be a solution to this problem. Another improvement could be to add sensors to the side and back of the car. Currently the car only avoids obstacles by backing away from them. With more sensors, the car could try turning to the left or right based on data collected so it can navigate around any room on its own.
