#ifndef _MODULE_H_
#define _MODULE_H_

#include <limits.h>

#include "stm32f4xx_ll_cortex.h"
#include "cmsis_gcc.h"

#define SUCCESS               0
#define MOD_ERR_INIT         -1
#define MOD_ERR_ARGS         -2
#define MOD_ERR_BAD_INSTANCE -3
#define MOD_ERR_BUF_OVERRUN  -4
#define MOD_ERR_RESOURCE     -5

// Get size of an array.
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* // Increment a uint16_t, saturating at the maximum value. */
#define INC_SAT_U16(a) do { (a) += ((a) == UINT16_MAX ? 0 : 1); } while (0)

static inline uint32_t disable_irq() {
  uint32_t primask;
  primask = __get_PRIMASK();
  __set_PRIMASK(1);
  return primask;
}

static inline void restore_irq(uint32_t primask) {
  __set_PRIMASK(primask);
}


#endif // _MODULE_H_
