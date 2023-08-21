#include <stdbool.h>

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"

#include "module.h"
#include "gpio.h"

int main(void) {
  const struct gpio_in button = {
    .name = "button",
    .port = GPIO_PORT_A,
    .pin = GPIO_PIN_0,
    .pull = GPIO_PULL_UP,
    .reversed = true
  };

  const struct gpio_out led = {
    .name = "led",
    .port = GPIO_PORT_C,
    .pin = GPIO_PIN_13,
    .pull = GPIO_PULL_NO,
    .output_type = GPIO_OUTPUT_PUSHPULL,
    .init_value = false,
    .speed = GPIO_SPEED_FREQ_LOW,
    .reversed = true
  };

  const struct gpio_in inputs[] = {button};
  const uint8_t num_inputs = ARRAY_SIZE(inputs);

  const struct gpio_out outputs[] = {led};
  const uint8_t num_outputs = ARRAY_SIZE(outputs);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOC);

  gpio_init(inputs, num_inputs, outputs, num_outputs);

  //  gpio_start(inputs, num_inputs, outputs, num_outputs);

  while (1) {
    int32_t value = gpio_get(&button);

    if (value == 1) {
      gpio_set(&led, true);
    } else {
      gpio_set(&led, false);
    }
  }

  return 0;
}
