// See LICENSE for license details.

#ifndef _RISCV_UART_LITE_H
#define _RISCV_UART_LITE_H

#include <stdint.h>

extern volatile uint32_t* uart_lite;

// Register map
#define UART_LITE_REG_RXFIFO      0
#define UART_LITE_REG_TXFIFO      1
#define UART_LITE_REG_STAT        2
#define UART_LITE_REG_CTRL        3

// Control register
#define UART_LITE_CTRL_RSTTX      0x01
#define UART_LITE_CTRL_RSTRX      0x02
#define UART_LITE_CTRL_INTEN      0x10

// Status register
#define UART_LITE_STAT_RXVALID    0x01
#define UART_LITE_STAT_RXFULL     0x02
#define UART_LITE_STAT_TXEMPTY    0x04
#define UART_LITE_STAT_TXFULL     0x08
#define UART_LITE_STAT_INTEN      0x10
#define UART_LITE_STAT_OVERRUNERR 0x20
#define UART_LITE_STAT_FRAMEERR   0x40
#define UART_LITE_STAT_PARITYERR  0x80


void uart_lite_putchar(uint8_t ch);
int uart_lite_getchar();
void query_uart_lite(uintptr_t dtb);

#endif
