/*
 * nhd.h
 *
 *  Created on: Sep 18, 2019
 *      Author: Shayne Rossum
 */
#include <string.h>
#include <Arduino.h>
using namespace std;



class DeBug
{
    private:
    bool debugging = false;
    
       
    public:
    HardwareSerial *serial_obj;
    char debug_msg[80];
    void debug(char *msg);
    void debug(unsigned char msg);
    void begin_s();
};

class SPI
{
    friend class SPINano;
    private:
    TCS()
    {
        digitalWrite(CS, !digitalRead(CS));
    }
    
    public:
    unsigned char CS = 0xFF;
    unsigned char MOSI = 0xFF;
    unsigned char MISO = 0xFF;
    unsigned char SCLK = 0xFF;
    DeBug dbg;
    void dbg_set_addr(HardwareSerial &serial_add);
    void dbg_begin();
    void debug(char *);
    
    void init(int cable_select, int master_out_slave_in, int master_in_slave_out, int serial_clock);
    void Nano();
    void Uno();
    void Duemilanove();
    void Mega();
    void One_Oh_One();

    void write(unsigned char msg);
    void write(unsigned char *msg, unsigned char total_bytes);
    void write(unsigned char start, unsigned char *msg, unsigned char total_bytes); // total_bytes excludes start
};

class SPINano : SPI // does not work...
{
    
    SPINano();
};

class NHD
{
    
    public:
    int ground_control_pin = -1; //extra control
    int voltage_control_pin = -1; //extra control
    int regulated_voltage_pin = -1; //extra control
    int reset_pin = -1; //only needs to be set if using reset should be tied high if not
    unsigned char command_buffer_length = 0;
    int interface_pins[3] = {-1, -1, -1}; //extra control
    DeBug dbg;
    void dbg_set_addr(HardwareSerial &serial_add);
    void dbg_begin();
    void debug(char *);
};

class NHDSerial : public NHD
{

    private:
    //control
    unsigned char total_columns = 20;
    unsigned char total_rows = 4;
    unsigned char char_rom = 0; // <=2
    unsigned char custom_char_addr = 0; // <= 4;

    //Start Byte
    const unsigned char sync_bit_string_h = 0xF8;
    const unsigned char data_command_h = 0x02; //on for command
    const unsigned char read_write_h = 0x04; //on for read off for write

    //cmd byte
    const unsigned char clear_display_h = 0x01; //should be used with command bit
    const unsigned char return_home_h = 0x02;
    const unsigned char entry_mode_set_h = 0x04;
    const unsigned char cursor_moves_right_h = 0x02;
    const unsigned char shift_enable_h = 0x01;
    const unsigned char data_shift_100_h = 0x01;
    const unsigned char data_shift_common_h = 0x02;
    const unsigned char display_control_h = 0x08;
    const unsigned char display_on_h = 0x04;
    const unsigned char cursor_on_h = 0x02;
    const unsigned char cursor_blink_h = 0x01;
    const unsigned char extended_function_set_h = 0x08;
    const unsigned char font_width_h = 0x04; //font width FW control
    const unsigned char bw_inversion_h = 0x02; //black white inversion at the cursor
    const unsigned char nw_bit_h = 0x01; //4 line mode
    const unsigned char shift_h = 0x10;
    const unsigned char shift_display_h = 0x08; //S/C display shift otherwise cursor shift
    const unsigned char shift_right_h = 0x04; //R/L shift to the right otherwise left
    const unsigned char double_height_h = 0x10;
    const unsigned char ud1_h = 0x08;
    const unsigned char ud2_h = 0x04;
    const unsigned char dh_four_four_h = 0x01; // shift per line otherwise smooth dot scroll
    const unsigned char dh_two_h = 0x04;
    const unsigned char shift_scroll_h = 0x10;
    const unsigned char line_1_h = 0x01;
    const unsigned char line_2_h = 0x02;
    const unsigned char line_3_h = 0x04;
    const unsigned char line_4_h = 0x08;
    const unsigned char function_set_h = 0x20;
    const unsigned char additional_line_h = 0x08;
    const unsigned char is_h = 0x01;
    const unsigned char function_set_ext_h = 0x20;
    const unsigned char data_blink_h = 0x04;
    const unsigned char re_h = 0x02;
    const unsigned char reverse_h = 0x01;
    const unsigned char set_cgram_h = 0x40;
    const unsigned char cgram_data_mask = 0x3F;
    const unsigned char set_ddram_h = 0x80;
    const unsigned char ddram_data_mask = 0x7F;
    const unsigned char scroll_quanity_h = 0x80;
    const unsigned char scroll_data_mask = 0x3F;
    const unsigned char first_four_mask = 0xF0;
    const unsigned char oled_characterization_h = 0x78;
    const unsigned char sd_h = 0x01;
    const unsigned char regulator_control_h = 0x71;
    const unsigned char regulator_on_h = 0x5C;
    const unsigned char regulator_off_h =0x00;
    const unsigned char character_h = 0x72;

