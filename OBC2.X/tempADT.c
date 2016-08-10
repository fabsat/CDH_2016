/*******************************************************************************
*  skADT7410 - 温度センサADT7410用関数ライブラリ                               *
*                                                                              *
*    skADT7410  - この関数ライブラリを生成する時の初期化処理                   *
*    Begin      - 温度センサの初期化を行う処理                                 *
*    ActionMode - 温度センサの動作モードを設定する処理                         *
*    Receive    - 温度センサから指定個数のデータを受信する処理                 *
*    Send       - 温度センサに指定個数のデータを送信する処理                   *
*    Read       - 温度センサから読込みを行い温度値を返す処理                   *
* ============================================================================ *
*   VERSION  DATE        BY             CHANGE/COMMENT                         *
* ---------------------------------------------------------------------------- *
*   1.00     2015-06-05  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.7.3    (Ardino Duemilanove 328/UNO)(Arduino Zero Pro)        *
*******************************************************************************/
#include <xc.h>
#include <pic16f877a.h>
#include <math.h>
#include "pic_clock.h"
#include "i2c.h"
#include "tempADT.h"



/*******************************************************************************
*  ans = Begin()                                                               *
*    温度センサの初期化を行う処理                                              *
*    設定のデータは16bitの分解能です。                                         *
*    (INT/CTの機能は利用しないのでデフォルトのままです)                        *
*    初期デバイスの動作モードは"シャットダウン"でスタートです。                *
*    ans  : 戻り値、0=正常終了　それ以外I2C通信エラー下記                      *
*                   1=送ろうとしたデータが送信バッファのサイズを超えた         *
*                   2=スレーブ・アドレスを送信し、NACKを受信した               *
*                   3=データ・バイトを送信し、NACKを受信した                   *
*                   4=その他のエラー                                           *
*                   5=データ受信エラー                                         *
*******************************************************************************/
unsigned char ADT_Begin(void){
    
    unsigned char ans;
    unsigned char data[2];

    // センサを初期化する処理
    data[0] = CONF_REG_DATA;    // コンフィギュレーションの設定(16bit  Shutdown Mode)
    ans = ADT_Send(CONF_REG_ADRS, &data[0], 1);

    return ans;
}

/*******************************************************************************
*  ans = ActionMode(mode)                                                      *
*  温度センサの動作モード(測定モード)を設定する処理                            *
*    mode: 動作モードを指定する                                                *
*          ADT_MODE_SHUTDOWN／ADT_MODE_ONESHOT／ADT_MODE_CONTINUE/ADT_MODE_1SPS*
*    ans : 戻り値、0=正常終了　それ以外Begin()のans値を参照                    *
*******************************************************************************/
unsigned char ADT_ActionMode(unsigned char mode)
{
    unsigned char  ans = 0;
    unsigned char data[2] = {0};

    // 現在の設定情報を読み出す
    ans = ADT_Receive(CONF_REG_ADRS, &data[0], 1);
    
    if(ans == 0){
        data[1] = (data[0] & 0b10011111) | mode;
        ADT_Send(CONF_REG_ADRS, &data[1], 1);
        ans = 1;
    }

    return ans;
}

/*******************************************************************************
*  ans = Receive(reg_adrs,*data,kosu)                                          *
*  温度センサから指定個数のデータを受信する処理                                *
*    reg_adrs : 読出すデータのレジスターアドレスを指定する                     *
*               連続的に読出す場合は、読出すレジスターの先頭アドレスを指定     *
*    *data    : 読出したデータの格納先を指定する                               *
*    kosu     : 読出すデータのバイト数を指定する                               *
*    ans      : 戻り値、0=正常終了　それ以外Begin()のans値を参照               *
*******************************************************************************/
unsigned char ADT_Receive(char reg_adrs, unsigned char *data, unsigned char kosu){
  
    unsigned char ans, i;

    I2C_Send(BUS_ADRS, reg_adrs);    
    ans = 0;                            // データの送信と通信の終了
    
    if(ans == 0){
        I2C_GetData(BUS_ADRS, data, kosu);
        ans = 0;
     } else ans = 5;                             // 受信バイト数がおかしい？
   
     return ans; 
}

/*******************************************************************************
*  ans = Send(reg_adrs,*data,kosu)                                             *
*  温度センサに指定個数のデータを送信する処理                                  *
*    reg_adrs : 書出すデータのレジスターアドレスを指定する                     *
*               連続的に書出す場合は、書出すレジスターの先頭アドレスを指定     *
*    *data    : 書出すデータの格納先を指定する                                 *
*    kosu     : 書出すデータのバイト数を指定する                               *
*    ans      : 戻り値、0=正常終了　それ以外Begin()のans値を参照               *
*******************************************************************************/
unsigned char ADT_Send(char reg_adrs, unsigned char *data, unsigned char kosu){

    unsigned char ans, i;

    I2C_Send(BUS_ADRS, reg_adrs);
    
    I2C_Start();
    
    for(i=0; i<kosu; i++){
          I2C_Out(*data);                     // データを通知
          data++;
    }
    
    I2C_Stop();
    ans = 1;               // データの送信と通信の終了
   
    return ans ;
}

/*******************************************************************************
*  ans = Read(*temp)                                                           *
*  温度センサから読込みを行い温度値を返す処理                                  *
*  温度センサより読み込んだデータは、摂氏温度に変換し"temp"にセットする。      *
*    temp : 摂氏温度を返すのでfloat変数のアドレスを指定                        *
*    ans  : 戻り値、0=正常終了　それ以外Begin()のans値を参照                   *
*******************************************************************************/
long ADT_Read(double *temp){
    
    long t;
    unsigned char ans;
    unsigned char data[2];

    ans = ADT_Receive(TEMP_DATA_ADRS, &data[0], 2);
    
    if(ans == 0){
        t = (data[0] << 8) | data[1];
        //if (t < 0) t = t - 65536;
        //*temp = (double)t / 128.0;
    }
     
    //return ans;

    return t;
}

