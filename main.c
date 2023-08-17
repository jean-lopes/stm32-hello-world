#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

void led_on() {
  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void led_off() {
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

int main(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

  // init LED Port and pin
  GPIO_InitTypeDef s;
  s.GPIO_Pin   = GPIO_Pin_13;
  s.GPIO_Mode  = GPIO_Mode_OUT;
  s.GPIO_Speed = GPIO_Speed_2MHz;
  s.GPIO_OType = GPIO_OType_PP;
  s.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &s);

  // Init USART port and pins
  s.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
  s.GPIO_Mode  = GPIO_Mode_AF;
  s.GPIO_Speed = GPIO_Speed_50MHz;
  s.GPIO_OType = GPIO_OType_PP;
  s.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &s);

  // Connect pins to AF pins of USART
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitTypeDef u;
  u.USART_BaudRate            = 115200;
  u.USART_WordLength          = USART_WordLength_8b;
  u.USART_StopBits            = USART_StopBits_1;
  u.USART_Parity              = USART_Parity_No;
  u.USART_Mode                = USART_Mode_Tx;
  u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART1, &u);
  USART_Cmd(USART1, ENABLE);

  led_off();
  while(1) {
    led_on();
    for(long i=0; i<SystemCoreClock/10; i++){__NOP();}
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    USART_SendData(USART1, 75);
    led_off();
    for(long i=0; i<SystemCoreClock/10; i++){__NOP();}
  }

  return 0;
}
