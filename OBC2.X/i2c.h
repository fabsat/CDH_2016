/************************************************
 * i2c_temp.h
 * Tetsuya Kaku
 * =============================================
 * i2c温度センサヘッダファイル
 * =============================================
 * ・ver1.00 一版 2016/02/13 Tetsuya Kaku
 * I2Cで温度センサを使用する
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/
#ifndef I2C_H
#define	I2C_H

#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "pic_clock.h"

#define TRISSDA TRISDbits.TRISD4
#define TRISSDL TRISDbits.TRISD5 
#define SDA PORTDbits.RD4
#define SCL PORTDbits.RD5

void I2C_init(void);
void I2C_Start(void);
void I2C_Stop(void);
unsigned char I2C_Out(unsigned char data);
void I2C_Send(unsigned char slave_adr, unsigned char data);
unsigned char I2C_Rcv(unsigned char Ack);
void I2C_GetData(unsigned char slave_adr, unsigned char *Buffer, unsigned char CNT);

#endif	/* I2C_H */

