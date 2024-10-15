#include "stm32f3xx.h"
#include "adc_dma.h"
#include "uart_diag.h"

static void clock_init(void);

void myDelay(void);

int main()
{
    clock_init();

	gpio_setup();
	adc_setup();
	dma_setup();
	uart_init();


    while(1)
    {
		//myDelay();
		take_sample();
		uart_send();
	
    }
    return 0;
}

static void clock_init(void)
{
	RCC->CR   |= RCC_CR_HSEON; 				// set HSE ON for external oscilator 8 MHz
	while((RCC->CR & RCC_CR_HSERDY) == 0u)	// wait 6 cycles to HSE to stabilise
	{

	}

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN ; 	// on 
    __IO uint32_t tmpreg = RCC->AHBENR & (~RCC_AHBENR_GPIOAEN);
    (void)tmpreg;

    GPIOA->MODER |= GPIO_MODER_MODER5_0;  	// 0100 0000 0000 we put that value into moder

    /*FLASH*/
	FLASH->ACR|= FLASH_ACR_PRFTBE;			// enabling FLASH prefatch buffer
	FLASH->ACR &= ~FLASH_ACR_LATENCY;		// set all bits to 0 (to ensure)
	FLASH->ACR|= FLASH_ACR_LATENCY_2;		// Two wait sates, if 48 < HCLK ≤ 72 MHz
	/*FLASH*/

	RCC->AHBENR |= RCC_AHBENR_ADC12EN;      // ADC enable (adc 1 and adc 2)
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;		// DMA1 enable
    //RCC->CFGR |= RCC_CFGR_PPRE1_2;          // APB1 div 2 - serial works on 9600 
    RCC->CFGR |= RCC_CFGR_PLLMUL9;  		// multiplicator 9 , input 8*9=72MHz
	RCC->CFGR |= RCC_CFGR_PLLSRC;   		// PLL entry clock source, external selected as PLL input clock

	RCC->CR	  |= RCC_CR_PLLON; 				// start the PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0u)	// wait for PLL is ready
	{

	}
	
	RCC->CFGR &= ~RCC_CFGR_SW; 				// System clock switch to external
	RCC->CFGR |= RCC_CFGR_SW_PLL; 			// select PLL as system clock
	
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { } // wait for switch status
}

void myDelay(void)
{
    for(volatile long i = 0; i < 1000000; i++)
    {

	}
	GPIOA->ODR ^= GPIO_ODR_5; 
    

}
