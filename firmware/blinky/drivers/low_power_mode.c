#include "sys_common.h"
#include "rti.h"
#include "sys_mpu.h"
#include "reg_flash.h"
#include "system.h"

/***********************************************
 * Enter the doze mode:
 * - All the interrupts, besides main OSC, are disabled
 * - All the clock domains, besides RTICLK, are turned off
 * - Flash module is set to sleep mode
 *
 **********************************************/
void enter_doze(void)
{
    /* Set up flash pump active grace period as 7 HCLK/16 cycles */
    flashWREG->FPAC2 = 0x7;

    /* Set flash bank fallback modes to "sleep" */
    flashWREG->FBFALLBACK = (uint32)((uint32)SYS_SLEEP << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_SLEEP << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_SLEEP << 0U); /* BANK 0 */

    /* Disable oscillator monitoring to prevent detection of oscillator failure */
    systemREG1->CLKTEST = 0x010A0000;

    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC |= 0x4;

    /** - Setup RTI compare 1 value. This value is compared with the selected free running counter. */
    rtiREG1->CMP[1U].COMPx = 400000U;

    /** - Setup RTI compare 1 update value. This value is added to the compare 1 value on each compare match. */
    rtiREG1->CMP[1U].UDCPx = 400000U;

    /** - Clear all pending interrupts */
    rtiREG1->INTFLAG = 0x0007000FU;

    /** - Disable all interrupts */
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /** - Enable RTI Compare 1 Interrupt **/
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;

    /* Turn off all clock sources except LF LPO */
    systemREG1->CSDISSET = 0xFE;

    /* Turn off all clock domains except RTICLK */
    systemREG1->CDDISSET = 0xFFBF;

    asm(" WFI");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
}

/***********************************************
 * Enter the snooze mode:
 * - All the interrupts, besides RTI, are disabled
 * - All the clock domains, besides RTICLK, are turned off
 * - Flash module is set to sleep mode
 *
 **********************************************/
void enter_snooze(void)
{
    /* Set up flash pump active grace period as 7 HCLK/16 cycles */
    flashWREG->FPAC2 = 0x7;

    /* Set flash bank fallback modes to "sleep" */
    flashWREG->FBFALLBACK = (uint32)((uint32)SYS_SLEEP << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_SLEEP << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_SLEEP << 0U); /* BANK 0 */

    /* Disable oscillator monitoring to prevent detection of oscillator failure */
    systemREG1->CLKTEST = 0x010A0000;

    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC |= 0x4;

    /** - Setup RTI compare 1 value. This value is compared with the selected free running counter. */
    rtiREG1->CMP[1U].COMPx = 400000U;

    /** - Setup RTI compare 1 update value. This value is added to the compare 1 value on each compare match. */
    rtiREG1->CMP[1U].UDCPx = 400000U;

    /** - Clear all pending interrupts */
    rtiREG1->INTFLAG = 0x0007000FU;

    /** - Disable all interrupts */
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /** - Enable RTI Compare 1 Interrupt **/
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;

    /* Turn off all clock sources except LF LPO */
    systemREG1->CSDISSET = 0xEF;

    /* Turn off all clock domains except RTICLK */
    systemREG1->CDDISSET = 0xFFBF;

    asm(" WFI");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
}

/***********************************************
 * Wake the system up from snooze mode:
 * - Restore clock sources and domains
 * - Restore Flash module
 * - Restore interrupts
 *
 **********************************************/
void post_wakeup(void)
{
    /* Restart main oscillator, LF LPO, and HF LPO */
    systemREG1->CSDISCLR = 0x31;

    /* Configure PLLs based on original configuration */
    systemREG1->PLLCTL1 =  (uint32)0x00000000U
                        | (uint32)0x20000000U
                        | (uint32)((uint32)0x1FU << 24U)
                        | (uint32)0x00000000U
                        | (uint32)((uint32)(6U - 1U) << 16U)
                        | (uint32)((uint32)(165U - 1U) << 8U);

    systemREG1->PLLCTL2 = (uint32)((uint32)255U << 22U)
                        | (uint32)((uint32)7U << 12U)
                        | (uint32)((uint32)(2U - 1U) << 9U)
                        | (uint32)61U;

    /* Restart all other clock sources. All clock domains are enabled automatically. */
    systemREG1->CSDISCLR = 0xFF;

    /* Restore flash bank fallback modes to "active" */
    flashWREG->FBFALLBACK = (uint32)((uint32)SYS_ACTIVE << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_ACTIVE << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_ACTIVE << 0U); /* BANK 0 */

    /* Restore clock source/domain bindings */
    mapClocks();

    /* Enable oscillator monitoring */
    systemREG1->CLKTEST = 0x000A0000;

    /* Enable RTI Compare 0 Interrupt */
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE0;
}