    bool data_command_f = false;
    bool read_write_f = false; //true is read
    bool cursor_moves_right_f = true;
    bool shift_enabled_f = false;
    bool data_shift_100_f = false;
    bool data_shift_common_f = true;
    bool display_on_f = true;
    bool cursor_on_f = false;
    bool cursor_blink_f = false;
    bool font_width_f = false;
    bool bw_inversion_f = false;
    bool nw_bit_f = true;
    bool shift_display_f = false;
    bool shift_right_f = false;
    bool double_height_f = false;
    bool dh_two_f = false;
    bool ud1_f = false;
    bool ud2_f = false;
    bool dh_four_f = false;
    bool shift_scroll_f = false;
    bool line_1_f = false;
    bool line_2_f = false;
    bool line_3_f = false;
    bool line_4_f = false;
    bool function_set_f = false;
    bool additional_line_f = true;
    bool is_f = false;
    bool function_set_ext_f = false;
    bool data_blink_f = false;
    bool re_f = false;
    bool reverse_f = false;
    bool scroll_quanity_f = false;
    bool sd_f = false;
    bool regulator_f = false; 

    bool IS_bit = false;
    bool RE_bit = false;
    bool SD_bit = false;

    void init()
    {
        if(total_columns <= 16)
        {
            font_width_f = true;
        }
        if(total_rows >= 4)
        {
            nw_bit_f = true;
            additional_line_f = true;
        }
        if(total_rows == 3)
        {
            nw_bit_f = true;
            additional_line_f = false;
        }
        if(total_rows == 2)
        {
            additional_line_f = true;
            nw_bit_f = false;
        }
        if(total_rows == 1)
        {
            additional_line_f = false;
            nw_bit_f = false;
        }
        dbg.debug("begin init()");
        
        function_set_ext(); //Turn on RE bit and N bit (additional line)0x28
        regulator(); //Turn on regulator for 5v operation 0x71 and 0x5C
        character_set();
        if(nw_bit_f) {extended_function();} //Turn on NW bit for 4 line mode
        entry_mode_set(); //Enable BDC bit to stop mirroring. data_shift_common_f
        function_set(); //Disable RE bit
        entry_mode_set();
        set_cursor(0, 0);
        display_mid(); //Turn on Display
        dbg.debug("End init()");
        
    }
    
    void character_set()
    {   
        pre_req_bit_handler(-1, 1, 0);
        double_byte_command(character_h, (char_rom << 2) | custom_char_addr);
    }
    
    void regulator()
    {
        dbg.debug("regulator_start");
        
        pre_req_bit_handler(-1, 1, 0);
        double_byte_command(regulator_control_h, regulator_f ? regulator_on_h : regulator_off_h);
        dbg.debug("regulator_complete");
        
    }

    void RE_change()
    {
        re_f ? function_set() : function_set_ext();
    }

    void IS_change()
    {
        is_f = !IS_bit;
        function_set();
    }

    void SD_change()
    {
        sd_f = !SD_bit;
        command_handler(oled_characterization_gen());
        SD_bit = sd_f;
    }

    void entry_mode_set_final(bool cursor_moves_right, bool shift)
    {
        cursor_moves_right_f = cursor_moves_right;
        shift_enabled_f = shift;
        command_handler(entry_mode_gen());
        if((cursor_on_f || cursor_blink_f) && bw_inversion_f) {}
    }

    void display_final(bool display, bool cursor, bool blink)
    {
        display_on_f = display;
        cursor_on_f = cursor;
        cursor_blink_f = blink;
        command_handler(display_gen());
        if((cursor_on_f || cursor_blink_f) && bw_inversion_f) {}
    }

    void extended_function_final(bool font_width, bool cursor_inversion,
                                 bool three_four_line)
    {
        if(cursor_inversion && (cursor_on_f || cursor_blink_f))
        {
            strcpy(dbg.debug_msg, "Cursor Inversion conflict");
        }
        font_width_f = font_width;
        bw_inversion_f = cursor_inversion;
        nw_bit_f = three_four_line;
        command_handler(extended_function_gen());
    }

