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