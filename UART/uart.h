/********************************************************************************/
/*   @TITLE : uart.h                                                            */
/*    @DATE : January 18, 2016                                                  */
/*  @AUTHOR : Enzo IGLESIS                                                      */
/* @VERSION : 1                                                                 */
/********************************************************************************/
#ifndef UART_H
#define UART_H

/** ===============================  Hardware  =============================== **/
/*
    +--------------+
    |VCC    M   GND|
    |P1.0   S   XIN|
RX->|P1.1   P  XOUT|
TX<-|P1.2   4  TEST|
    |P1.3   3   RST|
    |P1.4   0  P1.7|
    |P1.5   G  P1.6|
    +------ 2 -----+
    |P2.0   5  P2.5|
    |P2.1   5  P2.4|
    |P2.2   3  p2.3|
    +--------------+

USB:
change jumpers TXD and RXD, to horizontally

*/
/** ===============================   Macros   =============================== **/
/**** TRUE & FALSE ****/
#ifndef TRUE
    #define TRUE    1
#endif
#ifndef FALSE
    #define FALSE   0
#endif

/**** PIN ****/
#define UART_RX BIT1
#define UART_TX BIT2

/**** BITS ****/
#define BITS_7  1
#define BITS_8  0
/**** PARITY_BIT ****/
#define PARITY_DISABLED 0
#define PARITY_ODD      2
#define PARITY_EVEN     3
/**** STOP_BIT ****/
#define STB_1   0
#define STB_2   1

/** =============================== Functions  =============================== **/
void uart_baud(const unsigned long int baud_rate, const unsigned long int cpu_freq);
void uart_format(const unsigned char bits, const unsigned char parity_bit, const unsigned char stop_bit);
void uart_setup_device(const unsigned long int cpu_freq);
unsigned char uart_rd_byte(unsigned char * byte);
unsigned char uart_wr_byte(const unsigned char byte);
char uart_getchar(void);
void uart_putchar(const char c);
void uart_rd_data(unsigned char * data, const unsigned char length);
void uart_wr_data(const unsigned char * data, const unsigned char length);
unsigned short int uart_gets(unsigned char * str, const unsigned short int max_length);
unsigned char uart_puts(const char * str);

#endif
