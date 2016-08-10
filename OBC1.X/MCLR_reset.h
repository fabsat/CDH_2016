/******************************************************************************************
 * MCLR_reset.h
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * MCLRリセットヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2015/03/04 || Tetsuya Kaku
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef MCLR_RESET_H
#define	MCLR_RESET_H

#include <xc.h>
#include "pic_clock.h"

#define COM_MCLR       TRISEbits.TRISE1     // MCLRに使用するピンをRE1
#define COM_RESET      0x05                 // COMだけをリセット(RE1ピンをLOWに)
#define POW_MCLR       TRISEbits.TRISE0     // MCLRに使用するピンをRE0
#define POW_RESET      0x06                 // POWだけをリセット(RE0ピンをLOWに)
#define PORTE_REG_ADR  0x09                 // PORTEのレジスタアドレス
#define PORTE_POEWR_ON 0x03                 // RE1, 2をON

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
void MCLR_init(void);


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
void MCLR_reset(uint8_t resetted_mcu);



#endif	/* MCLR_RESET_H */
