#include "gpio.h"
#include "module.h"
#include "log.h"

static int32_t log_level = LOG_LEVEL_TRACE;

uint32_t port_to_periph_adress(gpio_port *port);

int32_t gpio_init(const struct gpio_in inputs[],
                  const uint8_t num_inputs,
                  const struct gpio_out outputs[],
                  const uint8_t num_outputs) {
  uint32_t idx;
  const struct gpio_in *input;
  const struct gpio_out *output;

  log_info("Initializing inputs");
  for (idx = 0; idx < num_inputs; idx++) {
    input = &inputs[idx];

    log_trace("Initializing input %s", input->name);

    if (input == 0)
      return MOD_ERR_INIT;

    LL_GPIO_SetPinPull(input->port, input->pin, input->pull);
    LL_GPIO_SetPinMode(input->port, input->pin, LL_GPIO_MODE_INPUT);
  };

  log_info("Initializing outputs");
  for (idx = 0; idx < num_outputs; idx++) {
    output = &outputs[idx];

    log_trace("Initializing output %s", output->name);

    if (output == 0)
      return MOD_ERR_INIT;

    LL_GPIO_SetPinSpeed(output->port, output->pin, output->speed);
    LL_GPIO_SetPinOutputType(output->port, output->pin, output->output_type);
    LL_GPIO_SetPinPull(output->port, output->pin, output->pull);
    LL_GPIO_SetPinMode(output->port, output->pin, LL_GPIO_MODE_OUTPUT);
    gpio_set(output, output->init_value);
  }

  log_info("Initialization complete");

  return SUCCESS;
}

int32_t gpio_start(const struct gpio_in inputs[],
                   const uint8_t num_inputs,
                   const struct gpio_out outputs[],
                   const uint8_t num_outputs) {
  uint8_t idx;
  uint32_t ports = 0;
  const struct gpio_in *in;
  const struct gpio_out *out;

  for (idx = 0; idx < num_inputs; idx++) {
    in = &inputs[idx];
    uint32_t addr = port_to_periph_adress(in->port);

    if (addr == 0)
      return MOD_ERR_ARGS;

    ports |= addr;
  }

  for (idx = 0; idx < num_outputs; idx++) {
    out = &outputs[idx];

    uint32_t addr = port_to_periph_adress(out->port);

    if (addr == 0)
      return MOD_ERR_ARGS;

    ports |= addr;
  }

  if (ports != 0)
    LL_AHB1_GRP1_EnableClock(ports);

  return SUCCESS;
}

uint32_t port_to_periph_adress(gpio_port *port) {
  if (port == GPIOA)
    return LL_AHB1_GRP1_PERIPH_GPIOA;

  if (port == GPIOB)
    return LL_AHB1_GRP1_PERIPH_GPIOB;

  if (port == GPIOC)
    return LL_AHB1_GRP1_PERIPH_GPIOC;

  if (port == GPIOD)
    return LL_AHB1_GRP1_PERIPH_GPIOD;

  if (port == GPIOE)
    return LL_AHB1_GRP1_PERIPH_GPIOE;

  if (port == GPIOH)
    return LL_AHB1_GRP1_PERIPH_GPIOH;

  return 0;
}

int32_t gpio_get(const struct gpio_in *in) {
  log_trace("in: %s", in->name);

  if (in == 0)
    return MOD_ERR_ARGS;

  int32_t value = LL_GPIO_IsInputPinSet(in->port, in->pin) ^ in->reversed;

  return value;
}

int32_t gpio_get_out(const struct gpio_out *out) {
  log_trace("out: %s", out->name);

  if (out == 0)
    return MOD_ERR_ARGS;

  int32_t value = LL_GPIO_IsOutputPinSet(out->port, out->pin) ^ out->reversed;

  return value;
}

int32_t gpio_set(const struct gpio_out *out, uint32_t value) {
  log_trace("out: %s, value: %d", out->name, value);

  if (out == 0)
    return MOD_ERR_ARGS;

  if (value ^ out->reversed) {
    LL_GPIO_SetOutputPin(out->port, out->pin);
  } else {
    LL_GPIO_ResetOutputPin(out->port, out->pin);
  }

  return SUCCESS;
}
