#ifndef _MODULE_H_
#define _MODULE_H_

//#include <limits.h>

#include "stm32f4xx_ll_cortex.h"

#define SUCCESS               0
#define MOD_ERR_INIT         -1
#define MOD_ERR_ARGS         -2


// Get size of an array.
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* // Increment a uint16_t, saturating at the maximum value. */
/* #define INC_SAT_U16(a) do { (a) += ((a) == UINT16_MAX ? 0 : 1); } while (0) */

/* // Clamp a numeric value between a lower and upper limit, inclusive. */
/* #define CLAMP(a, low, high) ((a) <= (low) ? (low) : ((a) > (high) ? (high) : (a))) */

/* // Concat tokens */
/* #define CONCAT_TOKEN(a, b) a ## b */

/* // Concat tokens after expansion */
/* #define CONCAT_X_TOKEN(a, b) CONCAT_TOKEN(a, b) */

/* #define CRIT_START() __disable_irq() */
/* #define CRIT_END() __disable_irq() */

/* #define CRIT_STATE_VAR uint32_t _primask_save */
/* #define CRIT_BEGIN_NEST()                                   \ */
/*     do {                                                    \ */
/*         _primask_save = __get_PRIMASK(); __set_PRIMASK(1);  \ */
/*     } while (0) */
/* #define CRIT_END_NEST() do { __set_PRIMASK(_primask_save); } while (0) */

#endif // _MODULE_H_