    void function_set_final(bool add_line, bool double_height, bool IS_set)
    {
        additional_line_f = add_line;
        dh_two_f = double_height;
        is_f = IS_set;
        command_handler(function_set_gen());
        RE_bit = false;
        IS_bit = is_f;
    }

    void function_set_ext_final(bool add_line, bool char_blink, bool rev_font)
    {
        dbg.debug("function_set_ext_final_start");
        
        additional_line_f = add_line;
        data_blink_f = char_blink;
        reverse_f = rev_font;
        command_handler(function_set_ext_gen());
        RE_bit = true;
        dbg.debug("function_set_ext_final_complete");
        
    }

    unsigned char check_return(bool flag, unsigned char value)
    {
        if(flag) {return value;} else {return 0x00;}
    }

    void pre_req_bit_handler(char IS_req, char RE_req, char SD_req)
    {
        if(IS_req != -1) {if(IS_req != IS_bit) {IS_change();}}
        if(RE_req != -1) {if(RE_req != RE_bit) {RE_change();}}
        if(SD_req != -1) {if(SD_req != SD_bit) {SD_change();}}
    }

    unsigned char start_byte_gen()
    {
        unsigned char return_var_h = sync_bit_string_h;
        return_var_h |= check_return(data_command_f, data_command_h);
        return_var_h |= check_return(read_write_f, read_write_h);
        return return_var_h;
    }

    unsigned char entry_mode_gen()
    {
        unsigned char return_var_h = entry_mode_set_h;

        pre_req_bit_handler(-1, -1, 0);
        if(RE_bit)
        {
            return_var_h |= check_return(data_shift_100_f, data_shift_100_h);
            return_var_h |= check_return(data_shift_common_f, data_shift_common_h);
        }
        else
        {
            return_var_h |= check_return(cursor_moves_right_f, cursor_moves_right_h);
            return_var_h |= check_return(shift_enabled_f, shift_enable_h);
        }
        return return_var_h;
    }

    unsigned char display_gen()
    {
        unsigned char return_var_h = display_control_h;

        pre_req_bit_handler(-1, 0, 0);
        return_var_h |= check_return(display_on_f, display_on_h);
        return_var_h |= check_return(cursor_on_f, cursor_on_h);
        return_var_h |= check_return(cursor_blink_f, cursor_blink_h);
        return return_var_h;
    }

    unsigned char extended_function_gen()
    {
        unsigned char return_var_h = extended_function_set_h;

        pre_req_bit_handler(-1, 1, 0);
        return_var_h |= check_return(font_width_f, font_width_h);
        return_var_h |= check_return(bw_inversion_f, bw_inversion_h);
        return_var_h |= check_return(nw_bit_f, nw_bit_h);
        return return_var_h;
    }

    unsigned char shift_gen()
    {
        unsigned char return_var_h = shift_h;

        pre_req_bit_handler(0, 0, 0);
        return_var_h |= check_return(shift_display_f, shift_display_h);
        return_var_h |= check_return(shift_right_f, shift_right_h);
        return return_var_h;
    }

    unsigned char double_height_gen()
    {
        unsigned char return_var_h = double_height_h;

        pre_req_bit_handler(0, 1, 0);
        return_var_h |= check_return(ud2_f, ud2_h);
        return_var_h |= check_return(ud1_f, ud1_h);
        return_var_h |= check_return(dh_four_f, dh_four_four_h);
        return return_var_h;
    }

    unsigned char shift_scroll_gen()
    {
        unsigned char return_var_h = shift_scroll_h;

        pre_req_bit_handler(1, 1, 0);
        return_var_h |= check_return(line_1_f, line_1_h);
        return_var_h |= check_return(line_2_f, line_2_h);
        return_var_h |= check_return(line_3_f, line_3_h);
        return_var_h |= check_return(line_4_f, line_4_h);
        return return_var_h;
    }

    unsigned char function_set_gen()
    {
        unsigned char return_var_h = function_set_h;

        pre_req_bit_handler(-1, -1, 0);
        return_var_h |= check_return(additional_line_f, additional_line_h);
        return_var_h |= check_return(dh_two_f, dh_two_h);
        return_var_h |= check_return(is_f, is_h);
        re_f = false;
        return return_var_h;

    }

    unsigned char function_set_ext_gen()
    {
        unsigned char return_var_h = function_set_ext_h;
        re_f = true;

        pre_req_bit_handler(-1, -1, 0);
        return_var_h |= check_return(additional_line_f, additional_line_h);
        return_var_h |= check_return(data_blink_f, data_blink_h);
        return_var_h |= check_return(re_f, re_h);
        return_var_h |= check_return(reverse_f, reverse_h);
        return return_var_h;
    }

