#include "uart_diag.h"
#include "stm32f3xx.h"
#include <stdio.h>

uint32_t adc_raw;
float adc_value;
uint16_t samples[2];

void uart_pin_init(void)
{
    // UART2 Pins clock: USART2_TX=PA2, USART2_RX=PA3 - AF7 (from table)
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	__IO uint32_t tmpreg = RCC->APB1ENR & (~RCC_APB1ENR_USART2EN);
	(void)tmpreg;

	// Clock of GPIOA has been enabled

	GPIOA->MODER |= GPIO_MODER_MODER2_1; //PA2 to AF
	GPIOA->MODER |= GPIO_MODER_MODER3_1; // PA3 to AF

	const uint32_t AF7 = 7U; // 0000 0111
	const uint32_t AFR2 = 8U; // 8 positions
	const uint32_t AFR3 = 12U; // 12 positions

	GPIOA->AFR[0] |= (AF7 << AFR2); // shifting in AFRLow
	GPIOA->AFR[0] |= (AF7 << AFR3); //shifting in AFRLow
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2; //push pull
	GPIOA->OTYPER |= GPIO_OTYPER_OT_3; //open drain

}

void uart_setup(void)
{
        // Program the M bits in USART_CR1 to define the word length = 8 bit length
	USART2->CR1 &= ~USART_CR1_M;        

	const uint32_t BAUD = 9600UL;
	const uint32_t FREQ = 72000000UL;
	USART2->BRR = FREQ / BAUD;           // 9600 equals 0x00001d4c

	// Program the number of stop bits in USART_CR2 = 1st stop bit
	USART2->CR2 &= ~USART_CR2_STOP; 
	// PCE: Parity control disabled
	USART2->CR1 &= ~USART_CR1_PCE;    

	// usart enable
	USART2->CR1 |= USART_CR1_UE;      

	// Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
	// place. Configure the DMA register as explained in multibuffer communication.
	USART2->CR3 |= USART_CR3_DMAT;  
	
	// Set the TE bit in USART_CR1 to send an idle frame as first transmission
	USART2->CR1 |= USART_CR1_TE;


}

void uart_init(void)
{
    uart_pin_init();
    uart_setup();
    
    // Enable clocks of USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	__IO uint32_t tmpreg = RCC->APB1ENR & (~RCC_APB1ENR_USART2EN);
    (void)tmpreg;



}

void uart_send(void)
{
    //formula needed;

	adc_value = (float)adc_raw / (4096) * 3.3f;
	//char txt[100] = { 0 };

	//snprintf(txt, sizeof(txt),"voltage = %f\r\n", adc_value);

	//char* point = &txt[0];

	//fprintf(stderr, "%.2f V\r\n",adc_value);

	fprintf(stderr, "Ch1 = %d   Ch2 = %d \r\n",samples[0],samples[1]);




}
