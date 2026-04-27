void init(void);
void Default_Handler(void);

// The following are 'declared' in the linker script
extern unsigned char INIT_DATA_VALUES;
extern unsigned char INIT_DATA_START;
extern unsigned char INIT_DATA_END;
extern unsigned char BSS_START;
extern unsigned char BSS_END;
extern void isr_spi1(void);
// the section "vectors" is placed at the beginning of flash
// by the linker script
const void *Vectors[] __attribute__((section(".isr_vector"))) = {
    (void *)0x2000a000, /* Top of stack */
    init,               /* Reset Handler */
    Default_Handler,    /* NMI */
    Default_Handler,    /* Hard Fault */
    0,                  /* Reserved */
    0,                  /* Reserved */
    0,                  /* Reserved */
    0,                  /* Reserved */
    0,                  /* Reserved */
    0,                  /* Reserved */
    0,                  /* Reserved */
    Default_Handler,    /* SVC */
    0,                  /* Reserved */
    0,                  /* Reserved */
    Default_Handler,    /* PendSV */
    Default_Handler,    /* SysTick */
                        /* External interrupt handlers follow */
    Default_Handler,    /* WWDG */
    Default_Handler,    /* PVD */
    Default_Handler,    /* RTC */
    Default_Handler,    /* FLASH */
    Default_Handler,    /* RCC */
    Default_Handler,    /* EXTI0_1 */
    Default_Handler,    /* EXTI2_3 */
    Default_Handler,    /* EXTI4_15 */
    Default_Handler,    /* TSC */
    Default_Handler,    /* DMA_CH1 */
    Default_Handler,    /* DMA_CH2_3 */
    Default_Handler,    /* DMA_CH4_5 */
    Default_Handler,    /* ADC_COMP */
    Default_Handler,    /* TIM1_BRK_UP_TRG_COM */
    Default_Handler,    /* TIM1_CC */
    Default_Handler,    /* TIM2 */
    Default_Handler,    /* TIM3 */
    Default_Handler,    /* TIM6_DAC */
    Default_Handler,    /* RESERVED */
    Default_Handler,    /* TIM14 */
    Default_Handler,    /* TIM15 */
    Default_Handler,    /* TIM16 */
    Default_Handler,    /* TIM17 */
    Default_Handler,    /* I2C1 */
    Default_Handler,    /* I2C2 */
    Default_Handler,    /* SPI1 */
    Default_Handler,    /* SPI2 */
    Default_Handler,    /* USART1 */
    Default_Handler,    /* USART2 */
    Default_Handler,    /* RESERVED */
    Default_Handler,    /* CEC */
    Default_Handler     /* RESERVED */
};

int main(void);
void init() {
  // do global/static data initialization
  unsigned char *src;
  unsigned char *dest;
  unsigned len;
  src = &INIT_DATA_VALUES;
  dest = &INIT_DATA_START;
  len = &INIT_DATA_END - &INIT_DATA_START;
  while (len--)
    *dest++ = *src++;
  // zero out the uninitialized global/static variables
  dest = &BSS_START;
  len = &BSS_END - &BSS_START;
  while (len--)
    *dest++ = 0;
  main();
}

void Default_Handler() {
  while (1)
    ;
}