    unsigned char ram_address_gen(unsigned char column, unsigned char row)
    {
        unsigned char return_var_h = 0x00;
        if(total_rows == 4)
        {
            return_var_h = row * 32;
            return_var_h += column;
        }
        return return_var_h;
    }

    unsigned char set_cgram_gen(unsigned char to_set = 0x00)
    {
        unsigned char return_var_h = set_cgram_h;

        pre_req_bit_handler(0, 0, 0);
        to_set &= cgram_data_mask;
        return_var_h |= to_set;
        return return_var_h;
    }



    unsigned char set_ddram_gen(unsigned char to_set = 0x00)
    {
        unsigned char return_var_h = set_ddram_h;

        pre_req_bit_handler(-1, 0, 0);
        to_set &= ddram_data_mask;
        return_var_h |= to_set;
        return return_var_h;
    }

    unsigned char set_scroll_quanity_gen(unsigned char scroll_quanity = 0)
    {
        unsigned char return_var_h = scroll_quanity_h;

        pre_req_bit_handler(-1, 1, 0);
        scroll_quanity &= scroll_data_mask;
        return_var_h |= scroll_quanity;
        return return_var_h;

    }

    unsigned char read_status()
    {
        pre_req_bit_handler(-1, -1, 0);
        return 0x00; //place holder
    }
    


    void write_data(char *data)
    {
        dbg.debug("write_data_start");
        
        unsigned char start_byte;
        unsigned char the_byte = 0;
        int count = 0;
        int count_e = 0;
        int length = strlen(data);
        char *expanded_data = malloc(sizeof(char)*length*2);
        while(count < length)
        {
            the_byte = reverse_byte(data[count++]);
            expanded_data[count_e++] = the_byte&first_four_mask;
            expanded_data[count_e++] = (the_byte<<4)&first_four_mask;
        }
        pre_req_bit_handler(-1, -1, 0);
        data_command_f = true;
        start_byte = start_byte_gen();
        SPI_obj.write(start_byte, expanded_data, length*2);
        free(expanded_data);
        data_command_f = false;
        dbg.debug("write_data_complete");
        
    }
    
    void write_data(unsigned char length, unsigned char *data)
    {
        unsigned char start_byte;
        unsigned char the_byte;
        unsigned char count = 0;
        unsigned char count_e = 0;
        char *expanded_data = malloc(sizeof(char)*length*2);
        while(count < length)
        {
            the_byte = reverse_byte(data[count++]);
            expanded_data[count_e++] = the_byte&first_four_mask;
            expanded_data[count_e++] = (the_byte<<4)&first_four_mask;
        }
        pre_req_bit_handler(-1, -1, 0);
        data_command_f = true;
        start_byte = start_byte_gen();
        SPI_obj.write(start_byte, expanded_data, length*2);
        free(expanded_data);
        data_command_f = false;
        
    }

    unsigned char read_data() //incomplete / rewrite
    {
        unsigned char the_start_byte;

        pre_req_bit_handler(-1, -1, 0);
        read_write_f = true;
        the_start_byte = start_byte_gen();

        read_write_f = false;
        return 0x00; // placeholder
    }

    unsigned char oled_characterization_gen()
    {
        unsigned char return_var_h = oled_characterization_h;

        pre_req_bit_handler(-1, 1, -1);
        return_var_h |= check_return(sd_f, sd_h);
        return return_var_h;
    }

    void no_dc_no_rw()
    {
        data_command_f = false;
        read_write_f = false;
    }
    
    unsigned char reverse_byte(unsigned char the_byte)
    {
        dbg.debug("ForwardByte");
        dbg.debug(the_byte);
        the_byte = ((the_byte & 0x1)  << 7) | ((the_byte & 0x2)  << 5) | 
                   ((the_byte & 0x4)  << 3) | ((the_byte & 0x8)  << 1) |
                   ((the_byte & 0x10) >> 1) | ((the_byte & 0x20) >> 3) |
                   ((the_byte & 0x40) >> 5) | ((the_byte & 0x80) >> 7);
        dbg.debug("Reverse_byte");
        dbg.debug(the_byte);
        return the_byte;
    }

    void command_handler(unsigned char cmd_byte)
    {
        dbg.debug("command_handler_start");
        no_dc_no_rw();
        cmd_byte = reverse_byte(cmd_byte);
        unsigned char a_byte0 = cmd_byte&first_four_mask;
        unsigned char a_byte1 = (cmd_byte<<4)&first_four_mask;
        unsigned char bytes_to_write[3] = {start_byte_gen(), a_byte0, a_byte1};
        SPI_obj.write(bytes_to_write, 3);
        dbg.debug("command_handler_complete");
        
    }
    
