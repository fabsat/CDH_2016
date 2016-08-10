/************************************************
 * i2c.c
 * Tetsuya Kaku
 * =============================================
 * I2C通信ソースファイル
 * =============================================
 * ・ver1.00 一版 2016/02/13 Tetsuya Kaku
 * I2C通信
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/
#include <xc.h>
#include <pic16f877a.h>
#include <math.h>
#include "pic_clock.h"
#include "i2c.h"


void I2C_init(void){
    
    TRISSDL = 0;
    SCL = 1;
    TRISSDA = 1;
    SDA = 1;
}

void I2C_Start(void){
    /*SCL, SDAは常時Highとする*/
    SDA = 0;
    TRISSDA = 0;
}


void I2C_Stop(void){
    
    SCL = 0;
    __delay_us(1);
    SDA = 0;
    TRISSDA = 0;
    SCL = 1;
    __delay_us(1);
    SDA = 1;
    __delay_us(10);
}


unsigned char I2C_Out(unsigned char data){
  
    int i;
    unsigned char BitPos, ACK;
    
    TRISSDA = 0;
    BitPos = 0x80;
    for(i=0; i<8; i++){
        SCL = 0;
        if((data & BitPos) != 0){
            SDA = 1;
        }else{
            SDA = 0;
        }
        BitPos = BitPos >> 1;
        SCL = 1;
    }
    
    SCL = 0;
    TRISSDA = 1;
    __delay_us(2);
    SCL = 1;
    __delay_us(2);
    ACK = SDA;
    
    return (ACK);
}


void I2C_Send(unsigned char slave_adr, unsigned char data){
    
    unsigned char adr = 0x00;
    adr = slave_adr << 1;
    
    I2C_Start();
    I2C_Out(adr);
    
    I2C_Out(data);
    I2C_Stop();
}


unsigned char I2C_Rcv(unsigned char Ack){
    
    int i;
    unsigned char BitPos, data;
    
    data = 0;
    BitPos = 0x80;
    
    for(i=0; i<8; i++){
        SCL = 0;
        TRISSDA = 1;
        __delay_us(3);
        SCL = 1;
        if(SDA){
            data |= BitPos;
        }
        BitPos = BitPos >> 1;
    }
    
    /*ACK/NAK 出力*/
    SCL = 0;
    SDA = Ack;
    TRISSDA = 0;
    __delay_us(2);
    SCL = 1;
    
    return (data);
}


void I2C_GetData(unsigned char slave_adr, unsigned char *Buffer, unsigned char CNT){
    
    unsigned char i = 0;
    unsigned char adr = 0x00;
    
    adr = slave_adr << 1;
    adr = adr | 0x01;

    I2C_Start();    
    I2C_Out(adr);    
    
    for(i=0; i<CNT; i++){
        Buffer[i] = I2C_Rcv(0);      
    }
    Buffer[i] = I2C_Rcv(1);
    I2C_Stop();
}    

