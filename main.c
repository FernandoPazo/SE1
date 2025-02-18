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
  // Pull enable
  PORTC->PCR[3] |= PORT_PCR_PE(1);
  GPIOC->PDDR |= GPIO_PDDR_PDD(1<<3);
  GPIOC->PSOR |= GPIO_PSOR_PTSO(1<<3);

}

int green_act(){
  return ((GPIOC->PDIR & (1<<3)) == 0);
}

void sw_3_init(){
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
  PORTC->PCR[12] |= PORT_PCR_MUX(1);
  // Pull enable
  PORTC->PCR[12] |= PORT_PCR_PE(1);
  GPIOC->PDDR |= GPIO_PDDR_PDD(1<<12);
  GPIOC->PSOR |= GPIO_PSOR_PTSO(1<<12);
}

int red_act(){
  return ((GPIOC->PDIR & (1<<12)) == 0);
}

int main(void)
{
  led_green_init();
  led_red_init();
  sw_1_init();
  sw_3_init();

  while (1) {
    if(green_act()){
      led_green_toggle();
      while(green_act()){}
    }else if(red_act()){
      led_red_toggle();
      while(green_act()){}
    }
  }

  return 0;
}
