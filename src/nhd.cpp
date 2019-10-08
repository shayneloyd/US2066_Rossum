/*
 * nhd.cpp
 *
 *  Created on: Sep 18, 2019
 *      Author: Shayne Rossum
 */

#include <nhd.h>

// ****************************************** DeBug ******************************************************************
void DeBug::debug(char *msg) {if(debugging) {serial_obj->println(msg);}}

void DeBug::debug(unsigned char msg)
{
    if(debugging)
    {
        serial_obj->print(msg, DEC);
        serial_obj->print("\t"); 
        serial_obj->print(msg, HEX);
        serial_obj->print("\t"); 
        serial_obj->println(msg, BIN);
    }
}

void DeBug::begin_s()
{
    serial_obj->begin(9600);
    serial_obj->println("Started");
    debugging = true;
}


// ****************************************** NHD ********************************************************************
void NHD::dbg_set_addr(HardwareSerial &serial_add)
{
    dbg.serial_obj = &serial_add; 
}

void NHD::dbg_begin()
{
    dbg.begin_s();
}

void NHD::debug(char *msg)
{
    dbg.debug(msg);
}


// **************************************** SPI **********************************************************************
void SPI::dbg_set_addr(HardwareSerial &serial_add)
{
    dbg.serial_obj = &serial_add;    
}

void SPI::dbg_begin()
{
    dbg.begin_s();
}

void SPI::debug(char *msg)
{
    dbg.debug(msg);
}

