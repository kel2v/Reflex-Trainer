/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/rand.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"


// Official Raspberry Pi's code with modification

#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

#define DELAY_US 600


const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;
const int LCD_BACKLIGHT = 0x08;
const int LCD_ENABLE_BIT = 0x04;

static int addr = 0x27;


void i2c_write_byte(uint8_t val);
void lcd_toggle_enable(uint8_t val);
void lcd_init();
void lcd_clear(void);
void lcd_send_byte(uint8_t val, int mode);
void lcd_set_cursor(int line, int position);
void lcd_write_string_at(int line, int position, const char *s);




void i2c_write_byte(uint8_t val)
{
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val)
{
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void lcd_init()
{
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

void lcd_clear(void)
{
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

void lcd_send_byte(uint8_t val, int mode)
{
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_set_cursor(int line, int position)
{
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}





































// My code



/*
 *  gpio 0, 1               :   IRPS 0
 *  gpio 4, 5, gnd          :   IRPS 1
 *  gpio 8, 9, gnd          :   IRPS 2
 *  gpio 12, 13, gnd        :   IRPS 3
 *  gpio 14, 15, gnd        :   IRPS 4
 *  gpio 20, 21, gnd and Vcc:   DISPLAY-I2C Interface
 *  gpio 17                 :   Buzzer
 *  gpio 22                 :   Button
 */


#define IRPS0_LED   0
#define IRPS0_DATA  1
#define IRPS1_LED   4
#define IRPS1_DATA  5
#define IRPS2_LED   8
#define IRPS2_DATA  9
#define IRPS3_LED   12
#define IRPS3_DATA  13
#define IRPS4_LED   14
#define IRPS4_DATA  15

#define LCD_SDA     20
#define LCD_SCL     21

#define BUZZER      17

#define BUTTON      22


struct IRPS
{
    uint LED;
    uint data;
};

struct IRPS sensors[5] = {{IRPS0_LED, IRPS0_DATA}, {IRPS1_LED, IRPS1_DATA}, {IRPS2_LED, IRPS2_DATA}, {IRPS3_LED, IRPS3_DATA}, {IRPS4_LED, IRPS4_DATA}};



void lcd_write_string_at(int line, int position, const char *s);
int intToStr(int64_t integer, char *str);
void append(char *source, int srcLen, char * str, int len);
void removeEntry(int element, int *arr, int *count);
void resetGame();
void beep(int ms);
int64_t senseIRPS(struct IRPS *obj);
void startGame(int round);
void core1_main();




void lcd_write_string_at(int line, int position, const char *s)
{
    lcd_set_cursor(line, position);

    while (*s)
    {
        lcd_send_byte(*s++, LCD_CHARACTER);
    }
}


int intToStr(int64_t integer, char *str)
{
    int64_t dup = integer;
    int digitcount = 0;

    while(dup>0)
    {
        digitcount++;
        dup /= 10;
    }

    if(digitcount == 0) digitcount = 1;

    for(int i=0; i<digitcount; i++)
    {
        int dig = integer%10;
        integer /= 10;

        if(dig == 0) str[digitcount-1-i] = '0';
        else if(dig == 1) str[digitcount-1-i] = '1';
        else if(dig == 2) str[digitcount-1-i] = '2';
        else if(dig == 3) str[digitcount-1-i] = '3';
        else if(dig == 4) str[digitcount-1-i] = '4';
        else if(dig == 5) str[digitcount-1-i] = '5';
        else if(dig == 6) str[digitcount-1-i] = '6';
        else if(dig == 7) str[digitcount-1-i] = '7';
        else if(dig == 8) str[digitcount-1-i] = '8';
        else if(dig == 9) str[digitcount-1-i] = '9';
    }

    str[digitcount] = '\0';

    return digitcount;
}


void append(char *source, int srcLen, char * str, int len)
{
    for(uint i=0; i<len; i++) source[srcLen+i] = str[i];
    source[srcLen+len] = '\0';
}


void removeEntry(int element, int *arr, int *count)
{
    for(int i=0; i<*count; i++)
    {
        if(arr[i] == element)
        {
            for(int j=i+1; j< *count; i++, j++)
            {
                arr[i] = arr[j];
            }

            break;
        }
    }

    *count = *count - 1;
}


void resetGame()
{
    gpio_put(IRPS0_LED, 1);
    gpio_put(IRPS1_LED, 1);
    gpio_put(IRPS2_LED, 1);
    gpio_put(IRPS3_LED, 1);
    gpio_put(IRPS4_LED, 1);
}


void beep(int ms)
{
    while(ms--)
    {
        gpio_put(BUZZER, 1);
        sleep_us(500);
        gpio_put(BUZZER, 0);
        sleep_us(500);
    }
}


int64_t senseIRPS(struct IRPS *obj)
{
    gpio_put(obj->LED, 1);
    absolute_time_t start = get_absolute_time();
    while(gpio_get(obj->data)){};
    absolute_time_t end = get_absolute_time();
    gpio_put(obj->LED, 0);

    int64_t diff_ms = absolute_time_diff_us(start, end)/1000;

    return diff_ms;
}


void startGame(int round)
{
    gpio_put(IRPS0_LED, 0);
    gpio_put(IRPS1_LED, 0);
    gpio_put(IRPS2_LED, 0);
    gpio_put(IRPS3_LED, 0);
    gpio_put(IRPS4_LED, 0);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }


    int arr[10] = {0,0,1,1,2,2,3,3,4,4}, order[10], count = 10, val;
    order[0] = arr[get_rand_32()%count];
    removeEntry(order[0] ,arr, &count);
    for(int i=1; i<10; i++)
    {
        if(i == 7)
        {
            int a = arr[0], b = arr[1], c = arr[2];
            if(a == b)
            {
                order[i] = a;
                order[i+1] = c;
                order[i+2] = b;
                break;
            }
            else if(b == c)
            {
                order[i] = b;
                order[i+1] = a;
                order[i+2] = c;
                break;
            }
            else if(a == c)
            {
                order[i] = a;
                order[i+1] = b;
                order[i+2] = c;
                break;
            }
        }
        while((val = arr[get_rand_32()%count]) == order[i-1]);
        order[i] = val;
        removeEntry(val ,arr, &count);
    }
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }


    lcd_clear();
    lcd_write_string_at(0, 5, "Ready");
    beep(200);
    sleep_ms(1000);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    lcd_clear();
    lcd_write_string_at(0, 6, "Set");
    beep(200);
    sleep_ms(1000);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    lcd_clear();
    lcd_write_string_at(0, 6, "Go!");
    beep(1000);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    lcd_clear();
    lcd_write_string_at(0, 3, "Running...");
    sleep_ms(get_rand_32()%250);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }


    int64_t responseTimes[round];
    for(int i=0; i<round; i++)
    {
        responseTimes[i] = senseIRPS(&sensors[order[i]]);
        multicore_fifo_push_blocking(150);
        if(multicore_fifo_rvalid())
        {
            multicore_fifo_drain();
            return;
        }
    }


    multicore_fifo_push_blocking(1000);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }


    int64_t sum = 0, min = responseTimes[0], max = responseTimes[0];
    for(int i=0; i<round; i++)
    {
        sum += responseTimes[i];
        if( responseTimes[i] < min) min = responseTimes[i];
        if( responseTimes[i] > max) max = responseTimes[i];
    }
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }


    int len;

    char summation[17];
    len = intToStr(sum, summation);
    append(summation, len, " ms", 3);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    char minimum[17];
    len = intToStr(min, minimum);
    append(minimum, len, " ms", 3);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    char maximum[17];
    len = intToStr(max, maximum);
    append(maximum, len, " ms", 3);
    if(multicore_fifo_rvalid())
    {
        multicore_fifo_drain();
        return;
    }

    lcd_clear();
    lcd_write_string_at(0, 0, "sum: ");
    lcd_write_string_at(0, 5, summation);
    lcd_write_string_at(1, 0, minimum);
    lcd_write_string_at(1, 8, maximum);
    sleep_ms(10000);
}


