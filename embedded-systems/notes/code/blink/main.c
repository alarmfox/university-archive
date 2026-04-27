#include <stdint.h>
#define GPIOE_BASE 0x48001000
#define RCC_BASE 0x40021000

#define GPIOE_MODER ((volatile uint32_t *)(GPIOE_BASE + 0x00))
#define GPIOE_ODR ((volatile uint32_t *)(GPIOE_BASE + 0x14))
#define RCC_AHBENR ((volatile uint32_t *)(RCC_BASE + 0x14))

int main(void) {
  *(RCC_AHBENR) |= (1 << 21);
  volatile uint32_t dummy;
  dummy = *(RCC_AHBENR);
  dummy = *(RCC_AHBENR);
  *(GPIOE_MODER) |= (1 << 30) | (1 << 28) | (1 << 26);
  while (1) {
    *(GPIOE_ODR) ^= 0x0000ff00;
    for (uint32_t i = 0; i < 1000000; i++)
      ;
  }

  return 0;
}
