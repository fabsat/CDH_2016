/*******************************************************************************
*  skADT7410.h - 温度センサADT7410用関数ライブラリのインクルードファイル       *
*                                                                              *
* ============================================================================ *
* ============================================================================ *
*   VERSION  DATE        BY             CHANGE/COMMENT                         *
* ---------------------------------------------------------------------------- *
*   1.00     2015-06-05  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.7.3    (Ardino Duemilanove 328/UNO)(Arduino Zero Pro)        *
*******************************************************************************/
#ifndef SKADT7410_h
#define SKADT7410_h

#include <xc.h>
#include <pic16f877a.h>
#include <math.h>
#include "pic_clock.h"
#include "i2c.h"

//I2Cバスのスレーブアドレス
#define BUS_ADRS 0x48

// デバイスのレジスタアドレス
#define TEMP_DATA_ADRS    0x00         // 読み出すデータの先頭レジスタアドレス
#define STAT_REG_ADRS     0x02         // ステータスレジスタアドレス
#define CONF_REG_ADRS     0x03         // コンフィギュレーションの設定レジスタアドレス

// コンフィギュレーションの設定
// (INT/CT機能は利用しないのでデフォルトのままとする)
#define CONF_REG_DATA     0b11100000   // 分解能16Bit シャットダウンモード

// モードの設定
#define ADT_MODE_SHUTDOWN 0b01100000   // シャットダウンモード
#define ADT_MODE_ONESHOT  0b00100000   // 単発測定モード
#define ADT_MODE_CONTINUE 0b00000000   // 連続測定モード
#define ADT_MODE_1SPS     0b01000000   // １秒間隔測定モード

unsigned char ADT_Begin(void);
unsigned char ADT_ActionMode(unsigned char mode);
unsigned char ADT_Receive(char reg_adrs, unsigned char *data, unsigned char kosu);
unsigned char ADT_Send(char reg_adrs, unsigned char *data, unsigned char kosu);
long ADT_Read(double *temp);

#endif
