#include "sdcard.h"

uint8_t sd_card_lld_init(void);

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
    if(!sd_card_lld_init())
    {
        Serial.println("SD card init error!");
    }
    else
    {
        Serial.println("SD card is ok so far!");
        switch(sd_lld_get_card_type())
        {
        case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
        case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("Unknown");
        }
    }
}

void loop()
{
    // put your main code here, to run repeatedly:
}
