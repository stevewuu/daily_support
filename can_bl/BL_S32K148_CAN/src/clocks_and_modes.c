/*
 * clocks_and_modes.c              Copyright NXP 2016
 * Description: Example clock and mode functions
 * 2015 Feb 22 S Mihalik/ O Romero - initial version
 * 2016 Oct 15 S.M. / O.R. - Updated PLL per S32K1XX data sheet rev 1
 * 2018 May 16 H.W - updated OSC for S32K Start Kit
 */

#include "device_registers.h"           /* include peripheral declarations S32K */
#include "clocks_and_modes.h"

/*
 * S32K148 Start Kit use 16MHz OSC
 * change Range = 3,
 */
 /**
 *  \brief SOSC configure - 16MHz
 *  
 *  \return None
 *  
 *  \details SCG SOSC configure
 *           S32K148 Start Kit use 16MHz OSC
 *           change Range = 3,
 */
void SOSC_init_16MHz(void) 
{
    SCG->SOSCDIV=0x00000101;  /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
    SCG->SOSCCFG=0x00000034;  /* Range=3: Medium freq (SOSC betw 8MHz-40MHz)*/
                              /* HGO=0:   Config xtal osc for low power */
                              /* EREFS=1: Input is external XTAL */ /*18-5-16*/
    while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */
    SCG->SOSCCSR=0x00000001;  /* LK=0:          SOSCCSR can be written */
                              /* SOSCCMRE=0:    OSC CLK monitor IRQ if enabled */
                              /* SOSCCM=0:      OSC CLK monitor disabled */
                              /* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
                              /* SOSCLPEN=0:    Sys OSC disabled in VLP modes */
                              /* SOSCSTEN=0:    Sys OSC disabled in Stop modes */
                              /* SOSCEN=1:      Enable oscillator */
    while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */
}

 /**
 *  \brief SPLL_init_160MHz
 *  
 *  \return None
 *  
 *  \details SPLL PREDIV = 1
 */
void SPLL_init_160MHz(void) 
{
    while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
    SCG->SPLLCSR = 0x00000000;  /* SPLLEN=0: SPLL is disabled (default) */
    SCG->SPLLDIV = 0x00000302;  /* SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */
    SCG->SPLLCFG = 0x00180100;  /* PREDIV=1: Divide SOSC_CLK by 0+1=1 */
                                /* MULT=24:  Multiply sys pll by 4+24=40 */
                                /* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */
    while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
    SCG->SPLLCSR = 0x00000001; /* LK=0:        SPLLCSR can be written */
                               /* SPLLCMRE=0:  SPLL CLK monitor IRQ if enabled */
                               /* SPLLCM=0:    SPLL CLK monitor disabled */
                               /* SPLLSTEN=0:  SPLL disabled in Stop modes */
                               /* SPLLEN=1:    Enable SPLL */
    while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
}
/**
 *  \brief NormalRUNmode_80MHz
 *  
 *  \return None
 *  
 *  \details Change to normal RUN mode with 8MHz SOSC, 80 MHz PLL
 */
void NormalRUNmode_80MHz (void) 
{  
    SCG->RCCR=SCG_RCCR_SCS(6)      /* PLL as clock source*/
      |SCG_RCCR_DIVCORE(0b01)      /* DIVCORE= 2, Core clock = 160/2 MHz = 80 MHz*/
      |SCG_RCCR_DIVBUS(0b01)       /* DIVBUS = 2, bus clock = 40 MHz*/
      |SCG_RCCR_DIVSLOW(0b10);     /* DIVSLOW = 4, SCG slow, flash clock= 20 MHz*/
    if ((SCG->CSR & SCG_CSR_SCS_MASK >> SCG_CSR_SCS_SHIFT ) != 6) {}
                                   /* Wait for sys clk src = SPLL */
}
