// See LICENSE for license details.

#include <string.h>
#include "uart_lite.h"
#include "fdt.h"

volatile uint32_t* uart_lite;

void uart_lite_putchar(uint8_t ch)
{
  // TODO: Use atomic operations?
  volatile uint32_t *tx = uart_lite + UART_LITE_REG_TXFIFO;
  volatile uint32_t *st = uart_lite + UART_LITE_REG_STAT;

  while ((*st) & UART_LITE_STAT_TXFULL);
  *tx = ch;
}

int uart_lite_getchar()
{
  volatile uint32_t *rx = uart_lite + UART_LITE_REG_RXFIFO;
  volatile uint32_t *st = uart_lite + UART_LITE_REG_STAT;

  if ((*st) & UART_LITE_STAT_RXVALID)
    return -1;

  return (*rx) & 0xFF;
}

struct uart_lite_scan
{
  int compat;
  uint64_t reg;
};

static void uart_lite_open(const struct fdt_scan_node *node, void *extra)
{
  struct uart_lite_scan *scan = (struct uart_lite_scan *)extra;
  memset(scan, 0, sizeof(*scan));
}

static void uart_lite_prop(const struct fdt_scan_prop *prop, void *extra)
{
  struct uart_lite_scan *scan = (struct uart_lite_scan *)extra;
  if (!strcmp(prop->name, "compatible") && fdt_string_list_index(prop, "xlnx,uart-lite") >= 0) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg")) {
    fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void uart_lite_done(const struct fdt_scan_node *node, void *extra)
{
  struct uart_lite_scan *scan = (struct uart_lite_scan *)extra;
  if (!scan->compat || !scan->reg || uart_lite) return;

  // No further initialization necessary.
  uart_lite = (void*)(uintptr_t)scan->reg;
}

void query_uart_lite(uintptr_t fdt)
{
  struct fdt_cb cb;
  struct uart_lite_scan scan;

  memset(&cb, 0, sizeof(cb));
  cb.open = uart_lite_open;
  cb.prop = uart_lite_prop;
  cb.done = uart_lite_done;
  cb.extra = &scan;

  fdt_scan(fdt, &cb);
}
