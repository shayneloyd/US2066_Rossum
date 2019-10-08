// example 0420CW create char
// The 0 position character will not work without debugging enabled.
#include <nhd.h>

unsigned char count = 0;
char the_char2[] = {2,0};
char the_char1[] = {1,0};
char the_char0[] = {3,0};

NHDSerial lcd("Mega");
void setup()
{   
    lcd.begin(20, 4);

    unsigned char frownie[8] = {
    B00000,
    B01010,
    B00000,
    B00000,
    B01110,
    B10001,
    B00000,
    };
    
    unsigned char smiley[8] = {
    B00000,
    B01010,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000,
    };

    unsigned char heart[8] = {
    B00000,
    B00000,
    B11011,
    B11111,
    B01110,
    B00100,
    B00000,
    };
    
    
    lcd.create_char(8, smiley);
    lcd.create_char(16, heart);
    lcd.create_char(24, frownie);

    lcd.clear();
}

void loop()
{
    count = 0;
    while(count < 80)
    {
        lcd.print(the_char0);
        count++;
    }
    delay(500);
    lcd.clear();
    count = 0;
    while(count < 80)
    {
        lcd.print(the_char1);
        count++;
    }
    delay(500);
    lcd.clear();
    count = 0;
    while(count < 80)
    {
        lcd.print(the_char2);
        count++;
    }
    delay(500);
    lcd.clear();
    
}
