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
#include "i2c.h"
#include "tempADT.h"
#include "OBC1_app.h"
#include "OBC2_app.h"
#include "spi_master.h"

//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

void interrupt I2Ctemp(void);

cw_t cw = CW_DATA_INIT;

int main(void){

    LED0TRIS = 0;
    LED0 = 0;
    OBC1_RESET_init();
    I2C_init();
    uart_init();
    LED0 = 1;
    __delay_ms(1000);
    LED0 = 0;
    __delay_ms(1000);
    //interrupt_able();
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(100);
        interrupt_able();
        //trans_I2Ctemp();
        /*OBC1生存確認＆リセット*/
        //while(!OBC1_ACK)
        //{
        //    int count = 0;
        //    if(count < 100){
        //        OBC1_reset();       // OBC1の状態によってOBC1をリセットする
        //        count++;
        //   }else{
        //        interrupt_disable();        // 全割り込み禁止
        //        OBC1();                     // OBC2がOBC1になる
        //    }
        //}
        LED0 = 0;
        __delay_ms(100);
    }
    return 0;
}


/*=====================================================
 * @brief
 *     OBC2の割り込み関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     OBC1からの割り込み(I2C温度センサデータ要求)
 *===================================================*/
void interrupt I2Ctemp(void)
{
    if(PIR1bits.RCIF == 1)
    {
        PIR1bits.RCIF == 0;         // UART割り込みフラグ解除
        interrupt_disable();
        trans_I2Ctemp();            // OBC1に温度データ送信
    }
}
