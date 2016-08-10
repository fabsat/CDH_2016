/******************************************************************************************
 * OBC1_app.c
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * OBC1プロトコルのソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/03/06 || Tetsuya Kaku
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE
 * XC8
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "pic_clock.h"
#include "system_protocol.h"
#include "spi_master.h"
#include "OBC1_app.h"
#include "MCLR_reset.h"
#include "uart_serial.h"

int uart_interrupt;
uint8_t I2CTEMP_data[2];

static void LED_data_set(void);
void I2Ctemp_data_set(void);

    

uint8_t COM_status(void)
{
    uint8_t COM_stat = 0;
    spi_master_send(COM, 0x01);
    COM_stat = spi_master_receive(COM);           // COMのステータス受け取り
    if(COM_stat == 0x00 || COM_stat == 0xFF)    // COMの生存判定
    {
        MCLR_reset(COM_RESET);                      // COMをリセット
    }
    return COM_stat;
}

void POW_status(void)
{
    spi_master_send(POW, 0x01);
    if(POW_READY==0) MCLR_reset(POW_RESET);         //POWの生存判定＆POWをリセット
}

void command(uint8_t data)
{
    switch(data)
    {
        case GET_CW_DATA:
            get_cw_data();
            cw_data_set(&cw);
            spi_master_send(COM, 0x11);
            send_data_master(COM, CW, DATA_END);
            break;
            
        case LED_BLINK:
            LED1 = 1;
            __delay_ms(100);
            LED1 = 0;
            __delay_ms(100);
            LED_data_set();
            spi_master_send(COM, 0x11);
            send_data_master(COM, LED_BLINK, DATA_END);
            break;

        case GET_I2C_TEMP:
            putch(0x01);                                        // OBC2へUART割り込み
            PIE1bits.RCIE = 1;
            INTCONbits.GIE = 1;
            INTCONbits.PEIE = 1;
            
            while(uart_interrupt!=2){
                LED1 = 1;
                __delay_ms(40);
                LED1 = 0;
                __delay_ms(40);
            }
            uart_interrupt = 0;
            PIE1bits.RCIE = 0;         // UART割り込みフラグ解除
            INTCONbits.GIE = 0;
            INTCONbits.PEIE = 0;
            sent_data_set(I2CTEMP_data, 2, 1);          // 温度データをパケットに格納
        
            //I2Ctemp_data_set();                                 // I2C温度データセット
            send_data_master(COM, GET_I2C_TEMP, DATA_END);
            break;
    }
}

/*LEDのパケット格納*/
static void LED_data_set(void)
{
    sent_data_set(0x01, 1, 1);
}

/*I2C温度センサデータをパケット格納*/
void I2Ctemp_data_set(void)
{       
    //sent_data_set(I2CTEMP_data, 2, 1);          // 温度データをパケットに格納
}

