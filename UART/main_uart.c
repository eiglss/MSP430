/********************************************************************************/
/*   @TITLE: main_uart.c                                                        */
/*    @DATE: January 18, 2016                                                   */
/*   @AUTOR: Enzo IGLESIS                                                       */
/* @VERSION: 0                                                                  */
/********************************************************************************/

/* This is a short program example, that show how to use the library uart.h. All
functions of this library are not all use in this program.
The purpose of this programme is to clear an UART terminal, display "Hello world!"
at the cursor home position and go to the next line where the text sent to the
MSP430 will be resend to the terminal. */

/*================================   Library   =================================*/
#include <msp430g2253.h>
#include "uart.h"

/*================================   Macros    =================================*/
/**** CPU ****/
#define CPU_FREQENCY    (16000000UL)

/*================================ Functions  ==================================*/
void initializing_watchdog(void)
{
    WDTCTL = WDTPW + WDTHOLD; /* stop watchdog */
}

void initializing_clock(void)
{
/* Set the SMCLK at 16 MHZ. Don't forget to change CPU_FREQ macro if you change
the clock speed */
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

/*================================    Main    ==================================*/
void main(void)
{
    /* Local declarations */
    /* Initialization */
    initializing_watchdog();
    initializing_clock();
    uart_setup_device(CPU_FREQENCY); /* configure P1.1 & P1.2 to use as uart port */
    uart_baud(9600, CPU_FREQENCY); /* set the baud rate at 9600 */
    uart_format(BITS_8, PARITY_DISABLED, STB_1); /* set the uart format 8,N,1 */
    __enable_interrupt(); /* enbale global interruption */
    /* Progam statement */
    uart_puts("\x1B[2J\x1B[HHello World!\n");
    while(1)
    {
        uart_putchar(uart_getchar());
    }
}
