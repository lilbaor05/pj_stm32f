#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

void gpio_init(void);
void btn_init(void);
void led_init(void);

void led_on(void);
void led_off(void);

uint8_t read_btn(void);

#endif /* GPIO_H_ */