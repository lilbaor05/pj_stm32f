#include "stm32f1xx.h"
#include <stdint.h>

/*
 * Candidate buttons:
 * WKUP : PA0
 * BTN1 : PB1 (repo GrapeTech xác nhận)
 * thử thêm PB2 cho nút còn lại
 */

static void GPIO_Init(void)
{
    /* GPIOA + GPIOB clock */
    RCC->APB2ENR |= (1U << 2);   /* IOPAEN */
    RCC->APB2ENR |= (1U << 3);   /* IOPBEN */

    /* PA0 = Input Floating */
    GPIOA->CRL &= ~(0xFU << 0);
    GPIOA->CRL |=  (0x4U << 0);

    /* PB1 = Input Floating */
    GPIOB->CRL &= ~(0xFU << 4);
    GPIOB->CRL |=  (0x4U << 4);

    /* PB2 = Input Floating */
    GPIOB->CRL &= ~(0xFU << 8);
    GPIOB->CRL |=  (0x4U << 8);

    /* PB10 = Output Push-Pull 2 MHz */
    GPIOB->CRH &= ~(0xFU << 8);
    GPIOB->CRH |=  (0x2U << 8);

    /*
     * LED RED trên sample board:
     * PB10 = 1 -> OFF
     * PB10 = 0 -> ON
     */
    GPIOB->BSRR = (1U << 10);
}

int main(void)
{
    uint32_t pa0_initial;
    uint32_t pb1_initial;
    uint32_t pb2_initial;

    GPIO_Init();

    /* Lưu trạng thái các nút lúc vừa khởi động */
    pa0_initial = GPIOA->IDR & (1U << 0);
    pb1_initial = GPIOB->IDR & (1U << 1);
    pb2_initial = GPIOB->IDR & (1U << 2);

    while (1)
    {
        uint32_t changed = 0U;

        if ((GPIOA->IDR & (1U << 0)) != pa0_initial)
        {
            changed = 1U;
        }

        if ((GPIOB->IDR & (1U << 1)) != pb1_initial)
        {
            changed = 1U;
        }

        if ((GPIOB->IDR & (1U << 2)) != pb2_initial)
        {
            changed = 1U;
        }

        if (changed)
        {
            /* PB10 = 0 -> RED ON */
            GPIOB->BRR = (1U << 10);
        }
        else
        {
            /* PB10 = 1 -> RED OFF */
            GPIOB->BSRR = (1U << 10);
        }
    }
}