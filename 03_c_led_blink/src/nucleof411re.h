#ifndef NUCLEOF411RE_HEADER
#define NUCLEOF411RE_HEADER

#include <stdint.h>

#ifdef __cplusplus
#define R_I_ volatile
#else
#define R_I_ volatile const
#endif
#define R__O volatile
#define R_IO volatile
#define R___ volatile

typedef struct {
    R_IO uint32_t moder;
    R_IO uint32_t otyper;
    R_IO uint32_t ospeedr;
    R_IO uint32_t pupdr;
    R_I_ uint16_t idr;
    R___ uint16_t reserved_idr;
    R_IO uint16_t odr;
    R___ uint16_t reserved_odr;
    R__O uint32_t bssr;
    R_IO uint32_t lckr;
    R_IO uint32_t afrl;
    R_IO uint32_t afrh;
} GPIO;

typedef struct {
    R_IO uint32_t cr;
    R_IO uint32_t pllcfgr;
    R_IO uint32_t cfgr;
    R_IO uint32_t cir;
    R_IO uint32_t ahb1rstr;
    R_IO uint32_t ahb2rstr;
    R___ uint32_t reserved0;
    R___ uint32_t reserved1;
    R_IO uint32_t apb1rstr;
    R_IO uint32_t apb2rstr;
    R___ uint32_t reserved2;
    R___ uint32_t reserved3;
    R_IO uint32_t ahb1enr;
    R_IO uint32_t ahb2enr;
    R___ uint32_t reserved4;
    R___ uint32_t reserved5;
    R_IO uint32_t apb1enr;
    R_IO uint32_t apb2enr;
    R___ uint32_t reserved6;
    R___ uint32_t reserved7;
    R_IO uint32_t ahb1lpenr;
    R_IO uint32_t ahb2lpenr;
    R___ uint32_t reserved8;
    R___ uint32_t reserved9;
    R_IO uint32_t apb1lpenr;
    R_IO uint32_t apb2lpenr;
    R___ uint32_t reserved10;
    R___ uint32_t reserved11;
    R_IO uint32_t bdcr;
    R_IO uint32_t csr;
    R___ uint32_t reserved12;
    R___ uint32_t reserved13;
    R_IO uint32_t sscgr;
    R_IO uint32_t plli2scfgr;
    R___ uint32_t reserved14;
    R_IO uint32_t dckcfgr;
} RCC;

typedef struct {
    R_IO uint32_t ctrl;
    R_IO uint32_t load;
    R_IO uint32_t val;
    R_IO uint32_t calib;
} SysTickSTK;

#define STM32_M4_BASE (0xE000E000UL)
#define AHB1PERIPH_BASE (0x40020000UL)

#define GPIOA_BASE (AHB1PERIPH_BASE + 0x0000UL)
#define RCC_BASE (AHB1PERIPH_BASE + 0x3800UL)
#define SYSTICK_STK_BASE (STM32_M4_BASE + 0x10UL)

// NOLINTNEXTLINE
static GPIO *const NUCLEO_GPIOA = (GPIO *)(GPIOA_BASE);
// NOLINTNEXTLINE
static RCC *const NUCLEO_RCC = (RCC *)(RCC_BASE);
// NOLINTNEXTLINE
static SysTickSTK *const NUCLEO_SYSTICK = (SysTickSTK *)(SYSTICK_STK_BASE);
#endif /* ifndef NUCLEOF411RE_HEADER */
