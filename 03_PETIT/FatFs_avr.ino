#include "diskio.h"
#include "pff.h"
#include <SPI.h>

#define chipSelectPin_ 4
/** SPI Master Out Slave In pin */
#define SPI_MOSI_PIN 11
/** SPI Master In Slave Out pin */
#define SPI_MISO_PIN 12
#define SPI_SCK_PIN 13

FATFS fs;
static SPISettings settings;

void setup()
{
    UINT w;
    UINT r;
    uint8_t rd_byte[128];
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Hack arduino fat file system.");
    Serial.print("result of init: ");
    Serial.println(disk_initialize());
    Serial.print("result of mount: ");
    Serial.println(pf_mount(&fs));
    Serial.print("result of open a file: ");
    Serial.println(pf_open("TEST.TXT"));
    Serial.println("test for file read...");


    pf_read(rd_byte, 128, &r);
    for (int i = 0; i < 128; i++)
    {
        Serial.print(char(rd_byte[i]));
    }

    Serial.println();
    Serial.println("test write function...");
    Serial.print("result of write: ");
    Serial.println(pf_write("hello world!\r\n", 14, &w));
    pf_write(0, 0, &w);
    Serial.println("test read function again");

    pf_open("TEST.TXT");
    pf_read(rd_byte, 14, &r);
    for (int i = 0; i < 14; i++)
    {
        Serial.print(char(rd_byte[i]));
    }
    Serial.println("test finished.");
}

void loop()
{
    // put your main code here, to run repeatedly:
}

void CS_LOW(void)
{
    digitalWrite(chipSelectPin_, LOW);
}
void CS_HIGH(void)
{
    digitalWrite(chipSelectPin_, HIGH);
    SPI.endTransaction();
}

uint8_t IS_CS_LOW(void)
{
    digitalRead(chipSelectPin_);
    SPI.beginTransaction(settings);
}

void xmit(char c)
{
    Serial.print(c);
}

void dly_100us(void)
{
    delayMicroseconds(100);
}

void init_spi(void)
{
    pinMode(chipSelectPin_, OUTPUT);
    pinMode(SPI_MISO_PIN, INPUT);
    pinMode(SPI_MOSI_PIN, OUTPUT);
    pinMode(SPI_SCK_PIN, OUTPUT);
    SPI.begin();
    settings = SPISettings(250000, MSBFIRST, SPI_MODE0);
}

void xmit_spi(BYTE d)
{
    SPI.transfer(d);
}

BYTE rcv_spi(void)
{
    return (BYTE)SPI.transfer(0xFF);
}
