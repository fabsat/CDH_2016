/* 
 * File:   OBC1_app.h
 * Author: Tetsuya
 *
 * Created on 2016/03/06, 18:06
 */

#ifndef OBC1_APP_H
#define	OBC1_APP_H

#define LED0TRIS    TRISDbits.TRISD7
#define LED0        PORTDbits.RD7
#define LED1TRIS    TRISDbits.TRISD6
#define LED1        PORTDbits.RD6

uint8_t COM_status(void);
void POW_status(void);
void command(uint8_t data);

#endif	/* OBC1_APP_H */

