#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

//
// Mappings for STM32F401.
//
#define GPIO_PORT_A (GPIOA)
#define GPIO_PORT_B (GPIOB)
#define GPIO_PORT_C (GPIOC)
#define GPIO_PORT_D (GPIOD)
#define GPIO_PORT_E (GPIOE)
#define GPIO_PORT_F (GPIOF)
#define GPIO_PORT_G (GPIOG)
#define GPIO_PORT_H (GPIOH)

#define GPIO_PIN_0  (LL_GPIO_PIN_0)
#define GPIO_PIN_1  (LL_GPIO_PIN_1)
#define GPIO_PIN_2  (LL_GPIO_PIN_2)
#define GPIO_PIN_3  (LL_GPIO_PIN_3)
#define GPIO_PIN_4  (LL_GPIO_PIN_4)
#define GPIO_PIN_5  (LL_GPIO_PIN_5)
#define GPIO_PIN_6  (LL_GPIO_PIN_6)
#define GPIO_PIN_7  (LL_GPIO_PIN_7)
#define GPIO_PIN_8  (LL_GPIO_PIN_8)
#define GPIO_PIN_9  (LL_GPIO_PIN_9)
#define GPIO_PIN_10 (LL_GPIO_PIN_10)
#define GPIO_PIN_11 (LL_GPIO_PIN_11)
#define GPIO_PIN_12 (LL_GPIO_PIN_12)
#define GPIO_PIN_13 (LL_GPIO_PIN_13)
#define GPIO_PIN_14 (LL_GPIO_PIN_14)
#define GPIO_PIN_15 (LL_GPIO_PIN_15)

#define GPIO_PULL_NO   (LL_GPIO_PULL_NO)
#define GPIO_PULL_UP   (LL_GPIO_PULL_UP)
#define GPIO_PULL_DOWN (LL_GPIO_PULL_DOWN)

#define GPIO_SPEED_FREQ_LOW       (LL_GPIO_SPEED_FREQ_LOW)
#define GPIO_SPEED_FREQ_MEDIUM    (LL_GPIO_SPEED_FREQ_MEDIUM)
#define GPIO_SPEED_FREQ_HIGH      (LL_GPIO_SPEED_FREQ_HIGH)
#define GPIO_SPEED_FREQ_VERY_HIGH (LL_GPIO_SPEED_FREQ_VERY_HIGH)

#define GPIO_OUTPUT_PUSHPULL  (LL_GPIO_OUTPUT_PUSHPULL)
#define GPIO_OUTPUT_OPENDRAIN (LL_GPIO_OUTPUT_OPENDRAIN)

typedef GPIO_TypeDef gpio_port;

struct gpio_in {
    const char* const name;
    gpio_port* const port;
    const uint32_t pin;
    const uint32_t pull;
    const uint8_t reversed;
};

struct gpio_out {
    const char* const name;
    gpio_port* const port;
    const uint32_t pin;
    const uint32_t pull;
    const uint8_t reversed;
    const uint8_t init_value;
    const uint32_t speed;
    const uint32_t output_type;
};

int32_t gpio_init(const struct gpio_in inputs[],
                  const uint8_t num_inputs,
                  const struct gpio_out outputs[],
                  const uint8_t num_outputs);

int32_t gpio_start(const struct gpio_in inputs[],
                   const uint8_t num_inputs,
                   const struct gpio_out outputs[],
                   const uint8_t num_outputs);

int32_t gpio_get(const struct gpio_in *in);
int32_t gpio_get_out(const struct gpio_out *out);

int32_t gpio_set(const struct gpio_out* out, uint32_t value);

#endif // _GPIO_H_
