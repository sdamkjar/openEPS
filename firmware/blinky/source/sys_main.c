/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "system.h"
#include "rti.h"
#include "het.h"
#include "gio.h"
#include "i2c.h"
#include "eps.h"
#include "tca9548a.h"
#include "low_power_mode.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    rtiInit();
    i2cInit();
    gioSetDirection(hetPORT1, 0xFFFFFFFF);
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    _enable_IRQ();
    rtiStartCounter(rtiCOUNTER_BLOCK0);

    // Set HET1_26 to high
    gioSetBit(hetPORT1, 26, 1);

    // Set HET1_22 to high
    gioSetBit(hetPORT1, 22, 1);

    uint8_t val = 0;

    TCA9548A_RegisterSet(i2cREG1, EPS_I2CMUX1_ADDR, TCA9548A_CHANNEL_3);

    TCA9548A_GetChannel(i2cREG1, EPS_I2CMUX1_ADDR, TCA9548A_Channel_3, &val);

    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification)
{

    gioSetPort(hetPORT1, gioGetPort(hetPORT1) ^ (0x00000001<<16));
    

}
/* USER CODE END */
