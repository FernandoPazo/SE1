#include "MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5
// LED_RED = PTE29

void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}

// LED_GREEN = PTD5
void led_green_init()
{
  // SIM->COPC
  SIM->COPC &= 0;
  // SIM->SCGC5
  SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
  // PORTD->PCR[5]
  PORTD->PCR[5] |= PORT_PCR_MUX(1);
  // GPIOD->PDDR
  GPIOD->PDDR |= GPIO_PDDR_PDD(1<<5);
  // GPIOD->PSOR
  GPIOD->PSOR |= GPIO_PSOR_PTSO(1<<5);
}

void led_green_toggle()
{
  GPIOD->PTOR |= GPIO_PTOR_PTTO(1<<5);
}

// LED_RED = PTE29
void led_red_init()
{
  // SIM->SCGC5
  SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
  // PORTD->PCR[29]
  PORTE->PCR[29] |= PORT_PCR_MUX(1);
  // GPIOD->PDDR
  GPIOE->PDDR |= GPIO_PDDR_PDD(1<<29);
  // GPIOD->PSOR
  GPIOE->PSOR |= GPIO_PSOR_PTSO(1<<29);
}

void led_red_toggle(void)
{
  GPIOE->PTOR |= GPIO_PTOR_PTTO(1<<29);
}

void sw_1_init(){
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[3] |= PORT_PCR_MUX(1);
  PORTC->PCR[3] |= PORT_PCR_PE(1);
  GPIOC->PDDR |= GPIO_PDDR_PDD(1<<3);
  GPIOC->PSOR |= GPIO_PSOR_PTSO(1<<3);

}

int sw_der(){
  return ((GPIOC->PDIR & (1<<3)) == 0);
}

void sw_3_init(){
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[12] |= PORT_PCR_MUX(1);
  PORTC->PCR[12] |= PORT_PCR_PE(1);
  GPIOC->PDDR |= GPIO_PDDR_PDD(1<<12);
  GPIOC->PSOR |= GPIO_PSOR_PTSO(1<<12);
}

int sw_izq(){
  return ((GPIOC->PDIR & (1<<12)) == 0);
}

int main(void){

  int STATE = 0;
  int PrevState = 0;
  led_green_init();
  led_red_init();
  sw_1_init();
  sw_3_init();
  led_green_toggle();

  while (1) {
    if(STATE == 0){         //STATE SAFE
      if(PrevState != 0){
        led_green_toggle(); while(sw_der()){}
        led_red_toggle(); while(sw_izq()){}
      }
      if(sw_der())
        STATE = 1;
      else if(sw_izq())
        STATE = 2;
      else if(sw_der() & sw_izq())
        STATE = 3;
      PrevState = 0;
    }
    else if(STATE == 1){

      if(PrevState == 0){
        led_green_toggle();
        while(sw_der()){}
        led_red_toggle();
        while(sw_izq()){}
      }
      
      if(sw_der())
        STATE = 0;
      else if(sw_izq())
        STATE = 3;
      else if(sw_der() & sw_izq())
        STATE = 2;
      PrevState = 1;
    }
    else if(STATE == 2){

      if(PrevState == 0){
        led_green_toggle();
        while(sw_der()){}
        led_red_toggle();
        while(sw_izq()){}
      }
      
      if(sw_der())
        STATE = 3;
      else if(sw_izq())
        STATE = 0;
      else if(sw_der() & sw_izq())
        STATE = 1;
      PrevState = 2;
    }
    else if(STATE == 3){

      if(PrevState == 0){
        led_green_toggle();
        while(sw_der()){}
        led_red_toggle();
        while(sw_izq()){}
      }
      
      if(sw_der())
        STATE = 2;
      else if(sw_izq())
        STATE = 1;
      else if(sw_der() & sw_izq())
        STATE = 0;
      PrevState = 3;
    }
  }

  return 0;
}
