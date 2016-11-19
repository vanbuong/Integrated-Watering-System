/* Includes ------------------------------------------------------------------*/
#include "nRF24L01.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
volatile uint8_t payload_len;
volatile uint8_t PTX; // is sending Mode
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void nRF24_SPI_Init()
{
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI_PORT, &SPI_InitStructure);

	// NSS must be set to '1' due to NSS_Soft settings (otherwise it will be Multimaster mode).
	SPI_NSSInternalSoftwareConfig(SPI_PORT, SPI_NSSInternalSoft_Set);
	SPI_Cmd(SPI_PORT, ENABLE);
}

void nRF24L01_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);
	
	// Configure SPI pins
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN | SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RF24_CE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RF24_CE_GPIO, &GPIO_InitStructure);
	
//	// Configure IRQ pin as input with Pull-Up
//	GPIO_InitStructure.GPIO_Pin = nRF24_IRQ_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(RF24_IRQ_GPIO, &GPIO_InitStructure);

	nRF24_SPI_Init();

	RF24_CE(0);
	RF24_CSN(1);
}

void nRF24L01_Config(uint8_t channel, uint8_t pay_length)
{
	/* Use static payload length ... */
	payload_len = pay_length;

	// Set RF channel
	nRF24_ConfigRegister(RF_CH, channel);

	// Set length of incoming payload 
	nRF24_ConfigRegister(RX_PW_P0, 0x00); 						// Auto-ACK pipe ...
	nRF24_ConfigRegister(RX_PW_P1, payload_len); 			// Data payload pipe
	nRF24_ConfigRegister(RX_PW_P2, 0x00); 						// Data payload pipe
	nRF24_ConfigRegister(RX_PW_P3, 0x00); 						// Data payload pipe
	nRF24_ConfigRegister(RX_PW_P4, 0x00); 						// Data payload pipe
	nRF24_ConfigRegister(RX_PW_P5, 0x00); 						// Data payload pipe

	// 1 Mbps, TX gain: 0dbm
	nRF24_ConfigRegister(RF_SETUP, (0<<RF_DR)|((0x03)<<RF_PWR));

	// ACTIVE
	nRF24_ConfigRegister(ACTIVATE, 0x73);
	
	// CRC enable, 1 byte CRC length
	nRF24_ConfigRegister(CONFIG, nRF24_CONFIG);

	// Auto Acknowledgment
	nRF24_ConfigRegister(EN_AA, (0<<ENAA_P0)|(0<<ENAA_P1)|(0<<ENAA_P2)|(0<<ENAA_P3)|(0<<ENAA_P4)|(0<<ENAA_P5));

	// Enable RX addresses
	nRF24_ConfigRegister(EN_RXADDR, (1<<ERX_P0)|(1<<ERX_P1)|(0<<ERX_P2)|(0<<ERX_P3)|(0<<ERX_P4)|(0<<ERX_P5));

	// Auto retransmit delay: 1000 us and Up to 15 retransmit trials
	nRF24_ConfigRegister(SETUP_RETR,(0x04<<ARD)|(0x0F<<ARC));

	// Dynamic length configurations: No dynamic length
	nRF24_ConfigRegister(DYNPD, (0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5));

	//nRF24_ConfigRegister(FEATURE, (1<<0));

	// Start listening
	nRF24_PowerUpRx();
	nRF24_FlushRx();
}

uint8_t nRF24_GetPayloadLength()
{
	return payload_len;
}

void nRF24L01_RxAddress(uint8_t* adr)
{
	RF24_CE(0);
	nRF24_WriteRegister(RX_ADDR_P1,adr,nRF24_ADDR_LEN);
	RF24_CE(1);	
}

void nRF24L01_TxAddress(uint8_t* adr)
{
	/* RX_ADDR_P0 must be set to the sending addr for auto ack to work. */
	nRF24_WriteRegister(RX_ADDR_P0, adr, nRF24_ADDR_LEN);
	nRF24_WriteRegister(TX_ADDR, adr, nRF24_ADDR_LEN);
}

uint8_t nRF24L01_DataReady(void)
{
	uint8_t status = nRF24_GetStatus();
	if (status & (1 << RX_DR))
	{
		return 1;
	}
	return !nRF24_RxFifoEmpty();
}

void nRF24L01_GetData(uint8_t* data)
{
	/* Pull down chip select */
	RF24_CSN(0);

	/* Send cmd to read rx payload */
	SPI_SendByte(R_RX_PAYLOAD);
	
	/* Read payload */
	nRF24_TransferSync(data, data, payload_len);
	
	/* Pull up chip select */
	RF24_CSN(1);

	/* Reset status register */
	nRF24_ConfigRegister(STATUS, (1<<RX_DR));
}

void nRF24L01_Send(uint8_t* value)
{
	uint8_t status;
	status = nRF24_GetStatus();
	while (PTX)
	{	// Wait until last paket is send
		status = nRF24_GetStatus();
		if (status & ((1 << TX_DS) | (1 << MAX_RT)))
		{
			PTX = 0;
			break;
		}
	}
	/* Go to Standby-I first */
	RF24_CE(0);

	/* Set to transmitter mode , Power up if needed */
	nRF24_PowerUpTx();

	/* Pull down chip select */
	RF24_CSN(0);
	
	/* Write cmd to flush transmit FIFO */
	SPI_SendByte(FLUSH_TX);
	
	/* Pull up chip select */
	RF24_CSN(1);

	/* Pull down chip select */
	RF24_CSN(0);
	
	/* Write cmd to write payload */
	SPI_SendByte(W_TX_PAYLOAD);

	/* Write payload */
	nRF24_TransmitSync(value, payload_len);

	/* Pull up chip select */
	RF24_CSN(1);
	
	/* Start the transmission */
	RF24_CE(1);
	
}

