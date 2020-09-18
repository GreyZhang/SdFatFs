#include "sdcard.h"
#include <SPI.h>

uint8_t sd_lld_inBlock_ = 0U;
const uint8_t sd_lld_spi_cs_pin = 4;
static SPISettings settings;
uint16_t sd_lld_offset_ = 0U;
uint8_t sd_lld_status_;
uint8_t sd_lld_errorCode_;
uint8_t sd_lld_sd_type_;


void spi_just_Send(uint8_t b)
{
    SPI.transfer(b);
}

uint8_t spi_just_spiRec(void)
{
    return SPI.transfer(0xFF);
}


void sd_lld_chipSelectLow(void)
{
    digitalWrite(sd_lld_spi_cs_pin, LOW);
}

void sd_lld_chipSelectHigh(void)
{
    digitalWrite(sd_lld_spi_cs_pin, HIGH);
}

//------------------------------------------------------------------------------
/** Skip remaining data in a block when in partial block read mode. */
//从注释看，这个功能主要是部分模块读取需求的，可以不必等结束，直接主动结束。
void sd_lld_readEnd(void)
{
    //根据前面的注释分析，这个量应该是用于表征一个块还在处理中的量
    if (sd_lld_inBlock_)
    {
        // skip data and crc

        //    从这里看，其实这个结束不是直接命令终止，而是继续读取然后放弃。
        //    这也可以解释，为什么这个SPI的接收有一个完全没有接收返回值的接口.
        //    为什么是514而不是512？
        while (sd_lld_offset_++ < 514)
        {
            spi_just_spiRec();
        }
        sd_lld_chipSelectHigh();
        sd_lld_inBlock_ = 0;
    }
}

//------------------------------------------------------------------------------
// wait for card to go not busy
// 超时且没有返回0xFF则报错。或者描述为：在规定时间内返回了0xFF则返回true
uint8_t sd_lld_waitNotBusy(unsigned int timeoutMillis)
{
    unsigned int t0 = millis();
    unsigned int d;
    do
    {
        if (spi_just_spiRec() == 0XFF)
        {
            return true;
        }
        d = millis() - t0;
    } while (d < timeoutMillis);
    return false;
}

//------------------------------------------------------------------------------
// send command and return error code.  Return zero for OK
uint8_t sd_lld_cardCommand(uint8_t cmd, uint32_t arg)
{
    // end read if in partialBlockRead mode
    sd_lld_readEnd();

    // select card
    sd_lld_chipSelectLow();

    // wait up to 300 ms if busy
    // we can use function offered by OS like FreeRTOS here
    sd_lld_waitNotBusy(300);

    // send command
    spi_just_Send(cmd | 0x40);

    // send argument by intel format
    for (int8_t s = 24; s >= 0; s -= 8)
    {
        spi_just_Send(arg >> s);
    }

    // send CRC
    uint8_t crc = 0XFF;
    if (cmd == CMD0)
    {
        crc = 0X95; // correct crc for CMD0 with arg 0
    }
    if (cmd == CMD8)
    {
        crc = 0X87; // correct crc for CMD8 with arg 0X1AA
    }
    spi_just_Send(crc);

    // wait for response
    for (uint8_t i = 0; ((sd_lld_status_ = spi_just_spiRec()) & 0X80) && i != 0XFF; i++)
    {
        ;
    }
    return sd_lld_status_;
}

uint8_t sd_lld_cardAcmd(uint8_t cmd, uint32_t arg)
{
    sd_lld_cardCommand(CMD55, 0);
    return sd_lld_cardCommand(cmd, arg);
}

void sd_lld_set_error(uint8_t code)
{
    sd_lld_errorCode_ = code;
}

void sd_lld_set_card_type(uint8_t type)
{
    sd_lld_sd_type_ = type;
}

uint8_t sd_lld_get_card_type(void)
{
    return sd_lld_sd_type_;
}

void sd_lld_spi_init(void)
{
    pinMode(sd_lld_spi_cs_pin, OUTPUT);
    pinMode(10, OUTPUT);
    digitalWrite(sd_lld_spi_cs_pin, HIGH);
    SPI.begin();
    settings = SPISettings(250000, MSBFIRST, SPI_MODE0);
    SPI.beginTransaction(settings);
}

uint8_t sd_card_command_test(void)
{
    unsigned int t0 = millis();
    uint32_t arg;

    /* init SPI */
    sd_lld_spi_init();

    // send 10 bytes, this would generate 80 clocks. 74 at least according to the document.
    for (uint8_t i = 0; i < 10; i++)
    {
        spi_just_Send(0XFF);
    }
    SPI.endTransaction();
    // select SD card
    sd_lld_chipSelectLow();
    // command to go idle in SPI mode
    while ((sd_lld_status_ = sd_lld_cardCommand(CMD0, 0)) != R1_IDLE_STATE)
    {
        unsigned int d = millis() - t0;
        // timeout after 2000ms
        if (d > SD_INIT_TIMEOUT)
        {
            sd_lld_set_error(SD_CARD_ERROR_CMD0);
            goto fail;
        }
    }

    sd_lld_chipSelectHigh();
    return 1;
fail:
    sd_lld_chipSelectHigh();
    return 0;
}
