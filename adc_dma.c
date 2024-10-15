#include "stm32f3xx.h"
#include "adc_dma.h"

extern uint32_t adc_raw;
extern uint16_t samples[2];

void gpio_setup(void)                           //input on pin PA0 and PA1
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;          // ABH enabling and GPIO A enabling
    __IO uint32_t tmpreg = RCC->AHBENR & (~RCC_AHBENR_GPIOAEN); 
    (void)tmpreg;
    GPIOA->MODER |= GPIO_MODER_MODER0;          // 11: Analog mode - for PA0
    GPIOA->MODER |= GPIO_MODER_MODER1;          // 11: Analog mode - for APA1


    //some MC require AF and push-pull setiinhgs
}

void adc_setup(void) // ADC1 and 2 
{
    ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;    // 0b01 // Set ADC clock ???

    ADC1->CFGR |= ADC_CFGR_DMAEN;               // DMA mode enable
    //ADC1->CFGR &= ~ADC_CFGR_DMACFG;             // DMA One Shot Mode selected

    ADC1->CFGR |= ADC_CFGR_DMACFG;              //1: DMA Circular Mode selected

    ADC1->SMPR1 |= ADC_SMPR1_SMP1_0|ADC_SMPR1_SMP1_1|ADC_SMPR1_SMP1_2; // max sample for ch1 ???
    ADC1->SMPR1 |= ADC_SMPR1_SMP2_0|ADC_SMPR1_SMP2_1|ADC_SMPR1_SMP2_2;

    ADC1->SQR1 |= ADC_SQR1_L_0;                 // 2 conversions
    ADC1->SQR1 |= ADC_SQR1_SQ1_0;               // channel 1st selected!!!= number 1 into SQ1
    ADC1->SQR1 |= 2 << 12;     //channel 2st selected = nuber 2 into SQ2

    //ADC1->CFGR &= ~ADC_CFGR_CONT;                // 0: Continuous conversion mode OFF

    ADC1->CFGR |= ADC_CFGR_DISCEN;                  //discont mode ??? SCAN
    ADC1->CR &= ~ADC_CR_ADEN;                   // disable the ADC before CAL.
    ADC1->CR |= ADC_CR_ADVREGEN_0;              // 01: ADC Voltage regulator enabled.

    ADC1->CR |= ADC_CR_ADCAL;                   // start ADC calibration cycle
    while (ADC1->CR & ADC_CR_ADCAL);            // wait for calibration to complete

    ADC1->CR |= ADC_CR_ADEN;                    // enable the ADC.
    while (!(ADC1->ISR & ADC_ISR_ADRDY));       // check if enabled
  
}

void dma_setup(void)
{
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));  // 1. Set the peripheral register address in the DMA_CPARx register
	DMA1_Channel1->CMAR = (uint32_t)samples;        // 2. Set the memory address in the DMA_CMARx register
	DMA1_Channel1->CNDTR = 2;                       // 3. Configure the total number of data to be transferred in the DMA_CNDTRx register
	DMA1_Channel1->CCR |= DMA_CCR_PL_1;             // 00: ensuring it is Low onfigure the channel priority using the PL[1:0]
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;          // 01: size 16bit
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;          // 01: size 16bit
	DMA1_Channel1->CCR |= DMA_CCR_MINC;             // 1 : memory increment enabled
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;             // 1: Circ mode enabled
	DMA1_Channel1->CCR |= DMA_CCR_EN;           // Activate the channel in the DMA control register

}

void take_sample(void)
{
    
    ADC1->CR |= ADC_CR_ADSTART;                 //start conversion
    //adc_raw = ADC1->DR;
    for(volatile long i = 0; i < 100000; i++)
    {

	}
    ADC1->CR |= ADC_CR_ADSTP;                   //stop conversion

    // control DMA transmission!
}
