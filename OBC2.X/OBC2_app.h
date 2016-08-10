/* 
 * File:   OBC2_app.h
 * Author: Tetsuya
 *
 * Created on 2016/03/06, 20:24
 */

#ifndef OBC2_APP_H
#define	OBC2_APP_H

#include <xc.h>
#include "pic_clock.h"


#define OBC1_MCLR        TRISEbits.TRISE0
#define OBC1_STATUS      PORTEbits.RE0
#define OBC1_ACK_TRIS    TRISDbits.TRISD2
#define OBC1_ACK         PORTDbits.RD2
#define PORTE_REG_ADR    0x09                 // PORTEのレジスタアドレス
#define PORTE_POEWR_ON   0x01                 // RE0をON


/*=====================================================
 * @brief
 *     MCLRリセットの初期化
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_RESET_init(void);

/*=====================================================
 * @brief
 *     MCLRリセットを実行
 * @param
 *     resetted_mcu:リセットされるMCUを選択
 *                  ヘッダファイルのマクロを使用
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void OBC1_reset(void);


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
void OBC1(void);

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
void interrupt I2Ctemp(void);



/*=====================================================
 * @brief
 *     OBC2の割り込み可能設定関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     割り込み可能設定
 *===================================================*/
void interrupt_able(void);


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
void interrupt_disable(void);

/*=====================================================
 * @brief
 *     I2Cセンサ温度データ送信関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     I2C温度データ取得＆OBC1へ送信
 *===================================================*/
void trans_I2Ctemp(void);


#endif	/* OBC2_APP_H */

