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
#include "uart_serial.h"
#include "AD_value.h"
#include "plati_temp.h"
#include "spi_master.h"
#include "MCLR_reset.h"
#include "system_protocol.h"


//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

#define LED0TRIS    TRISD7
#define LED0        PORTDbits.RD7
#define LED1TRIS    TRIS
#define LED1        PORT

cw_t cw = CW_DATA_INIT;


int main(void){
    
    LED0TRIS = 0;
    LED0 = 0;

    //MCLR_init();    // MCLR_reset 初期設定
    sysprot_init(); // system_protocol 初期設定
    uart_init();
    //ad_con_init();  // AD変換の初期設定

    //spi_master_init();

    
    /*生存判定*/
    //if(COM_READY==0) MCLR_reset(COM_RESET);
    //if(POW_READY==0) MCLR_reset(POW_RESET);
    
    put_string("Program Start!\r\n");
    
    const double data0 = 1.0;
    const double data4 = 1.0;
    const uint8_t data1 = 0x01;
    const double data5 = 1.0;
    const uint16_t data2 = 0x02;
    const double data3 = 1.0;
     
    sent_data_set(&data0, 1, 4);
    sent_data_set(&data4, 1, 4);

    sent_data_set(&data2, 1, 2);
    sent_data_set(&data5, 1, 4);
    sent_data_set(&data1, 1, 1);
    sent_data_set(&data3, 1, 4);
    
    show_packet();
    
            
    while(1){
        RD7 = 1;
//        MCLR_reset(POW_RESET);            //　これをしたらBBM全体がリセットされる
        __delay_ms(1000);
   
        RD7 = 0;
        __delay_ms(1000);
    }
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(1000);
        get_cw_data();
        cw_data_set(&cw);
        send_data_master(COM, CW, DATA_END);
        LED0 = 0;
        __delay_ms(1000);
        
        /*COM生存確認*/
        if(COM_READY==0) 
    
        __delay_ms(10000);
    }
    
    
    return 0;
}

