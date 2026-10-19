#include <gpio.h>


#define DELAY_TIME 3000


void delay(int time) {
    volatile uint i;
    for(i = 0; i< time; i++);
}


void main() {
    //cap clock cho portC
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_(1);
    //cap clock cho portD
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_(1);
    //chon chuc nang cho PTC12( la gpio)
    IP_PORTC->PCR[12] = PORT_PCR_MUX(0b001);
    //chon chuc nang cho PTD0 (la gpio)
    IP_PORTD->PCR[0] = PORT_PCR_MUX(0b001);
    // cau hinh input cho PTC12
    IP_PTC->PDDR &= ~(1U << 12); // 12 thi dich 12 lan
    //cau hinh ouput cho PTD0
    IP_PTD->PDDR |= (1U << 0);
    //cau hinh pull down cho PTC12
    IP_PORTC->PCR[12] &=~(1U<<0); // xoa bit 0
    //cau hinh pull up
    IP_PORTC->PCR[12] |=(1U<<1);
    //turn on PTD0
    IP_PTD->PDOR |= (1U << 0);
    //turn off PTD0
    IP_PTD->PDOR &= ~(1U << 0);
    while(1) {
        IP_PTD->PDOR |=(1U <<0);
        delay(DELAY_TIME);
        IP_PTD->PDOR &= ~(1U << 0);
        delay(DELAY_TIME);  
    }
}