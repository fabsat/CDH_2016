/******************************************************************************************
 * OBC2_app.c
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * OBC2プロトコルのソースファイル
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
#include <stdio.h>
#include "pic_clock.h"
#include "system_protocol.h"
#include "tempADT.h"
#include "uart_serial.h"
#include "spi_master.h"
#include "OBC2_app.h"
#include "OBC1_app.h"

/*=====================================================
 * @brief
 *     MCLRによるOBC1リセットの初期化
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_RESET_init(void)
{
    OBC1_ACK_TRIS = 1;       // OBC1のステータス確認ピンを入力ピンに設定
    uint8_t *nin;
    nin = PORTE_REG_ADR;     // PORTEのレジスタアドレスをポインタ変数に格納

    /* MCLRリセットに使用する各I/Oピンを出力に設定 */
    OBC1_MCLR = 0;
    
    /* 信号をHighにしてリセットOff */
    *nin = PORTE_POEWR_ON;
}

/*=====================================================
 * @brief
 *     MCLRによってOBC1をリセットを実行
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_reset(void)
{
    uint8_t *nin;
    nin = PORTE_REG_ADR;
    
    /* リセット実行 */
    *nin = 0;      // OBC1をリセット(RE0ピンをLOWに)
    __delay_ms(10);
    *nin = PORTE_POEWR_ON;              // 全てHIGHにして再起動
}


/*=====================================================
 * @brief
 *     OBC2の割り込み可設定関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     割り込み可能設定
 *===================================================*/
void interrupt_able(void)
{
    /*割り込み許可*/
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}


/*=====================================================
 * @brief
 *     OBC2の割り込み不可設定関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     割り込み不可設定
 *===================================================*/
void interrupt_disable(void)
{
    /*割り込み禁止*/
    PIE1bits.RCIE = 0;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
}



/*=====================================================
 * @brief
 *     OBC1の機能を実行
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1(void)
{
    MCLR_init();            // MCLR_reset 初期設定
    sysprot_init();         // system_protocol 初期設定
    spi_master_start();     // spiをOBC1の代わりに実行
    uart_init();            // OBC1との通信
    uint8_t COM_s;
    
    while(1)
    {
        LED0 = 1;
        __delay_ms(1000);

        /*COMのステータス確認*/
        COM_s = COM_status();

        /*POWのステータス確認*/
        POW_status();
                
        /*コマンド関数*/
        command(COM_s);

        LED0 = 0;
        __delay_ms(1000);   
    }
}

/*I2C温度センサデータをOBC1に送信*/
void trans_I2Ctemp()
{
    uint8_t data[2];
    ADT_Receive(TEMP_DATA_ADRS, &data[0], 2);
    __delay_ms(3000);
    putch(data[0]);
    __delay_ms(1000);
    putch(data[1]);
}

