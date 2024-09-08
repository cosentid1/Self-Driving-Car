#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "tm4c1294ncpdt.h"
#include <math.h>
#include "onboardLEDs.h"

#define DEGREE 11.25
 
/*

This code is used to activate the ports needed to turn the car with a second motor
Although the final design does not contain a second motor, an older version did

void PortE_Init(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4) == 0){};
	GPIO_PORTE_DIR_R = 0x0F;                         // Configure Port E pins (PE0-PE3) as output
	GPIO_PORTE_AFSEL_R &= ~0x0F;
	GPIO_PORTE_DEN_R = 0x0F;                         // Enable digital I/O on Port E pins (PE0-PE3)
  GPIO_PORTE_AMSEL_R &= ~0x0F;
	return;
}
*/

void PortL_Init(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};
	GPIO_PORTL_DIR_R = 0x0F;                         // Configure Port L pins (PL0-PL3) as output
	GPIO_PORTL_AFSEL_R &= ~0x0F;
	GPIO_PORTL_DEN_R = 0x0F;                         // Enable digital I/O on Port L pins (PL0-PL3)
  GPIO_PORTL_AMSEL_R &= ~0x0F;
	return;
}

void PortH_Init(void) {
    // Use PortH pins (PH0-PH3) for output
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;          // Activate clock for Port H
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R7) == 0) {}  
    GPIO_PORTH_DIR_R = 0x01;                         // Configure Port H pins (PH0) as output
		GPIO_PORTH_DEN_R = 0x01;                         // Enable digital I/O on Port H pins (PH0)
		return;
}

void PortK_Init(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R9;
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R9) == 0){};
	GPIO_PORTK_DIR_R = 0x00;                         // Configure Port K pins (PK0) as output
	GPIO_PORTK_DEN_R = 0x01;                         // Enable digital I/O on Port K pins (PK0)
	return;
}

//various functions for operating the interupts
void EnableInt(void){
	__asm("    cpsie   i\n");
}

void DisableInt(void){
	__asm("    cpsid   i\n");
}

void WaitForInt(void){
	__asm("    wfi\n");
}

//function to activate an internal timer on the microcontroller and set it to periodic mode
void Timer0_Init(void){

	SYSCTL_RCGCTIMER_R = 0x01;
	SysTick_Wait10ms(1);
	
	TIMER0_CTL_R = 0;
	TIMER0_CFG_R = 0;
	TIMER0_TAMR_R = 0x2;
	TIMER0_TAPR_R = 0;
	TIMER0_TAILR_R = (1000000*120)-1;
	TIMER0_ICR_R = 0x1;
	TIMER0_IMR_R = 0x1;
	
	NVIC_EN1_R = 0x00000008;
	NVIC_PRI8_R = 0x40000000;
	
	EnableInt();
}

uint32_t read = 0;
uint32_t distance = 0;


//makes the motor turn right, which, when coupled with the gear system, causes the car to move forward
void driveForwards(int delay) {
	
	for(int i=0; i<512*DEGREE/360; i++){
		
		GPIO_PORTL_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00001001;
		SysTick_Wait(delay);
	}
}

//makes the motor turn left, which, when coupled with the gear system, causes the car to move backwards
void driveBackwards(int delay) {
	
	for(int i=0; i<512*DEGREE/360; i++){
		
		GPIO_PORTL_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTL_DATA_R = 0b00000011;
		SysTick_Wait(delay);
	}
}

/*

This code is used to activate the ports needed to turn the car with a second motor
Although the final design does not contain a second motor, an older version did

void turnLeft(int delay) {
	
	for(int i=0; i<512*DEGREE/360; i++){
		
		GPIO_PORTE_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00001001;
		SysTick_Wait(delay);
	}
}


void turnRight(int delay) {
	
	for(int i=0; i<512*DEGREE/360; i++){
		
		GPIO_PORTE_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTE_DATA_R = 0b00000011;
		SysTick_Wait(delay);
	}
}
*/

//Interrupt handler to acknowledge the interrupt and allow the code to continue
void TIMER0A_IRQHandler(void){
	TIMER0_ICR_R = 0x1;
}

//used to find the distance of the closest wall/object 
void getDistance(void){
	GPIO_PORTK_DATA_R |= 0x01; //tuns on the acitvation pin	
	SysTick_Wait10ms(1);
	
	TIMER0_CTL_R = 0x01; //tells the internal timer to begin counting
	WaitForInt(); //waits for the interrupt
	TIMER0_CTL_R = 0x00; //stops the timer
	GPIO_PORTK_DATA_R &= 0x00; //turn off the activatino pin
	distance = (0.0272*TIMER0_TAR_R)/2; //calculates the actual distance of the signal based on the bus speed
}

int main(void){
	PLL_Init();	
	SysTick_Init();
	//PortE_Init();
	PortH_Init();
	PortK_Init();
	PortL_Init();
	Timer0_Init();
	onboardLEDs_Init();
	int i;	
	
	while(1){
		if(distance < 35){ //will enter the if when the distance calculated is less than 35
			
			//once an obstacle is detected, the car will back up, in future expansion of this design, the car would attempt to turn away from the obstacle with a turning mechanism
			for(int i = 0; i < 20; i++){
				driveBackwards(250000);
			}
		}else{
			driveForwards(250000); //if no obstacle is detected, the car will proceed forward
		}
	}
}