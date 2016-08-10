/******************************************************************************************
 * uart_serial.h
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * uart通信(シリアル)定義ヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2015/12/09 || Tetsuya Kaku
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef __UART_SERIAL_H__
#define __UART_SERIAL_H__

#include "pic_clock.h"
#include "uint8_to_string.h"


/******************************************************************************************
 *                                  ユーザー設定用マクロ                                   *
 *****************************************************************************************/
#define BAUDRATE    (9600)  // 9.6kbps
#define TX9_RX9_BIT 0       // 0: 8bit, 1: 9bit
#define BRGH_BIT    1       // 0: 低速サンプル指定, 1: 高速サンプル指定


/* 9bitのフォーマットの場合 */
#if TX9_RX9_BIT == 1
#define TX9_RX9_DATA 0x40
#else
#define TX9_RX9_DATA 0x00
#endif


/* 高速サンプルか低速サンプル */
#if BRGH_BIT == 1
#define BRGH_DATA 0x04
#define SPBRG_DATA ((unsigned char)(((_XTAL_FREQ / 16) / BAUDRATE) - 1))
#else
#define BRGH_DATA 0x00
#define SPBRG_DATA ((unsigned char)(((_XTAL_FREQ / 64) / BAUDRATE) - 1))
#endif



/******************************************************************************************
 *                                   プロトタイプ宣言                                      *
 *****************************************************************************************/
/*=====================================================
 * @breif
 *     uart通信初期設定関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     型番によってRXピンを確認する必要がある
 *===================================================*/
void uart_init(void);


/*=====================================================
 * @brief
 *     1Byteデータ送信
 * @param
 *     byte:送信データ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void putch(uint8_t byte);



/*=====================================================
 * @brief
 *     文字列送信
 * @param
 *     str:文字列へのポインタ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void put_string(uint8_t *str);



/*=====================================================
 * @brief
 *     1Byte受信
 * @param
 *     data:受信したデータ
 * @return
 *     0xFF:受信失敗
 *     0x00:受信成功
 * @note
 *     none
 *===================================================*/
uint8_t getch(void);

#endif  // __UART_SERIAL_H__
