/* 
 * File:   OBC2_main.c
 * Author: Tetsuya
 *
 * Created on 2016/02/12, 19:31
 */
#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "pic_clock.h"
#include "uart_serial.h"
#include "spi.h"
#include "i2c.h"
#include "tempADT.h"

//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

typedef struct
{
    unsigned char preint;
    unsigned char data_type;
    unsigned char payload_index_end;
    unsigned char payload;
    unsigned char data_end;
} packet_t;

typedef struct
{
    unsigned char power1[4];
    unsigned char power2[4];
    unsigned char temp[4];
    unsigned char OBC2:1;
    unsigned char COMMUC:1;
    unsigned char POWMCU:1;
} cw_t;

#define command 0x03
#define data    0x02
#define cw      0x01

#define Light   0x04
#define temp    0x05

int i=0;
//char data[32];

void interrupt isr(void);

int main(void){

    TRISD6 = 0;
    
    
    /*
    union{
        double d;
        char c[1];
    }u;
    */
    uart_init();
    //I2C_init();
    //spi_slave_init(SPI_ISR_ENABLE);
    put_string("Hello! TSP!\r\n");
    
    
    /*
    while(1){
        RD7 = 1;
        while((data[i] = spi_send_receive(0xFF)) != '\0' || i <= 32){
            i++;
        }

        if(data[1] == '\0'){
            u.c[0] = data[0];
            printf("%.2f\r\n", u.d);
            __delay_ms(10);
        }else{
            while(data[i] != '\0'){
                putch(data[i]);
                i++;
            }
            putch('\r');
            putch('\n');
            __delay_ms(10);
        }
        RD7 = 0;
        __delay_ms(100);
    }*/
    
    while(1)
    {
        put_string("Start\r\n");
        RD6 = 1;
        __delay_ms(100);
        /*
        i = 0;
        while((data[i] = spi_send_receive(0xFF)) != '\0' || i <= 32){
            i++;
        }
        */
        /*
        i = 0;
        while(data[i] != '\0' || i <= 32){
            putch(data[i]);
            i++;
        }
        */
       
        put_string("End\r\n");
        RD6 = 0;
        __delay_ms(100);
    }
    
    return 0;
}
/*
void interrupt isr(void)
{
    if(PIR1bits.SSPIF == 1)
    {
        put_string("Into SPI Interrupt\r\n");
        spi_interrupt();
        SSPBUF = 0xff;
        
        PIR1bits.SSPIF = 0;
    }
}
*/