    double_byte_command(unsigned char cmd_byte0, unsigned char cmd_byte1)
    {
        dbg.debug("double_byte_command_start");
        no_dc_no_rw();
        cmd_byte0 = reverse_byte(cmd_byte0);
        cmd_byte1 = reverse_byte(cmd_byte1);
        unsigned char start_byte = start_byte_gen();
        unsigned char a_byte0 = cmd_byte0&first_four_mask;
        unsigned char a_byte1 = (cmd_byte0<<4)&first_four_mask;
        unsigned char a_byte2 = cmd_byte1&first_four_mask;
        unsigned char a_byte3 = (cmd_byte1<<4)&first_four_mask;
        unsigned char bytes_to_write[3] = {start_byte, a_byte0, a_byte1};
        SPI_obj.write(bytes_to_write, 3);
        data_command_f = true;
        start_byte = start_byte_gen();
        bytes_to_write[0] = start_byte;
        bytes_to_write[1] = a_byte2;
        bytes_to_write[2] = a_byte3;
        SPI_obj.write(bytes_to_write, 3);
        data_command_f = false;
        dbg.debug("double_byte_command_complete");
    }

    public:
    
    SPI SPI_obj;
    
    void dbg_begin();
    void dbg_set_addr(HardwareSerial &serial_add);
    
    void begin();
    void begin(char *pin_conf);
    void begin(unsigned char cols, unsigned char rows);
    
    void regulate();
    void regulate_off();
    void regulateOff();
    
    void print(char *the_char_array);
    void clear();
    void home();
    void set_cursor(int column, int row);
    void setCursor(int column, int row);
    void cursor();
    void no_cursor();
    void noCursor();
    void blink();
    void no_blink();
    void noBlink();
    void display();
    void no_display();
    void noDisplay();
    void autoscroll();
    void no_autoscroll();
    void noAutoscroll();
    void scroll_display_left();
    void scrollDisplayLeft();
    void scroll_display_right();
    void scrollDisplayRight();
    void left_to_right();
    void leftToRight();
    void right_to_left();
    void rightToLeft();
    void create_char(unsigned char an_address, unsigned char *the_byte_array);
    void createChar(unsigned char an_address, unsigned char *the_byte_array);
    void create_chars(unsigned char an_address, unsigned char *the_byte_array, unsigned char *the_byte_array1, unsigned char *the_byte_array2);
    
    void print_auto(char *the_char_array);
    void print_auto(char *the_char_array0, char *the_char_array1);
    void print_auto(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2);
    void print_auto(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2, char *the_char_array3);
    void print_auto_h(char *the_char_array);
    void print_auto_h(char *the_char_array0, char *the_char_array1);
    void print_auto_h(char *the_char_array0, char *the_char_array1, 
                      char *the_char_array2);
    void print_auto_h(char *the_char_array0, char *the_char_array1, 
                      char *the_char_array2, char *the_char_array3);
    void printAuto(char *the_char_array);
    void printAuto(char *the_char_array0, char *the_char_array1);
    void printAuto(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2);
    void printAuto(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2, char *the_char_array3);
    void printAutoH(char *the_char_array);
    void printAutoH(char *the_char_array0, char *the_char_array1);
    void printAutoH(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2);
    void printAutoH(char *the_char_array0, char *the_char_array1, 
                    char *the_char_array2, char *the_char_array3);
    
    void entry_mode_set();
    void entry_mode_set(bool cursor_moves_right);
    void entry_mode_set(bool cursor_moves_right, bool shift);
    void display_mid();
    void display_mid(bool display_on);
    void display_mid(bool display_on, bool cursor_on);
    void display_mid(bool display_on, bool cursor_on, bool cursor_blink);
    void extended_function();
    void extended_function(bool font_width);
    void extended_function(bool font_width, bool cursor_inversion);
    void extended_function(bool font_width, bool cursor_inversion,
                           bool three_four_line);

    void function_set();
    void function_set(bool add_line);
    void function_set(bool add_line, bool double_height);
    void function_set(bool add_line, bool double_height, bool IS_set);
    void function_set_ext();
    void function_set_ext(bool add_line);
    void function_set_ext(bool add_line, bool char_blink);
    void function_set_ext(bool add_line, bool char_blink, bool rev_font);


    NHDSerial();
    NHDSerial(char *board);
    NHDSerial(int csp, int mosi, int miso, int sck);

};

