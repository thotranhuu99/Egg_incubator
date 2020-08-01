#include "stm8s.h"
void simpleDelay(void);
volatile uint8_t flag = 1;
int count;
uint8_t Tx_Buffer[] = "Hello world!!\r\n";
/*void UART_init();
void UART_putchar(char c);
void UART_getchar(void);
void UART_putstr(char *msg);*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void UART_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* CLK configuration -----------------------------------------*/
  CLK_Config();
  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();
  /* TIM1 configuration -----------------------------------------*/
  TIM1_Config();
  /* UART configuration -----------------------------------------*/
  UART_Config();

  while (1)
  { 
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
    {
    }
    UART1_SendData8(0x48);
    GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
    count++;
    while(flag);
    flag = 1;
  }
}

void simpleDelay(void)
{
	unsigned int i, j;
	
	for(i = 0; i < 1000; i ++)
	{
		for(j=0; j < 10; j ++)
		{
		}
	}
}

static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

static void TIM1_Config(void)
{
   TIM1_DeInit();       // Reset TM1 register
   CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);    //Clock for TIM1
   TIM1_TimeBaseInit((uint16_t) 15999, TIM1_COUNTERMODE_UP, 1000, 0);    //Overflow after 1s
   TIM1_ARRPreloadConfig(ENABLE);       //Enable auto reload
   TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);       //Interrupt when overflow
   TIM1_ClearFlag(TIM1_FLAG_UPDATE);    
   TIM1_Cmd(ENABLE);    //Enable timer1
   enableInterrupts();  //Enable global interrupt
}
static void GPIO_Config(void)
{
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
}

static void UART_Config(void)
{
  UART1_DeInit();
  UART1_Init((uint32_t) 9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
}
  

INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  flag = 0;
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
}


/*
void UART_Init(){
  UART1_CR1_M = 0; // 8 data bit
  UART1_CR1_PCEN = 0; // No parity
  UART1_CR3_STOP = 0; // 1 stop bit
  
  //Baud rate 9600
  UART1_BRR1 = 0x0D; // Baud rate register to 9600 baud (2Mhz/9600 = 0xD0)
  UART1_BRR2 = 0x00  // Based upon 2Mhz system clock
  
  UART1_CR2_TEN = 1; // Transmission enable
  UART1_CR2_REN = 1; // Receiver enable
}

void UART_putchar(char c){
  while (UART1_SR_TXE == 0); // Transmit data register empty
  UART1_DR = c;
}

void UART_getchar(void){
  while (!UART1_SR_RXNE); // Read data register is empty
  return UART1_DR;
}

void UART_putstr(char *msg){
  char *ch = msg;
  while (*ch){
    UART_putchar(*ch);
    ch++;
  }
}

*/

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  while(1){}
}
#endif