void SPI::init(int cable_select, int master_out_slave_in, int master_in_slave_out, int serial_clock)
{
    dbg.debug("spi_init");
    CS = cable_select;
    MOSI = master_out_slave_in;
    MISO = master_in_slave_out;
    SCLK = serial_clock;
    pinMode(CS, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(SCLK, OUTPUT);
    digitalWrite(SCLK, HIGH);
    digitalWrite(CS, HIGH);
}

void SPI::Nano()
{
    dbg.debug("nano_init");
    init(10, 11, 12, 13);
}

void SPI::Uno()
{
    dbg.debug("uno_init");
    init(10, 11, 12, 13);
}

void SPI::Duemilanove()
{
    dbg.debug("duemilanove_init");
    init(10, 11, 12, 13);
}

void SPI::Mega()
{
    dbg.debug("mega_init");
    init(53, 51, 50, 52);
}


void SPI::One_Oh_One()
{
    dbg.debug("101_init");
    init(10, 11, 12, 13);
}


void SPI::write(unsigned char msg)
{
    dbg.debug("spi_write_1var");
    unsigned char bit_count = 0x00;
    unsigned char bit_mask = 0x80;
    dbg.debug(msg);
    TCS();
    for(bit_count = 0x00; bit_count < 8; bit_count++)
    {
        digitalWrite(SCLK, LOW);
        //if(msg&bit_mask) {dbg.debug("True");} else {dbg.debug("False");}
        digitalWrite(MOSI, msg&bit_mask);
        digitalWrite(SCLK, HIGH);
        bit_mask = bit_mask>>1;
    }
    TCS();
}

void SPI::write(unsigned char *msg, unsigned char total_bytes)
{
    dbg.debug("spi_write_2var");
    int count = 0;
    unsigned char bit_count = 0x00;
    unsigned char bit_mask = 0x80;
    TCS();
    while(count < total_bytes)
    {
        dbg.debug(msg[count]);
        
        for(bit_count = 0x00; bit_count < 8; bit_count++)
        {
            digitalWrite(SCLK, LOW);
            //dbg.debug(msg[count]); dbg.debug(bit_mask);
            //if(msg[count]&bit_mask) {dbg.debug("True");} else {dbg.debug("False");}
            digitalWrite(MOSI, msg[count]&bit_mask);
            digitalWrite(SCLK, HIGH);
            bit_mask = bit_mask>>1;
        }
        bit_mask = 0x80;
        count++;
    }
    TCS();
}

void SPI::write(unsigned char start, unsigned char *msg, unsigned char total_bytes) // total_bytes excludes start
{
    dbg.debug("spi_write_3var");
    int count = 0;
    unsigned char bit_count = 0x00;
    unsigned char bit_mask = 0x80;
    dbg.debug(start);
    TCS();
    for(bit_count = 0x00; bit_count < 8; bit_count++)
    {
        digitalWrite(SCLK, LOW);
        //if(start&bit_mask) {dbg.debug("True");} else {dbg.debug("False");}
        digitalWrite(MOSI, start&bit_mask);
        digitalWrite(SCLK, HIGH);
        bit_mask = bit_mask>>1;
    }
    bit_mask=0x80;
    while(count < total_bytes)
    {
        dbg.debug(msg[count]);
        for(bit_count = 0x00; bit_count < 8; bit_count++)
        {
            digitalWrite(SCLK, LOW);
            //if(msg[count]&bit_mask) {dbg.debug("True");} else {dbg.debug("False");}
            digitalWrite(MOSI, msg[count]&bit_mask);
            digitalWrite(SCLK, HIGH);
            bit_mask = bit_mask>>1;
        }
        bit_mask = 0x80;
        count++;
    }
    TCS();
}


// *************************************** SPINano *******************************************************************
SPINano::SPINano() // Does not work!
{
    init(10, 11, 12, 13);
}

// * ************************************ NHDSerial ******************************************************************
void NHDSerial::dbg_set_addr(HardwareSerial &serial_add)
{
    dbg.serial_obj = &serial_add;
    SPI_obj.dbg.serial_obj= &serial_add;   
}

void NHDSerial::dbg_begin()
{
    dbg.begin_s();
    SPI_obj.dbg.begin_s();
}


void NHDSerial::begin()
{
    dbg.debug("begin without pins or cols or rows");
    init();
}

void NHDSerial::begin(char *board)
{
    if(strcmp(board, "Nano") == 0 || strcmp(board, "nano") == 0) {SPI_obj.Nano();}
    else {if(strcmp(board, "Uno") == 0 || strcmp(board, "uno") == 0) {SPI_obj.Uno();}
    else {if(strcmp(board, "Duemilanove") == 0 || strcmp(board, "duemilanove") == 0) {SPI_obj.Duemilanove();}
    else {if(strcmp(board, "Mega") == 0 || strcmp(board, "mega") == 0) {SPI_obj.Mega();}
    else {if(strcmp(board, "101") == 0) {SPI_obj.One_Oh_One();}}}}}
    init();
}

void NHDSerial::begin(unsigned char cols, unsigned char rows)
{
    total_columns = cols;
    total_rows = rows;
    init();
}

void NHDSerial::regulate()
{
    regulator_f = true;
    regulator();
}

void NHDSerial::regulate_off()
{
    regulator_f = false;
    regulator();
} void NHDSerial::regulateOff() {regulate_off();}

void NHDSerial::print(char *the_char_array)
{
    write_data(the_char_array);
}

void NHDSerial::clear()
{
    command_handler(clear_display_h);
}

void NHDSerial::home()
{
    command_handler(return_home_h);
}

void NHDSerial::set_cursor(int column, int row)
{
    command_handler(set_ddram_gen(ram_address_gen(column, row)));
} void NHDSerial::setCursor(int column, int row){set_cursor(column, row);}

void NHDSerial::cursor()
{
    cursor_on_f = true;
    command_handler(display_gen());
}

void NHDSerial::no_cursor()
{
    cursor_on_f = false;
    command_handler(display_gen());
} void NHDSerial::noCursor() {no_cursor();}

void NHDSerial::blink()
{
    cursor_blink_f = true;
    command_handler(display_gen());
}

void NHDSerial::no_blink()
{
    cursor_blink_f = false;
    command_handler(display_gen());
} void NHDSerial::noBlink() {no_blink();}

void NHDSerial::display()
{
    display_mid(true);
}

void NHDSerial::no_display()
{
    display_mid(false);
} void NHDSerial::noDisplay() {no_display();}


void NHDSerial::scroll_display_left()
{
    shift_display_f = true;
    shift_right_f = false;
    command_handler(shift_gen());
} void NHDSerial::scrollDisplayLeft() {scroll_display_left();}

void NHDSerial::scroll_display_right()
{   
    shift_display_f = true;
    shift_right_f = true;
    command_handler(shift_gen());
} void NHDSerial::scrollDisplayRight() {scroll_display_right();}

void NHDSerial::autoscroll()
{
    shift_enabled_f = true;
    line_1_f = true;
    line_2_f = true;
    line_3_f = true;
    line_4_f = true;
    command_handler(shift_scroll_gen());
    function_set();
    entry_mode_set();
}

void NHDSerial::no_autoscroll()
{
    shift_enabled_f = false;
    command_handler(shift_scroll_gen());
    function_set();
    entry_mode_set();
} void NHDSerial::noAutoscroll(){no_autoscroll();}

void NHDSerial::left_to_right()
{
    function_set();
    entry_mode_set(true);
} void NHDSerial::leftToRight() {left_to_right();}

void NHDSerial::right_to_left()
{
    function_set();
    entry_mode_set(false);
} void NHDSerial::rightToLeft() {right_to_left();}

void NHDSerial::create_char(unsigned char an_address, unsigned char *the_byte_array)
{
    dbg.debug("create_char_start");
    bool current_move = cursor_moves_right_f;
    bool current_shift = shift_enabled_f;
    entry_mode_set(true, false);
    dbg.debug(set_cgram_gen(an_address));
    command_handler(set_cgram_gen(an_address));
    write_data(8, the_byte_array);
    entry_mode_set(current_move, current_shift);
    dbg.debug("create_char_end");
} void NHDSerial::createChar(unsigned char an_address, unsigned char *the_byte_array) 
                            {create_char(an_address, the_byte_array);}
                            
void NHDSerial::create_chars(unsigned char an_address, unsigned char *the_byte_array, unsigned char *the_byte_array1, unsigned char *the_byte_array2)
{
    dbg.debug("create_char_start");
    bool current_move = cursor_moves_right_f;
    bool current_shift = shift_enabled_f;
    entry_mode_set(true, false);
    dbg.debug(set_cgram_gen());
    command_handler(set_cgram_gen());
    unsigned char double_chars[16];
    for(unsigned char x = 0; x < 24; x++)
    {
        if(x<8) {double_chars[x] = the_byte_array[x];}
        else if(x<16) {double_chars[x] = the_byte_array1[x-8];}
        else {double_chars[x] = the_byte_array2[x-16];}    
    }
    write_data(24, double_chars);
    entry_mode_set(current_move, current_shift);
    dbg.debug("create_char_end");
}

/*
void scroll_display()
{
    line_1_f = true;
    line_2_f = true;
    line_3_f = true;
    line_4_f = true;
    command_handler(shift_scroll_gen());
    command_handler(set_scroll_quanity_gen(1));
}

void no_scroll_display()
{
    line_1_f = false;
    line_2_f = false;
    line_3_f = false;
    line_4_f = false;
    command_handler(shift_scroll_gen());
}
*/

void NHDSerial::print_auto(char *the_char_array)
{
    clear();
    write_data(the_char_array);
}


void NHDSerial::print_auto(char *the_char_array0, char *the_char_array1)
{
    clear();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
}

void NHDSerial::print_auto(char *the_char_array0, char *the_char_array1, char *the_char_array2)
{
    clear();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
    set_cursor(0,2);
    write_data(the_char_array2);
}

void NHDSerial::print_auto(char *the_char_array0, char *the_char_array1, char *the_char_array2, char *the_char_array3)
{
    clear();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
    set_cursor(0,2);
    write_data(the_char_array2);
    set_cursor(0,3);
    write_data(the_char_array3);
}

void NHDSerial::print_auto_h(char *the_char_array)
{
    home();
    write_data(the_char_array);
}

void NHDSerial::print_auto_h(char *the_char_array0, char *the_char_array1)
{
    home();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
}

void NHDSerial::print_auto_h(char *the_char_array0, char *the_char_array1, char *the_char_array2)
{
    home();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
    set_cursor(0,2);
    write_data(the_char_array2);
}

void NHDSerial::print_auto_h(char *the_char_array0, char *the_char_array1, char *the_char_array2, char *the_char_array3)
{
    home();
    write_data(the_char_array0);
    set_cursor(0,1);
    write_data(the_char_array1);
    set_cursor(0,2);
    write_data(the_char_array2);
    set_cursor(0,3);
    write_data(the_char_array3);
}

void NHDSerial::printAuto(char *the_char_array)
{
    print_auto(the_char_array);
}

void NHDSerial::printAuto(char *the_char_array0, char *the_char_array1)
{
    print_auto(the_char_array0, the_char_array1);
}

void NHDSerial::printAuto(char *the_char_array0, char *the_char_array1, 
                char *the_char_array2)
{
    print_auto(the_char_array0, the_char_array1, the_char_array2);
}

void NHDSerial::printAuto(char *the_char_array0, char *the_char_array1, 
                char *the_char_array2, char *the_char_array3)
{
    print_auto(the_char_array0, the_char_array1, the_char_array2, the_char_array3);
}

void NHDSerial::printAutoH(char *the_char_array)
{
    print_auto_h(the_char_array);
}

void NHDSerial::printAutoH(char *the_char_array0, char *the_char_array1)
{
    print_auto_h(the_char_array0, the_char_array1);
}

void NHDSerial::printAutoH(char *the_char_array0, char *the_char_array1, 
                char *the_char_array2)
{
    print_auto_h(the_char_array0, the_char_array1, the_char_array2);
}

void NHDSerial::printAutoH(char *the_char_array0, char *the_char_array1, 
                char *the_char_array2, char *the_char_array3)
{
    print_auto_h(the_char_array0, the_char_array1, the_char_array2, the_char_array3);
}

void NHDSerial::entry_mode_set()
{
    entry_mode_set_final(cursor_moves_right_f, shift_enabled_f);
}

void NHDSerial::entry_mode_set(bool cursor_moves_right)
{
    entry_mode_set_final(cursor_moves_right, shift_enabled_f);
}

void NHDSerial::entry_mode_set(bool cursor_moves_right, bool shift)
{
    entry_mode_set_final(cursor_moves_right, shift);
}

void NHDSerial::display_mid()
{
    display_final(display_on_f, cursor_on_f, cursor_blink_f);
}

void NHDSerial::display_mid(bool display_on)
{
    display_final(display_on, cursor_on_f, cursor_blink_f);
}

void NHDSerial::display_mid(bool display_on, bool cursor_on)
{
    display_final(display_on, cursor_on, cursor_blink_f);
}

void NHDSerial::display_mid(bool display_on, bool cursor_on, bool cursor_blink)
{
    display_final(display_on, cursor_on, cursor_blink);
}

void NHDSerial::extended_function()
{
    extended_function_final(font_width_f, bw_inversion_f, nw_bit_f);
}

void NHDSerial::extended_function(bool font_width)
{
    extended_function_final(font_width, bw_inversion_f, nw_bit_f);
}

void NHDSerial::extended_function(bool font_width, bool cursor_inversion)
{
    extended_function_final(font_width, cursor_inversion, nw_bit_f);
}

void NHDSerial::extended_function(bool font_width, bool cursor_inversion,
                       bool three_four_line)
{
    extended_function_final(font_width, cursor_inversion, three_four_line);
}

void NHDSerial::function_set()
{
    function_set_final(additional_line_f, dh_two_f, is_f);
}

void NHDSerial::function_set(bool add_line)
{
    function_set_final(add_line, dh_two_f, is_f);
}

void NHDSerial::function_set(bool add_line, bool double_height)
{
    function_set_final(add_line, double_height, is_f);
}

void NHDSerial::function_set(bool add_line, bool double_height, bool IS_set)
{
    function_set_final(add_line, double_height, IS_set);
}

void NHDSerial::function_set_ext()
{
    function_set_ext_final(additional_line_f, data_blink_f, reverse_f);
}

void NHDSerial::function_set_ext(bool add_line)
{
    function_set_ext_final(add_line, data_blink_f, reverse_f);
}

void NHDSerial::function_set_ext(bool add_line, bool char_blink)
{
    function_set_ext_final(add_line, char_blink, reverse_f);
}

void NHDSerial::function_set_ext(bool add_line, bool char_blink, bool rev_font)
{
    function_set_ext_final(add_line, char_blink, rev_font);
}

NHDSerial::NHDSerial()
{
}

NHDSerial::NHDSerial(char *board)
{
    if(strcmp(board, "Nano") == 0 || strcmp(board, "nano") == 0) {SPI_obj.Nano();}
    else {if(strcmp(board, "Uno") == 0 || strcmp(board, "uno") == 0) {SPI_obj.Uno();}
    else {if(strcmp(board, "Duemilanove") == 0 || strcmp(board, "duemilanove") == 0) {SPI_obj.Duemilanove();}
    else {if(strcmp(board, "Mega") == 0 || strcmp(board, "mega") == 0) {SPI_obj.Mega();}
    else {if(strcmp(board, "101") == 0) {SPI_obj.One_Oh_One();}}}}}
}


NHDSerial::NHDSerial(int csp, int mosi, int miso, int sck)
{
    SPI_obj.init(csp, mosi, miso, sck);
}











/*

class NHDParallel : public NHD
{
    public:
    int data_command_pin = -1; //must be set for parallel
    int read_write_pin = -1; //must be set for parallel
    int enable_signal_pin = -1; //must be set for parallel
    int cable_select_pin = -1; //must be set for parallel
    int low_order_pins[4] = {-1, -1, -1, -1}; //must be set for 8 bit
    int high_order_pins[4] = {-1, -1, -1, -1}; //must be set for 4 and 8 bit
};


class NHDI2c : public NHD
{
    public:
    int slave_address_select_pin = -1; //not sure if required for operation
    int clock_pin = -1; //must be set for i2c
    int input_pin = -1; //must be set for i2c
    int output_pin = -1; //must be set for i2c
};

*/