void core1_main()
{
    while(1)
    {
        if(gpio_get(BUTTON))
        {
            if(multicore_fifo_wready())multicore_fifo_push_blocking(1);
            sleep_ms(500);
        }

        if(multicore_fifo_rvalid())
        {
            uint32_t ms = multicore_fifo_pop_blocking();
            multicore_fifo_drain();
            beep(ms);
        }
    }
}


int main()
{
    gpio_init(IRPS0_LED);
    gpio_init(IRPS1_LED);
    gpio_init(IRPS2_LED);
    gpio_init(IRPS3_LED);
    gpio_init(IRPS4_LED);
    gpio_init(IRPS0_DATA);
    gpio_init(IRPS1_DATA);
    gpio_init(IRPS2_DATA);
    gpio_init(IRPS3_DATA);
    gpio_init(IRPS4_DATA);
    gpio_init(LCD_SDA);
    gpio_init(LCD_SCL);
    gpio_init(BUTTON);
    gpio_init(BUZZER);

    gpio_set_dir(IRPS0_LED, GPIO_OUT);
    gpio_set_dir(IRPS1_LED, GPIO_OUT);
    gpio_set_dir(IRPS2_LED, GPIO_OUT);
    gpio_set_dir(IRPS3_LED, GPIO_OUT);
    gpio_set_dir(IRPS4_LED, GPIO_OUT);

    gpio_set_dir(IRPS0_DATA, GPIO_IN);
    gpio_set_dir(IRPS1_DATA, GPIO_IN);
    gpio_set_dir(IRPS2_DATA, GPIO_IN);
    gpio_set_dir(IRPS3_DATA, GPIO_IN);
    gpio_set_dir(IRPS4_DATA, GPIO_IN);

    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_down(BUTTON);

    gpio_set_dir(BUZZER, GPIO_OUT);


    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(LCD_SDA, GPIO_FUNC_I2C);
    gpio_set_function(LCD_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(LCD_SDA);
    gpio_pull_up(LCD_SCL);
    bi_decl(bi_2pins_with_func(LCD_SDA, LCD_SCL, GPIO_FUNC_I2C));
    lcd_init();


    multicore_launch_core1(core1_main);

    while(1)
    {
        resetGame();
        lcd_clear();
        lcd_write_string_at(0, 2, "Press button");
        lcd_write_string_at(1, 5, "to start");
        while(1)
        {
            if(multicore_fifo_rvalid())
            {
                multicore_fifo_drain();
                break;
            }
        };
        startGame(10);
    }
}
