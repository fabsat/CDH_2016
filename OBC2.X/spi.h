/******************************************************************************************
 * spi_slave.h
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI Slaveライブラリヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/28 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(V3.10/Ubuntu)
 * XC8 (V1.35/Linux)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/

#ifndef _SPI_H
#define	_SPI_H

#include <xc.h>
#include <pic16f877a.h>
#include "pic_clock.h"

/* SPI interrupt enable type */
typedef enum
{
    SPI_ISR_ENABLE,      // Enable
    SPI_ISR_DISABLE,     // Disable
} spi_isr_set_t;
    


/* Prototype of function */
/*=====================================================
 * @brief
 *     SPI Slave初期化関数
 * @param
 *     spi_isr:spi割り込みの有効無効を設定
 * @return
 *     なし
 * @note
 *     pin43:SDO(RC5)
 *     pin42:SDI(RC4/SDA)
 *     pin37:SCK(RC3/SCL)
 *     pin24:SS (RA5/AN4/C2OUT)
 *===================================================*/
void spi_slave_init(spi_isr_set_t spi_isr);


/*=====================================================
 * @brief
 *     SPIデータ送受信関数(1Byte)
 * @param
 *     data:送信データ(char型)
 * @return
 *     recieved_data:受信データ
 *     -1           :timeout終了時
 * @note
 *     ・通信完了フラグの待ち状態でreset_counterが
 *       0になったらresetをかける
 *     ・2回resetをかけても完了しなければtimeoutと
 *       して-1を返す
 *===================================================*/
char spi_send_receive(char data);


/*=====================================================
 * @brief
 *     SPI Interrupt function
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void spi_interrupt(void);



#endif	/* _SPI_SLAVE_H */
