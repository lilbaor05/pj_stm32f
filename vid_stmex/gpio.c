#include "gpio.h"
//#include "S32K144.h"


void gpio_init(void)
{
    /* Cap clock cho PORTC */
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC(1);

    /* Cap clock cho PORTD */
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC(1);

    /* PTC12 la GPIO */
    IP_PORTC->PCR[12] |= PORT_PCR_MUX(0b001);

    /* PTD0 la GPIO */
    IP_PORTD->PCR[0] |= PORT_PCR_MUX(0b001);
}


void btn_init(void)
{
    /* PTC12 la input */
    IP_PTC->PDDR &= ~(1U << 12);

    /* PS = 0: pull-down */
    IP_PORTC->PCR[12] &= ~(1U << 0);

    /* PE = 1: enable pull */
    IP_PORTC->PCR[12] |= (1U << 1);
}


void led_init(void)
{
    /* PTD0 la output */
    IP_PTD->PDDR |= (1U << 0);

    /* LED OFF - LED tren board active-low */
    IP_PTD->PDOR |= (1U << 0);
}


void led_on(void)
{
    /* Output LOW -> LED ON */
    IP_PTD->PDOR &= ~(1U << 0);
}


void led_off(void)
{
    /* Output HIGH -> LED OFF */
    IP_PTD->PDOR |= (1U << 0);
}


uint8_t read_btn(void)
{
    return ((IP_PTC->PDIR >> 12U) & 0x01U);
}