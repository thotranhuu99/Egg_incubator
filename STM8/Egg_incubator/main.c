#include "stm8s.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHT30 0x44
/* USER CODE END PD */

volatile uint8_t flag = 1;
int count;
uint8_t soft_reset[2] = {0x30,0xA2};
uint8_t break_cmd[2] = {0x30,0x93};
uint8_t single_shot[2]= {0x2C, 0x06};
uint8_t periodic[2]= {0x22, 0x36};
uint8_t fetch[2] = {0xE0, 0x00};
uint8_t receive[6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t Send_UART[] = {0x53, 0x65, 0x30,0x31,0x32,0x33,0x34,0x35,0x2D,0x45,0x6E};
/*void UART_init();
void UART_putchar(char c);
void UART_getchar(void);
void UART_putstr(char *msg);*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void UART_Config(void);
static void TIM1_Config(void);
static void I2C_Config(void);

/* Private functions ---------------------------------------------------------*/
void I2C_Receive(uint8_t address, uint8_t *data, uint8_t len);
void I2C_Transmit( uint8_t address, uint8_t *cmd, uint8_t len);
void SHT30_Initialize();
void UART_Transmit(uint8_t *ch, uint8_t len);
void simpleDelay(void);
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
  //GPIO_Config();
  /* TIM1 configuration -----------------------------------------*/
  TIM1_Config();
  /* UART configuration -----------------------------------------*/
  UART_Config();
  /* I2C configuration -----------------------------------------*/
  I2C_Config();
  /* SHT30 Init*/
  SHT30_Initialize();
  while (1)
  { 
    I2C_Transmit(SHT30<<1, fetch, 2);
    I2C_Receive(SHT30<<1, receive, 6);
    UART_Transmit(Send_UART,11);
    count++;
    while(flag);
    flag = 1;
  }
}

// User defined functions
void simpleDelay(void)
{
	unsigned int i, j;
	
	for(i = 0; i < 1000; i ++)
	{
		for(j=0; j < 1000; j ++)
		{
		}
	}
}

void UART_Transmit(uint8_t *ch, uint8_t len)
{
  for (int i=0;i<len;i++)
  {
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(*ch);
    ch ++;
  }
}

void I2C_Transmit( uint8_t address, uint8_t *cmd, uint8_t len)
{
  while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY) == SET);
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT ));
  I2C_Send7bitAddress((uint8_t) address, I2C_DIRECTION_TX); //Send address
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  for(uint8_t i=0;i<len;i++)
  {
    I2C_SendData(cmd[i]);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));  
  }
  while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
  I2C_GenerateSTOP(ENABLE);
  I2C->SR1;I2C->SR3;
}

void I2C_Receive(uint8_t address, uint8_t *data, uint8_t len)
{
  while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY) == SET);
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(address, I2C_DIRECTION_RX);
  while(I2C_GetFlagStatus( I2C_FLAG_ADDRESSSENTMATCHED) == RESET);
  I2C_AcknowledgeConfig(I2C_ACK_NONE);
  I2C->SR1;I2C->SR3;
  for (uint8_t i=0;i<len-1;i++)
  {
    I2C_AcknowledgeConfig( I2C_ACK_CURR);
    while(I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET );
    data[i] = I2C_ReceiveData();
    while(I2C->CR2 & I2C_CR2_STOP);
  }
  
  I2C_AcknowledgeConfig( I2C_ACK_NONE);
  while(I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET );
  data[len-1] = I2C_ReceiveData();
  while(I2C->CR2 & I2C_CR2_STOP);
  I2C_GenerateSTOP(ENABLE);
  I2C->SR1;I2C->SR3;
  
}

void SHT30_Initialize()
{
	simpleDelay();
	I2C_Transmit(SHT30<<1, break_cmd, 2);
	simpleDelay();
	I2C_Transmit(SHT30<<1, soft_reset, 2);
	simpleDelay();
	I2C_Transmit(SHT30<<1, periodic, 2);
	simpleDelay();
}
 

// End of user defined functions
static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

static void I2C_Config(void)
{
  uint8_t Input_Clock;
  I2C_DeInit();
  Input_Clock = CLK_GetClockFreq()/1000000;
  I2C_Init(400000, SHT30, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
  I2C_Cmd(ENABLE);
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