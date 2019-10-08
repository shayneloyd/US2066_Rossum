// example 0420CW
#include <nhd.h>

const char line_1[] = " line_1 is 20 chars ";
const char line_2[] = " line_2 is 20 chars ";
const char line_3[] = " line_3 is 20 chars ";
const char line_4[] = " line_4 is 20 chars ";

NHDSerial lcd;
void setup()
{   
    lcd.begin("Mega");
}

void loop()
{
    lcd.print_auto(line_1, line_2, line_3, line_4);
    delay(1000);
}
