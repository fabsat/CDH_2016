#include <xc.h>
#include "pic_setting.h"
#include "uart.h"
#include "spi_slave.h"
#include "isr.h"
#include "protocol.h"
#include "com_app.h"


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
    //uint8_t i;
    uint8_t r_data_type;
    uint8_t r_data[30];
    uint8_t r_data_len;
    
    uint8_t send_data[] = {'a', 'b', 'c', 'd', 'e'};
    
    pic_port_init();
    
    /* confirm Power on of PIC */
    RA0 = 1;
    __delay_ms(500);
    RA0 = 0;
    __delay_ms(500);

    /* Initialize UART */
    uart_init();
    spi_slave_start(SPI_ISR_ENABLE);

    /* Start message transmited */
    put_string("Slave Program Start!\r\n");
    
    while(1)
    {
        RA0 = 1;
        __delay_ms(100);
        RA0 = 0;
        __delay_ms(100);
        
        if (rx_packet_status == TRUE)
        {
            extract_receive_packet(&r_data_type, r_data, &r_data_len);
            put_char(r_data[0]);
            put_char(r_data[1]);
            
            if(r_data[1] == 0x02)
            {
                __delay_ms(5000);
                sent_data_set(send_data, sizeof(send_data), sizeof(uint8_t));
                send_data_slave(DATA, DATA_END);
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
