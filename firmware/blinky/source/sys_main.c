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
#include "sci.h"
#include "het.h"
#include "gio.h"
#include "i2c.h"
#include "eps.h"
#include "print.h"
#include "queue.h"
#include "tca9548a.h"
#include "ina226.h"
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

/* Global Variables */
static char CommandString[PRINT_BUFFER_SIZE + 1];
static char uartRxData;
Queue *receiveBuffer;

/* Function Prototypes */
void rtiNotification(uint32_t notification);
void ssiInterrupt(void);
void PORT_UART_ISR(PORT_UART_Reg_TypeDef *uart, uint32_t flags);

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    /* Initialize necessary peripherals and configurations */

    INA226_TypeDef *battery_bus_sensor;
    
    battery_bus_sensor = INA226_Create(i2cREG1,
                                    EPS_BATBUS_I2CADDR,
                                    EPS_BATBUS_MUXCHAN,
                                    EPS_BATBUS_SENSERESISTOR);

    rtiInit();
    i2cInit();

    PORT_UART_Init();
    PORT_UART_Enable_ISR(PORT_UART_UART0,PORT_UART_Flags_RX);

    /* Initialize receive buffer */
    receiveBuffer = QUEUE_Create();

    /* Set direction for I2C_MUX_nRESET and LED pins */
    gioSetDirection(EPS_GPIO_LED_PORT, (1<<EPS_GPIO_LED_PIN) | (1<<EPS_GPIO_I2CMUXRESET_PIN));

    /* Enable RTI Compare 0 interrupt notification and start RTI counters */
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    rtiStartCounter(rtiCOUNTER_BLOCK0);
    rtiStartCounter(rtiCOUNTER_BLOCK1);

    _enable_IRQ();

    // Set HET1_26 (I2C_MUX_nRESET) to high
    gioSetBit(EPS_GPIO_I2CMUXRESET_PORT, EPS_GPIO_I2CMUXRESET_PIN, 1);
    TCA9548A_RegisterSet(i2cREG1, EPS_MUX1_I2CADDR, EPS_BATBUS_MUXCHAN);
    uint16_t voltage = 0;
    uint16_t shunt_voltage = 0;
    battery_bus_sensor->RegisterGet(battery_bus_sensor, INA226_RegBusV, &voltage);
    battery_bus_sensor->RegisterGet(battery_bus_sensor, INA226_RegShuntV, &shunt_voltage);
    // Clear HET1_26 (I2C_MUX_nRESET) to low
    gioSetBit(EPS_GPIO_I2CMUXRESET_PORT, EPS_GPIO_I2CMUXRESET_PIN, 0);

    PORT_UART_Receive(PORT_UART_UART0,1,&uartRxData);

    /* Run forever */
    while (1)
    {
        /* Do nothing...*/
    }

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

void rtiNotification(uint32_t notification)
{
    if(notification == rtiNOTIFICATION_COMPARE0)
    {
        /* Toggle HET1_16 (LED) */
        gioSetPort(EPS_GPIO_LED_PORT, gioGetPort(EPS_GPIO_LED_PORT) ^ (1<<EPS_GPIO_LED_PIN));

    }
}

#pragma INTERRUPT(ssiInterrupt, IRQ)
void ssiInterrupt(void)
{

    static char * function;
    static char * arg[EPS_MAX_ARGS];
    static uint8_t i = 0;

    if (systemREG1->SSIVEC & 0x1U )
    {
        /* Extract function name from command string */
        function = strtok(CommandString,"(");

        /* Extract arguments from command string */
        for(i = 0; i<EPS_MAX_ARGS; i++)
        {
            arg[i] = strtok(NULL,",");
            if (arg[i] == NULL) break;
        }
        i = i - 1;
        arg[i] = strtok(arg[i],")");

        /* Split argument strings into individual arguments */
        for(i = 0; arg[i]!=NULL && i<EPS_MAX_ARGS; i++)
        {
            arg[i] = strtok(arg[i]," ");

        }

        /* Call the EPS_runCommand function with the parsed command */
        EPS_runCommand(function,arg,i);

    }
}

void PORT_UART_ISR(PORT_UART_Reg_TypeDef *uart, uint32_t flags)
{
    static uint8_t i = 0;
    static bool commandReceived = false;

    if(flags & PORT_UART_Flags_RX)
    {
        if (uartRxData == '\r' || uartRxData == '\n')
        {
            if (commandReceived)
            {
                /* Insert null terminator at the end of command string */
                receiveBuffer->insert(receiveBuffer,'\0');

                /* Copy command string from receive buffer to CommandString */
                for (i = 0; (!receiveBuffer->isEmpty(receiveBuffer)); i++)
                {
                    CommandString[i] = receiveBuffer->remove(receiveBuffer);
                }

                /* Clear commandReceived flag */
                commandReceived = false;

                /* Trigger SSISR1 to process the command */
                systemREG1->SSISR1 = 0x7500U;
            }
        }
        else if (!receiveBuffer->isFull(receiveBuffer))
        {
            /* Convert lowercase characters to uppercase */
            if (uartRxData >= 'a' && uartRxData <= 'z')
            {
                uartRxData = uartRxData - 0x20;
            }

            /* Insert received character into the receive buffer */
            receiveBuffer->insert(receiveBuffer, uartRxData);

            /* Set commandReceived flag */
            commandReceived = true;
        }

        /* Receive next character */
        PORT_UART_Receive(uart,1,&uartRxData);
    }
}

/* USER CODE END */
