/********************************************************************************/
/*   @TITLE : uart.c                                                            */
/*    @DATE : January 18, 2016                                                  */
/*  @AUTHOR : Enzo IGLESIS                                                      */
/* @VERSION : 2                                                                 */
/********************************************************************************/
/** ===============================  Library   =============================== **/
#include <msp430g2553.h>
#include "uart.h"

/** =============================== Functions  =============================== **/
void uart_baud(const unsigned long int baud_rate, const unsigned long int cpu_freq)
{
/* This function allows to set the baud rate. The first parameter is for the
baud rate desired, and the second one is for the SMCLK speed.*/
    UCA0CTL1 = UCSSEL_2|UCSWRST;
    UCA0BR0 = (cpu_freq/baud_rate)&0x00FF;
    UCA0BR1 = ((cpu_freq/baud_rate)>>8)&0x00FF;
    UCA0MCTL = ((unsigned short int)(((double)cpu_freq/(double)baud_rate-cpu_freq/baud_rate)*8.0+0.5)&0x0007)<<1;
    UCA0CTL1 &= ~UCSWRST;
}

void uart_format(const unsigned char bits, const unsigned char parity_bit, const unsigned char stop_bit)
{
/* This function allows to set the UART format. the first parameter is for the
number of bits (BITS_7 or BITS_8), the second one is for the parity
(PARITY_DISABLED, PARITY_ODD  or PARITY_EVEN) and the last one is for the number
of stop bits (STB_1 or STB_2). */
    IE2 &= ~(UCA0RXIE|UCA0TXIE);
    UCA0CTL1 |= UCSWRST;
    UCA0CTL0 = (parity_bit&(BIT1|BIT0))<<6;
    UCA0CTL0 |= (bits&BIT0)<<4;
    UCA0CTL0 |= (stop_bit&BIT0)<<3;
    UCA0CTL1 &= ~UCSWRST;
}

void uart_setup_device(const unsigned long int cpu_freq)
{
/* This function set P1.1 as USCI_A0 UART mode: receive data input and P1.2 as
USCI_A0 UART mode: transmit data output */
    P1SEL = UART_RX|UART_TX;
    P1SEL2 = UART_RX|UART_TX;
}

unsigned char uart_rd_byte(unsigned char * byte)
{
/* This function return TRUE, if a byte is available on the UART RX buffer or
FALSE if not. If a byte is available it will be write at the address pointed
by "byte" */
    if(!(IFG2&UCA0RXIFG))
    {
        return FALSE;
    }
    else
    {
        IE2 |= UCA0RXIE;
        (*byte) = UCA0RXBUF;
    }
    return TRUE;
}

unsigned char uart_wr_byte(const unsigned char byte)
{
/* This function return TRUE, if it's possible to write on UART TX buffer and
FALSE if not. If it's TRUE then "byte" will be write on the UART TX buffer. */
    if(!(IFG2&UCA0TXIFG))
    {
        return FALSE;
    }
    else
    {
        UCA0TXBUF = byte;
        IE2 |= UCA0TXIE;
    }
    return TRUE;
}

char uart_getchar(void)
{
/* This function waits for receiving a character from UART, and return the
character */
    /* Local declaration */
    unsigned char c;
    /* Progam statement */
    while(!uart_rd_byte(&c));
    return (char)c;
}

void uart_putchar(const char c)
{
/* This function waits until it's possible to send a character by the UART, and
then sends the parameter "c" */
    while(!uart_wr_byte((const unsigned char)c));
}

void uart_rd_data(unsigned char * data, const unsigned char length)
{
/* This function allows to read multiple data from the UART, the first parameter
is a pointer where data will be written, and the second is the number of data to
read. it's a blocking function */
    /* local declaration */
    unsigned char i = 0;
    /* Progam statement */
    for(i = 0; i<length; i++)
    {
        while(!uart_rd_byte(&data[i]));
    }
}

void uart_wr_data(const unsigned char * data, const unsigned char length)
{
/* This function allows to write multiple data from the UART, the first parameter
is a pointer of data to send, and the second is the number of data to send. it's a
blocking function */
    /* local declaration */
    unsigned char i = length;
    /* Progam statement */
    for(i = 0; i<length; i++)
    {
        while(!uart_wr_byte(data[i]));
    }
}

unsigned short int uart_gets(unsigned char * str, const unsigned short int max_length)
{
/* This function allows playback of a string, terminated by \n from the UART, and
places the result into a string  "str", to avoid an overflow of str, the max
length of the string is indicate with the parameter "length" .*/
    /* local declaration */
    unsigned short int i = 0;
    unsigned char c = 0;
    /* Progam statement */
    for(i = 0; i<max_length-1 && (c = uart_getchar()) != '\0' && c!= '\n' && c!= '\r'; ++i)
    {
        str[i] = c;
        while(!uart_wr_byte(str[i]));
    }
    if(c == '\n' || c == '\r')
    {
        str[i] = c;
        ++i;
    } 
    str[i] = '\0';
    return i;
}

unsigned char uart_puts(const char * str)
{
/* This function writes a string to the UART up to but not including the null
character. */
    /* local declaration */
    unsigned short int i=0;
    /* Progam statement */
    for(i = 0; str[i] != '\0' && i < 0xFFFF; i++)
    {
        uart_putchar(str[i]);
    }
    if(i == 0xFFFF && str[i] != '\0')
    {
        return FALSE;
    }
    return TRUE;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    IE2 &= ~UCA0TXIE;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    IE2 &= ~UCA0RXIE;   
}
