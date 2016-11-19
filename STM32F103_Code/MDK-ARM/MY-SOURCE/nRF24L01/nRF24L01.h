#ifndef __nRF24L01_H
#define __nRF24L01_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define nRF24_CMD_LED1_ON				"#CMD LED1 ON    "
#define nRF24_CMD_LED1_OFF			"#CMD LED1 OFF   "
#define nRF24_CMD_LED2_ON				"#CMD LED2 ON    "
#define nRF24_CMD_LED2_OFF			"#CMD LED2 OFF   "

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE	    0x1D

/* Bit Mnemonics */

/* configuratio nregister */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

/* enable auto acknowledgment */
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

/* enable rx addresses */
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

/* setup of address width */
#define AW          0 /* 2 bits */

/* setup of auto re-transmission */
#define ARD         4 /* 4 bits */
#define ARC         0 /* 4 bits */

/* RF setup register */
#define PLL_LOCK    4
#define RF_DR			  3
#define RF_PWR      1 /* 2 bits */   

/* general status register */
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1 /* 3 bits */
#define TX_FULL     0

/* transmit observe register */
#define PLOS_CNT    4 /* 4 bits */
#define ARC_CNT     0 /* 4 bits */

/* fifo status */
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

/* dynamic length */
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5

/* Instruction Mnemonics */
#define R_REGISTER    				0x00 /* last 4 bits will indicate reg. address */
#define W_REGISTER    				0x20 /* last 4 bits will indicate reg. address */
#define REGISTER_MASK 				0x1F
#define R_RX_PAYLOAD  				0x61
#define W_TX_PAYLOAD  				0xA0
#define FLUSH_TX      				0xE1
#define FLUSH_RX      				0xE2
#define REUSE_TX_PL   				0xE3
#define ACTIVATE      				0x50 
#define R_RX_PL_WID   				0x60
#define NOP           				0xFF
#define W_TX_PAYLOAD_NO_ACK		0x58

#define nRF24_ADDR_LEN 5
#define nRF24_CONFIG ((1<<EN_CRC)|(1<<CRCO))	// Enable CRC, CRC Encoding scheme 2 byte

#define nRF24_TRANSMISSON_OK 0
#define nRF24_MESSAGE_LOST   1

#define SPI_DUMMY_BYTE   0xFF

// nRF24L01 GPIO Configuration
#define SPI_PORT      SPI1
#define SPI_SCK_PIN   GPIO_Pin_5     // PA5
#define SPI_MISO_PIN  GPIO_Pin_6     // PA6
#define SPI_MOSI_PIN  GPIO_Pin_7     // PA7
#define SPI_CS_PIN    GPIO_Pin_4     // PA4
#define SPI_GPIO			GPIOA

// nRF24L01 CE (Chip Enable) pin
#define RF24_CE_GPIO     GPIOC
#define RF24_CE_PIN      GPIO_Pin_3    // PA0

// nRF24L01 IRQ pin
#define RF24_IRQ_GPIO    GPIOA
#define RF24_IRQ_PIN     GPIO_Pin_1    // PA1

/* Private macro -------------------------------------------------------------*/
// Chip Enable Activates RX or TX mode
#define RF24_CE(x)				x?GPIO_SetBits(RF24_CE_GPIO, RF24_CE_PIN):GPIO_ResetBits(RF24_CE_GPIO, RF24_CE_PIN)
// SPI Chip Select
#define RF24_CSN(x) 			x?GPIO_SetBits(SPI_GPIO, SPI_CS_PIN):GPIO_ResetBits(SPI_GPIO, SPI_CS_PIN)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* adjustment functions */
void nRF24_SPI_Init(void);
void nRF24L01_Init(void);
void nRF24L01_Config(uint8_t channel, uint8_t pay_length);
void nRF24L01_RxAddress(uint8_t* adr);
void nRF24L01_TxAddress(uint8_t* adr);

/* state check functions */
uint8_t nRF24L01_DataReady(void);

uint8_t nRF24_IsSending(void);
uint8_t nRF24_GetStatus(void);
uint8_t nRF24_RxFifoEmpty(void);

/* core TX / RX functions */
void    nRF24L01_Send(uint8_t* value);
void    nRF24L01_GetData(uint8_t* data);

/* use in dynamic length mode */
uint8_t nRF24_PayloadLengthRxFIFO(void);

/* post transmission analysis */
uint8_t nRF24_LastMessageStatus(void);
uint8_t nRF24_RetransmissionCount(void);

/* Returns the payload length */
uint8_t nRF24_GetPayloadLength(void);

/* power management */
void    nRF24_PowerUpRx(void);
void    nRF24_PowerUpTx(void);
void    nRF24_PowerDown(void);
void		nRF24_FlushRx(void);

/* low level interface ... */
uint8_t SPI_SendByte(uint8_t data);
void    nRF24_TransmitSync(uint8_t* dataout,uint8_t len);
void    nRF24_TransferSync(uint8_t* dataout,uint8_t* datain,uint8_t len);
void    nRF24_ConfigRegister(uint8_t reg, uint8_t value);
void    nRF24_ReadRegister(uint8_t reg, uint8_t* value, uint8_t len);
void    nRF24_WriteRegister(uint8_t reg, uint8_t* value, uint8_t len);

/* Private functions ---------------------------------------------------------*/

#endif
