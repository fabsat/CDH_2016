#include <xc.h>
#include "pic_setting.h"
#include "uart.h"
#include "spi_master.h"
#include "protocol.h"
#include "uint8_to_string.h"



/* Configuration bits */
#pragma config FOSC  = HS      // Oscillator Selection bits (HS oscillator)
#pragma config WDTE  = OFF     // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP   = OFF     // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD   = OFF     // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT   = OFF     // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP    = OFF     // Flash Program Memory Code Protection bit (Code protection off)



/* Prototype of static function */
static void pic_port_init(void);

extern bool_t rx_packet_status;

/******************************************************
 * main function
 *****************************************************/
int main(void)
{ 
    const uint8_t send_1[] = 
    {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', 'm', 'n', 'o', 'p'};

    const uint8_t send_2[] = 
    {'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4'};
    
    uint8_t i;
    uint8_t r_data_type;
    uint8_t r_data[30];
    uint8_t r_data_len;
    
    
    pic_port_init();
    
    /* confirm Power on of PIC */
    RD6 = 1;
    __delay_ms(500);
    RD6 = 0;
    __delay_ms(500);

    /* Initialize UART */
    uart_init();
    spi_master_start();

    /* Start message transmitted */
    put_string("Master Program Start!\r\n");
    
    __delay_ms(1000);

    //sent_data_set(send_1, sizeof(send_1), sizeof(uint8_t));
    //send_data_master(POW, DATA, DATA_END);
    
    //__delay_ms(1000);
    //sent_data_set(send_2, sizeof(send_2), sizeof(uint8_t));
    //send_data_master(POW, DATA, DATA_END);
    
    
    uint8_t cmd = 0x02;                                // コマンドを定義
    sent_data_set(&cmd, sizeof(cmd), sizeof(uint8_t)); // パケットにコマンドをセット
    send_data_master(POW, COMMAND, DATA_END);          // パケットを送信する
    receive_data_master(POW);                          // コマンドの応答を受信
    
    while(1)
    {
        RD6 = 1;
        __delay_ms(200);
        RD6 = 0;
        __delay_ms(200);
        
        if(rx_packet_status == TRUE)
        {
            extract_receive_packet(&r_data_type, r_data, &r_data_len);
            for(i = 0; i < r_data_len; i++)
            {
                put_char(r_data[i]);
            }
        }
    }
    
    return 0;
}


/*-----------------------------------------------------
 * @brief
 *     Set all port OUTPUT
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     Prevent for PORT incorrect action
 *---------------------------------------------------*/
static void pic_port_init(void)
{
    /* Set all port OUTPUT */
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    /* All port are driven low */
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
}
