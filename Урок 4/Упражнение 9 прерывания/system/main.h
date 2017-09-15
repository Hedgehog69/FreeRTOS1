/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#if defined  (PLL_SOURCE_HSI)
#define PLL_M      16
#else
#define PLL_M      8
#endif
#define PLL_N      360

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

/******************************************************************************/

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
/******************************************************************************/