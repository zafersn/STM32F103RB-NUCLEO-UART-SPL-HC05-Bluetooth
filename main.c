#include "stm32f10x.h"                  // Device header

#define USART1_RxPin             GPIO_Pin_10
#define USART1_TxPin             GPIO_Pin_9 


 uint8_t RxCounter1= 0;


USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

uint8_t RxBuffer1[4];
uint8_t fff=0;



void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

int main(void)
{
  
    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration(); 
    
   /* USART configuration -------------------------------------------------
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
    
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
     /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USARTy Receive and Transmit interrupts */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   /* USART_ITConfig(USART1, USART_IT_TXE, ENABLE);*/
    
    /* Enable SPI_SLAVE */

     USART_Cmd(USART1, ENABLE);
     
      /* Wait until end of transmission from USART1 to HC05 */
     /* USART1'den HC05'a iletim bitene kadar bekleyin */
 
    
    while(1){}
   
   return 0;
}
void RCC_Configuration(void)
{  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
}

void GPIO_Configuration(void)
{
 
  /* Configure SPI_SLAVE pins: SCK and MOSI ---------------------------------*/
  /* Configure SCK and MOSI pins as Input Floating */
  
  
      GPIO_InitStructure.GPIO_Pin = USART1_RxPin; 				  													   /* NOTE: Configure USART1 Rx (PA.10) as input floating */
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    
      GPIO_InitStructure.GPIO_Pin = USART1_TxPin ; // RX: 10  TX:9
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void NVIC_Configuration(void)
{

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler()
{

    while(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)
  {
    /* Read one byte from the receive data register */
    RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);
    if(RxCounter1==4){
    RxCounter1=0;
    }
    

  }
  fff=1;
  
 /* if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    //USART_SendData(USART1, TxBuffer1[TxCounter1++]);

  /*
  }*/
}