#include "stm8s.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHT30 0x44
/* USER CODE END PD */

volatile uint8_t flag = 1;
volatile uint8_t ACK_to_send=0;
volatile uint8_t RxCounter1 = 0x00;

double temperature = 100;
double temperature_pre =100;
double humidity;
double humidity_pre;
double temperature_set = 50;
double temperature_max = 70;
uint8_t manual_mode_run = 0;
uint8_t Receive_error;
uint8_t auto_mode =1;
uint8_t time=0;

uint8_t soft_reset[2] = {0x30,0xA2};
uint8_t break_cmd[2] = {0x30,0x93};
uint8_t single_shot[2]= {0x2C, 0x06};
uint8_t periodic[2]= {0x22, 0x36};
uint8_t fetch[2] = {0xE0, 0x00};
uint8_t receive[6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t Send_UART[11];
static uint8_t Receive_UART[8];

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void UART_Config(void);
static void TIM1_Config(void);
static void I2C_Config(void);

/* Private functions ---------------------------------------------------------*/
void I2C_Receive(uint8_t address, uint8_t *data, uint8_t len);
void I2C_Transmit( uint8_t address, uint8_t *cmd, uint8_t len);
void SHT30_Initialize();
void UART_Transmit(uint8_t *ch, uint8_t len);
void simpleDelay(void);
static uint8_t crc8(const uint8_t *data, int len) {
	const uint8_t POLYNOMIAL = 0x31;
  uint8_t crc = 0xFF;
	for (int j = len; j; --j) {
    crc ^= *data++;

    for (int i = 8; i; --i) {
      crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
    }
  }
	return crc;
}
static double calculate_temperature(uint8_t MSB, uint8_t LSB, uint8_t crc)	
{	
	uint8_t data[2] ={MSB, LSB};
	uint8_t crc_calculated = crc8(data,2);
	if (crc == crc_calculated)
	{temperature_pre = temperature;
	return( 175*((double)MSB*256 + (double)LSB)/65535 -45);
	}
	else return(temperature_pre);
}
static double calculate_humidity(uint8_t MSB, uint8_t LSB, uint8_t crc)
{	uint8_t data[2] ={MSB, LSB};
	uint8_t crc_calculated = crc8(data,2);
	if (crc == crc_calculated)
	{humidity_pre = humidity;
	return( 100*((double)MSB*256 + (double)LSB)/65535);
	}
	else return(humidity_pre);
}
void Pack_data_to_send_UART(uint8_t *receive, uint8_t ACK_send)
{
	Send_UART[0]=0x53;// STX[1] (character S)
	Send_UART[1]=0x65;// STX[2] (character e)
	if (ACK_send ==1)
	{
		Send_UART[8]=0x2B;//ACK (character +)
	}
	else
	{
	Send_UART[8]=0x2D; //Not ACK (character -)
	}
	Send_UART[9]=0x45;// ETX[1] (character E)
	Send_UART[10]=0x6e;// ETX[2] (character n)
	for (int i=0; i<6; i++)
		{
				Send_UART[i+2] = receive[i];
		}
}
uint8_t Process_UART_received(uint8_t Receive_UART[8])
{	
	if(Receive_UART[0]=0x53, Receive_UART[1]=0x65, Receive_UART[6]=0x45, Receive_UART[7]=0x6e)
	{
		if (Receive_UART[2] == 0x54)
		{
				uint8_t data[2] ={Receive_UART[3], Receive_UART[4]};
				if (Receive_UART[5] == crc8(data,2))
				{
					temperature_set = calculate_temperature(Receive_UART[3],Receive_UART[4], Receive_UART[5]);
					return(0);
				}
				else
				{
					return(1);
				}
		}
		if (Receive_UART[2] == 0x52)
		{
			manual_mode_run =1;
			return(0);
		}
		if (Receive_UART[2] == 0x50)
		{
			manual_mode_run =0;
			return(0);
		}
	}
		return(1);
}
void control_on_off(uint8_t auto_mode, uint8_t run, double temperature, double temperature_set)
{
	if (temperature < temperature_max)
	{
			if (auto_mode == 0)
			{
				if (run == 1)
				{
					if ((temperature_set - 0.5) > temperature) //-deadband
					{
						GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
					}
					if ((temperature_set) < temperature) //+deadband
					{
						GPIO_WriteLow(GPIOA, GPIO_PIN_1);
					}
				}
				else
				{
					GPIO_WriteLow(GPIOA, GPIO_PIN_1);
				}
			}
			if (auto_mode ==1)
			{
				if ((temperature_set - 0.5) > temperature) //-deadband
				{
					GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
				}
				if ((temperature_set) < temperature) //+deadband
				{
					GPIO_WriteLow(GPIOA, GPIO_PIN_1);
				}
			}

	}
	if (temperature >= temperature_max)
	{
		GPIO_WriteLow(GPIOA, GPIO_PIN_1);
	}
}
void SHT30_check_connection(double temperature, double temperature_pre, double humidity, double humidity_pre)
{
	static int times;
	if (temperature == temperature_pre & humidity == humidity_pre)
	{
		times ++;
	}
	else
	{
		times =0;
	}
	if( times > 4)
	{
		GPIO_WriteLow(GPIOA, GPIO_PIN_1);
		WWDG->CR = 0x80;
	}
}

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
  /* I2C configuration -----------------------------------------*/
  I2C_Config();
  /* SHT30 Init*/
  SHT30_Initialize();
  while (1)
  { 
    I2C_Transmit(SHT30<<1, fetch, 2);
    simpleDelay();
    I2C_Receive(SHT30<<1, receive, 6);
    temperature = calculate_temperature(receive[0], receive[1], receive[2]);
    humidity = calculate_humidity(receive[3], receive[4], receive[5]);
    Pack_data_to_send_UART(receive, ACK_to_send);
    UART_Transmit(Send_UART,11);
    ACK_to_send = 0;
    control_on_off(auto_mode, manual_mode_run, temperature, temperature_set);
    while(flag);
    flag = 1;
    time = 0;
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

void UART_Transmit(uint8_t *send, uint8_t len)
{
  for (int i=0;i<len;i++)
  {
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(*send);
    send ++;
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
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  UART1_Cmd(ENABLE);
  enableInterrupts();
}
 
static void GPIO_Config(void)
{
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);
}

INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  flag = 0;
  if (time++>5)
  {
    GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    WWDG->CR = 0x80;
  }
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
}

INTERRUPT_HANDLER (UART1_RX_IRQHandler, 18)
{
  if(UART1_GetFlagStatus(UART1_FLAG_RXNE)!= RESET)
  {
    Receive_UART[RxCounter1++]=UART1_ReceiveData8();
  }
  
  if (RxCounter1 == 8)
  {
    auto_mode = 0;
    Receive_error = Process_UART_received(Receive_UART);
    if (Receive_error == 0)
    {
	ACK_to_send = 1;
    }
    RxCounter1 =0;
    UART1_ClearFlag(UART1_FLAG_RXNE);
    if((Receive_UART[6] != 0x45)|(Receive_UART[7] != 0x6e))
    {
      UART_Config();
    }
  }
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