/* Private functions ---------------------------------------------------------*/
uint8_t SPI_SendByte(uint8_t data)
{
	/*!< Wait until the transmit buffer is empty */
	while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_PORT, data);
	/*!< Wait to receive a byte*/
	while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI_PORT); // Read byte from SPI bus
}

/* Checks if receive FIFO is empty or not */
uint8_t nRF24_RxFifoEmpty(void)
{
	uint8_t fifoStatus;

	nRF24_ReadRegister(FIFO_STATUS, &fifoStatus, sizeof(fifoStatus));

	return (fifoStatus & (1 << RX_EMPTY));
}

/* Returns the length of data waiting in the RX fifo */
uint8_t nRF24_PayloadLengthRxFIFO(){
	uint8_t status;
	RF24_CSN(0);
	SPI_SendByte(R_RX_PL_WID);
	status = SPI_SendByte(0x00);
	RF24_CSN(1);
	return status;
}

/* Returns the number of retransmissions occured for the last message */
uint8_t nRF24_RetransmissionCount()
{
	uint8_t ret_val = 1;
	nRF24_ReadRegister(OBSERVE_TX, &ret_val, 1);
	ret_val = ret_val & 0x0F;
	return ret_val;
}

uint8_t nRF24_IsSending()
{
	uint8_t status;
	if (PTX)
	{
		/* read the current status */
		status = nRF24_GetStatus();
		
		/* if sending successful (TX_DS) or max retries exceded (MAX_RT). */
		if (status & ((1 << TX_DS)  | (1 << MAX_RT)))
		{
			//nRF24_PowerUpRx();
			return 0; /* false */
		}
		return 1; /* true */
	}
	else 
		return 0; /* false */
}

uint8_t nRF24_GetStatus()
{
	uint8_t ret_val = 0;
	nRF24_ReadRegister(STATUS, &ret_val,1);
	return ret_val;
}

uint8_t nRF24_LastMessageStatus()
{
	uint8_t rv;

	rv = nRF24_GetStatus();

	/* Transmission went OK */
	if (rv & (1 << TX_DS))
	{
		return nRF24_TRANSMISSON_OK;
	}
	/* Maximum retransmission count is reached */
	/* Last message probably went missing ... */
	else if (rv & (1 << MAX_RT))
	{
		return nRF24_MESSAGE_LOST;
	}
	/* Probably still sending ... */
	else
	{
		return 0xFF;
	}
}

void nRF24_PowerUpRx()
{
	PTX = 0;
	RF24_CE(0);
	nRF24_ConfigRegister(CONFIG, nRF24_CONFIG|((1<<PWR_UP)|(1<<PRIM_RX)));
	RF24_CE(1);
	nRF24_ConfigRegister(STATUS,(1<<TX_DS)|(1<<MAX_RT));
}

void nRF24_PowerUpTx()
{
  PTX = 1;
  nRF24_ConfigRegister(CONFIG, nRF24_CONFIG|((1<<PWR_UP)|(0<<PRIM_RX)));
}

void nRF24_PowerDown()
{
	RF24_CE(0);
	nRF24_ConfigRegister(CONFIG, nRF24_CONFIG);
}

void nRF24_FlushRx()
{
	RF24_CSN(0);
	SPI_SendByte(FLUSH_RX);
	RF24_CSN(1);
}

/* send and receive multiple bytes over SPI */
void nRF24_TransferSync(uint8_t* dataout, uint8_t* datain, uint8_t len)
{
	uint8_t i;
	for (i=0; i<len; i++)
	{
		datain[i] = SPI_SendByte(dataout[i]);
	}
}

/* send multiple bytes over SPI */
void nRF24_TransmitSync(uint8_t* dataout, uint8_t len)
{    
	uint8_t i=0;
	for (i=0; i<len; i++)
	{
		SPI_SendByte(dataout[i]);
	}
}

/* Clocks only one byte into the given nRF24 register */
void nRF24_ConfigRegister(uint8_t reg, uint8_t value)
{
	RF24_CSN(0);
	SPI_SendByte(W_REGISTER | (REGISTER_MASK & reg));
	SPI_SendByte(value);
	RF24_CSN(1);
}

/* Read single register from nRF24 */
void nRF24_ReadRegister(uint8_t reg, uint8_t* value, uint8_t len)
{
	RF24_CSN(0);
	SPI_SendByte(R_REGISTER | (REGISTER_MASK & reg));
	nRF24_TransferSync(value, value, len);
	RF24_CSN(1);
}

/* Write to a single register of nRF24 */
void nRF24_WriteRegister(uint8_t reg, uint8_t* value, uint8_t len)
{
	RF24_CSN(0);
	SPI_SendByte(W_REGISTER | (REGISTER_MASK & reg));
	nRF24_TransmitSync(value, len);
	RF24_CSN(1);
}
