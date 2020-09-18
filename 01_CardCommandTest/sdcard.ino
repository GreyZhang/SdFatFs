#include "sdcard.h"

uint8_t sd_card_lld_init(void);
uint8_t sd_card_command_test(void);

void setup()
{
    // put your setup code here, to run once:
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("test for SD card");
    if(!sd_card_command_test())
    {
        Serial.println("SD card init error!");
    }
    else
    {
        Serial.println("SD card is ok so far!");
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}
