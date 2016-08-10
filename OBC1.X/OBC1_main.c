/* 
 * File:   OBC1_main.c
 * Author: Tetsuya
 *
 * Created on 2016/02/12, 19:22
 */
#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "pic_clock.h"
#include "MCLR_reset.h"
#include "uart_serial.h"
#include "AD_value.h"
#include "plati_temp.h"
#include "spi_master.h"
#include "system_protocol.h"
#include "OBC1_app.h"


//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

cw_t cw = CW_DATA_INIT;


void interrupt get_uart(void);
void check_bit(uint8_t data);


int main(void){
    
    LED0TRIS = 0;
    LED0 = 0;
    LED1TRIS = 0;
    LED1 = 0;
    
    OBC1_ACK_TRIS = 0;
    
    OBC1_ACK = 1;
    
    MCLR_init();            // MCLR_reset 初期設定
    sysprot_init();         // system_protocol 初期設定
    ad_con_init();            // AD変換の初期設定
    spi_master_start();
    uart_init();
    uart_interrupt = 0;
    //__delay_ms(1000);
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(100);
        spi_master_send_int(COM, 0x12);
        __delay_ms(100);
        spi_master_send_int(COM, 0x34);
        __delay_ms(100);
        spi_master_send(COM, 0x55);
        
        /*
        if(i==10)
        {
            spi_master_send(COM, 0x55);
            spi_master_send(COM, 0x15);
            __delay_ms(500);
            i = 0;
        }
        */
        
        /*COMのステータス確認*/
        //uint8_t COM_stat = COM_status();
        
        /*
        LED0 = 1;
        LED1 = 1;
        __delay_ms(1000);
        LED0 = 0;
        LED1 = 0;
        __delay_ms(1000);
        check_bit(COM_stat);
        LED0 = 1;
        LED1 = 1;
        __delay_ms(1000);
        LED0 = 0;
        LED1 = 0;
        __delay_ms(1000);
        */

        /*POWのステータス確認*/
        //POW_status();
        
        /*OBC1作業関数*/
        //command(COM_stat);
        //__delay_ms(5000);
        //command(0x02);
        
        //check_bit(I2CTEMP_data[1]);
        
        /*
        long t;
        double temp = 0.0;
        t = (cw.temp[0] << 8) | cw.temp[1];
        if (t < 0) t = t - 65536;
        temp = (double)t / 128.0;
        
        if(temp >= 10.0){
            LED1 = 1;
            __delay_ms(100);
            LED1 = 0;
            __delay_ms(00);
        }
        */
        
        LED0 = 0;
        __delay_ms(100);   
    }
    
    return 0;
}

void interrupt get_uart(void)
{
    if(PIR1bits.RCIF == 1)
    {   
        LED1 = 1;
        __delay_ms(500);
        PIR1bits.RCIF = 0;         // UART割り込みフラグ解除
        I2CTEMP_data[uart_interrupt] = getch();                  // 温度データをOBC2から取得
        spi_master_send(COM, I2CTEMP_data[uart_interrupt]);
        uart_interrupt++;
        LED1 = 0;
        __delay_ms(1000);
    }
}


void check_bit(uint8_t data)
{
    const uint8_t bit_mask = 0b10000000;
    uint8_t check_bit;
    uint8_t i;
    
    for(i = 0; i < 8; i++)
    {
        check_bit = bit_mask & data;
        
        LED0 = 1;
        __delay_ms(100);
        LED0 = 0;
        __delay_ms(100);
                
        if(check_bit)
        {
            LED1 = 1;
            __delay_ms(1000);
            LED1 = 0;
            __delay_ms(1000);
        }
        else
        {
            __delay_ms(2000);
        }
        data <<= 1;
    }

}