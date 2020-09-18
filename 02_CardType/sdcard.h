#ifndef SDCARD_H
#define SDCARD_H
#include "stdint.h"
#include "sdinfo.h"

//------------------------------------------------------------------------------
/** Protect block zero from write if nonzero */
#define SD_PROTECT_BLOCK_ZERO 1
/** init timeout ms */
#define SD_INIT_TIMEOUT 2000
/** erase timeout ms */
#define SD_ERASE_TIMEOUT 10000
/** read timeout ms */
#define SD_READ_TIMEOUT 300
/** write time out ms */
#define SD_WRITE_TIMEOUT 600
//------------------------------------------------------------------------------
// SD card errors
/** timeout error for command CMD0 */
#define SD_CARD_ERROR_CMD0  0X1
/** CMD8 was not accepted - not a valid SD card*/
#define SD_CARD_ERROR_CMD8  0X2
/** card returned an error response for CMD17 (read block) */
#define SD_CARD_ERROR_CMD17  0X3
/** card returned an error response for CMD24 (write block) */
#define SD_CARD_ERROR_CMD24  0X4
/**  WRITE_MULTIPLE_BLOCKS command failed */
#define SD_CARD_ERROR_CMD25  0X05
/** card returned an error response for CMD58 (read OCR) */
#define SD_CARD_ERROR_CMD58  0X06
/** SET_WR_BLK_ERASE_COUNT failed */
#define SD_CARD_ERROR_ACMD23  0X07
/** card's ACMD41 initialization process timeout */
#define SD_CARD_ERROR_ACMD41  0X08
/** card returned a bad CSR version field */
#define SD_CARD_ERROR_BAD_CSD  0X09
/** erase block group command failed */
#define SD_CARD_ERROR_ERASE  0X0A
/** card not capable of single block erase */
#define SD_CARD_ERROR_ERASE_SINGLE_BLOCK = 0X0B;
/** Erase sequence timed out */
#define SD_CARD_ERROR_ERASE_TIMEOUT  0X0C
/** card returned an error token instead of read data */
#define SD_CARD_ERROR_READ  0X0D
/** read CID or CSD failed */
#define SD_CARD_ERROR_READ_REG  0X0E
/** timeout while waiting for start of read data */
#define SD_CARD_ERROR_READ_TIMEOUT  0X0F
/** card did not accept STOP_TRAN_TOKEN */
#define SD_CARD_ERROR_STOP_TRAN  0X10
/** card returned an error token as a response to a write operation */
#define SD_CARD_ERROR_WRITE  0X11
/** attempt to write protected block zero */
#define SD_CARD_ERROR_WRITE_BLOCK_ZERO  0X12
/** card did not go ready for a multiple block write */
#define SD_CARD_ERROR_WRITE_MULTIPLE  0X13
/** card returned an error to a CMD13 status check after a write */
#define SD_CARD_ERROR_WRITE_PROGRAMMING  0X14
/** timeout occurred during write programming */
#define SD_CARD_ERROR_WRITE_TIMEOUT  0X15
/** incorrect rate selected */
#define SD_CARD_ERROR_SCK_RATE  0X16
//------------------------------------------------------------------------------
// card types
/** Standard capacity V1 SD card */
#define SD_CARD_TYPE_SD1  1
/** Standard capacity V2 SD card */
#define SD_CARD_TYPE_SD2  2
/** High Capacity SD card */
#define SD_CARD_TYPE_SDHC  3
//------------------------------------------------------------------------------

extern uint8_t sd_lld_inBlock_;
extern const uint8_t sd_lld_spi_cs_pin;
extern uint16_t sd_lld_offset_;
extern uint8_t sd_lld_status_;
extern uint8_t sd_lld_errorCode_;

void spi_just_Send(uint8_t b);
void sd_lld_chipSelectLow(void);
uint8_t sd_lld_cardCommand(uint8_t cmd, uint32_t arg);
void sd_lld_chipSelectHigh(void);
uint8_t spi_just_spiRec(void);
uint8_t sd_lld_waitNotBusy(unsigned int timeoutMillis);
void sd_lld_set_error(uint8_t code);
void sd_lld_set_card_type(uint8_t type);
uint8_t sd_lld_get_card_type(void);
uint8_t sd_lld_cardAcmd(uint8_t cmd, uint32_t arg);
void sd_lld_spi_init(void);

#endif
