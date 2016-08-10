/******************************************************************************************
 * pic_clock.h
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * delay用のクロック定義ヘッダファイルと各データ型のtypedef
 *
 *=========================================================================================
 * ・ver1.00 || 2015/03/04 || Tetsuya Kaku
 *   20MHzで定義
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef PIC_CLOCK_H
#define	PIC_CLOCK_H

#include <xc.h>

/* Define Clock Frequency for Delay */
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

typedef unsigned char uint8_t;
typedef signed char    int8_t; 
typedef unsigned int uint16_t;
typedef signed int    int16_t;


#endif	/* PIC_CLOCK_H */
