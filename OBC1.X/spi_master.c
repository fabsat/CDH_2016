/******************************************************************************************
 * spi_master.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI通信(Master)ソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/27 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(V3.10/Ubuntu)
 * XC8 (V1.35/Linux)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "spi_master.h"
#include "pic_clock.h"
#include "system_protocol.h"


/*=====================================================
 * @brief
 *     SPI Masterスタート関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     Pin43:SDO(RC5)
 *     Pin42:SDI(RC4/SDA)
 *     Pin37:SCK(RC3/SCL)
 *===================================================*/
void spi_master_start(void)
{
    /* SPI pin I/O Configuration ( RC4 is SDI -> INPUT  Controlled by module??) */
    TRISCbits.TRISC3 = 0;    // RC3 is SCK -> OUTPUT
    TRISCbits.TRISC5 = 0;    // RC5 is SDO -> OUTPUT
    TRISCbits.TRISC4 = 1;    // RC4 is SDI -> INPUT
    
    /* SS pin configure OUTPUT */
    SS_COM_TRIS  = 0;
    SS_POW_TRIS  = 0;

    /* Allow Programming of SPI configuring */
    SSPCONbits.SSPEN = 0;

    /*  SPI Mode Setup */
    SSPSTATbits.SMP = 0;     // Input data sampled at middle of data output time
    SSPCONbits.CKP  = 0;     // Idle state for clock is a low level
    SSPSTATbits.CKE = 0;     // Transmit occurs on transition from idle to active clock state
    
    /* SPI Master mode, clock = FOSC/4(Maximum speed) */
    SSPCONbits.SSPM3 = 0;          
    SSPCONbits.SSPM2 = 0;          
    SSPCONbits.SSPM1 = 0;          
    SSPCONbits.SSPM0 = 0;

    /* SS_PIN set HIGH */
    SS_COM  = 1;
    SS_POW  = 1;
   
    /* End SPI programming and Start serial port */
    SSPCONbits.SSPEN = 1;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ受信関数(1Byte)
 * @param
 *     destination:通信の相手先を選択
 * @return
 *     void:
 * @note
 *     1[s]で受信完了しなければTIMEOUTとなる
 *===================================================*/
uint8_t spi_master_receive(destination_t destination)
{
    uint8_t dummy;
    uint16_t timeout_counter = 1000;

    /* ダミー変数にデータをリードする */
    dummy = SSPBUF;

    /* 各サブシステムからの準備完了通知を待つ */
    switch(destination)
    {
	    case COM:
            while(COM_READY == 0){;}  // COM_READYピンがセットされるまで待つ
            break;
        case POW:
            while(POW_READY == 0){;}  // POW_READYピンがリセットされるまで待つ
            break;
    }
    
    /* Slave Select -> Low */
    switch(destination)
    {
        case COM:
            SS_COM  = 0;
            break;
        case POW:
            SS_POW  = 0;
            break;
    }


    /* ダミーデータをSSPBUFにセットしてSPI通信開始 */
    SSPBUF = 0x00;

    /* 送信完了待ち */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (1[ms]を超えた時) */
        if(timeout_counter == 0)
        {
            return;
        }

        /* 1[us]ごとにtimeout_counterをデクリメントする */
        __delay_us(1);
        timeout_counter--;
    }

    /* Slave Select -> High */
    switch(destination)
    {
        case COM:
            SS_COM  = 1;
            break;
        case POW:
            SS_POW  = 1;
            break;
    }

    return SSPBUF;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ送信関数(1Byte)
 * @param
 *     destination:通信の相手先を選択
 *     data     :送信データ
 * @return
 *     void:
 * @note
 *     1[ms]で送信完了しなければTIMEOUTとなる
 *===================================================*/
void spi_master_send_int(destination_t destination, uint8_t data)
{
    uint8_t dummy;
    uint16_t timeout_counter = 1000;

    /* dummy変数にデータをリードする */
    dummy = SSPBUF;

    
    /* Slave Select -> Low */
    switch(destination)
    {
        case COM:
            SS_COM  = 0;
            break;
        case POW:
            SS_POW  = 0;
            break;
    }
    

    /* 送信データをSSPBUFにセット SPI通信スタート */
    SSPBUF = data;

    /* 各サブシステムからの準備完了通知を待つ */
    /*
    switch(destination)
    {
	    case COM:
            while(COM_READY == 0){;}  // COM_READYピンがセットされるまで待つ
            break;
        case POW:
            while(POW_READY == 0){;}  // POW_READYピンがリセットされるまで待つ
            break;
    }
    */
    
    /* 送信完了待ち */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (1[ms]を超えた時) */
        if(timeout_counter == 0)
        {
            return;
        }

        /* 1[us]ごとにtimeout_counterをデクリメントする */
        __delay_us(1);
        timeout_counter--;
    }

    /* Slave Select -> High */
    switch(destination)
    {
        case COM:
            SS_COM  = 1;
            break;
        case POW:
            SS_POW  = 1;
            break;
    }
    
    /* slaveから来たダミーデータを読み込んでおく */
    dummy = SSPBUF;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ送信関数(1Byte)
 * @param
 *     destination:通信の相手先を選択
 *     data     :送信データ
 * @return
 *     void:
 * @note
 *     1[ms]で送信完了しなければTIMEOUTとなる
 *===================================================*/
void spi_master_send(destination_t destination, uint8_t data)
{
    uint8_t dummy;
    uint16_t timeout_counter = 1000;

    /* dummy変数にデータをリードする */
    dummy = SSPBUF;

    /* 各サブシステムからの準備完了通知を待つ */
    switch(destination)
    {
	    case COM:
            while(COM_READY == 0){;}  // COM_READYピンがセットされるまで待つ
            break;
        case POW:
            while(POW_READY == 0){;}  // POW_READYピンがリセットされるまで待つ
            break;
    }
    
    /* Slave Select -> Low */
    switch(destination)
    {
        case COM:
            SS_COM  = 0;
            break;
        case POW:
            SS_POW  = 0;
            break;
    }
    

    /* 送信データをSSPBUFにセット SPI通信スタート */
    SSPBUF = data;

    
    
    /* 送信完了待ち */
    while(SSPSTATbits.BF == 0)
    {
        PIE1bits.TMR1IE   = 1;  // Timer 1 overflow interupt enabled
        T1CONbits.TMR1ON  = 1;  // Timer 1 on
        
        /* TIMEOUT (300[ms]を超えた時) */
        if(timeout_counter == 0)
        {
            return;
        }

        /* 1[us]ごとにtimeout_counterをデクリメントする */
        __delay_us(300);
        timeout_counter--;
    }
    PIR1bits.TMR1IF   = 0;  // Reset interupt flag
    PIE1bits.TMR1IE   = 0;  // Timer 1 overflow interupt enabled
    T1CONbits.TMR1ON  = 0;  // Timer 1 on
    

    /* Slave Select -> High */
    switch(destination)
    {
        case COM:
            SS_COM  = 1;
            break;
        case POW:
            SS_POW  = 1;
            break;
    }
    
    /* slaveから来たダミーデータを読み込んでおく */
    dummy = SSPBUF;
}

/*=====================================================
 * @brief
 *     SPI stop関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void spi_master_stop(void)
{
    /* SS pin -> LOW */
    SS_COM  = 0;
    SS_POW  = 0;

    /* SPI(MSSP)無効化 */
    SSPCONbits.SSPEN = 0;
}

