/** @file eps.h 
*   @brief EPS Board Implementation File
*   @date 20-Dec-2022
*   @author Stefan Damkjar, Junqi Zhu
*
*/

#include "eps.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "adc.h"
#include "system.h"
#include "ina226.h"
#include "ad5324.h"
#include "print.h"
#include "rti.h"
#include "het.h"
#include "gio.h"


static char  StringBuf[PRINT_BUFFER_SIZE+1];

char* EPS_Command[] = {
	"RESET",
	"READ",
	"WRITE"
};

char* EPS_Arg0[] = {
    "IDN",
    "TIME",
    "MPPT1",
    "MPPT2",
    "MPPT3",
    "MPPT4",
    "TEMP1",
    "HEATER1"
};

char* EPS_Arg1[] = {
    "OFF",
    "ON"
    "VOLT",
    "CURR",
    "POWER"
};

typedef enum
{
  EPS_Command_reset = 0,
  EPS_Command_read = 1,
  EPS_Command_write = 2
} EPS_Commands_TypeDef;

typedef enum
{
  EPS_Arg0_idn = 0,
  EPS_Arg0_time = 1,
  EPS_Arg0_mppt1 = 2,
  EPS_Arg0_mppt2 = 3,
  EPS_Arg0_mppt3 = 4,
  EPS_Arg0_mppt4 = 5,
  EPS_Arg0_temp1 = 6,
  EPS_Arg0_heater1 = 7
} EPS_Args_read_arg0_TypeDef;

typedef enum
{
  EPS_Arg1_off = 0,
  EPS_Arg1_on = 1,
  EPS_Arg1_volt = 2,
  EPS_Arg1_curr = 3,
  EPS_Arg1_power = 4

} EPS_Args_read_arg1_TypeDef;


void printBusVoltage(uint32_t address);
void printBusCurrent(uint32_t address, uint32_t senseResistor );

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

EPS_Err_TypeDef EPS_runCommand(char * command,
                            char * arg[EPS_MAX_ARGS],
                            uint8_t numArgs)
{
    static uint8_t i = 0;
    static adcData_t adc_data;
    static uint8_t heater1State = 0;
    PRINT_PrintString( PORT_UART_UART0,"ECHO: ");
    PRINT_PrintString( PORT_UART_UART0,command );
    PRINT_PrintChar(PORT_UART_UART0,'(');
    for (i = 0; i< numArgs; i++)
    {
        PRINT_PrintString( PORT_UART_UART0,arg[i] );
        PRINT_PrintChar(PORT_UART_UART0,',');
    }
    PRINT_Print(PORT_UART_UART0,4,"\b)\r\n");

    if(!strcmp(command,EPS_Command[EPS_Command_read]))
    {
        if(numArgs == 1 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_idn]))
        {
            sprintf(StringBuf,
                "0x%08X",
                DEVICE_ID_REV);
            PRINT_PrintStringln(scilinREG,StringBuf);
        }
        else if(numArgs == 1 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_time]))
        {
            sprintf(StringBuf,
                "%d",
                rtiGetCurrentTick(rtiCOMPARE1));
            PRINT_PrintStringln(scilinREG,StringBuf);
        }
        else if(numArgs == 2 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_mppt1]))
        {
            if(!strcmp(arg[1],EPS_Arg1[EPS_Arg1_volt]))
                printBusVoltage(EPS_I2CADDR_MPPT1);
            else if(!strcmp(arg[1],EPS_Arg1[EPS_Arg1_curr]))
                printBusCurrent(EPS_I2CADDR_MPPT1,EPS_SENSERESISTOR_MPPT1);
            else
            {
                PRINT_PrintStringln(PORT_UART_UART0,"\033[0;31mERROR: Invalid syntax! Bad arguments...\033[0m");
                return EPS_Err_Syntax;
            }
        }
        else if(numArgs == 1 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_temp1]))
        {
            adcStartConversion(adcREG1,adcGROUP1);
            while((adcIsConversionComplete(adcREG1,adcGROUP1))!=8);
            adcGetData(adcREG1, adcGROUP1,&adc_data);
            sprintf(StringBuf,
                "0x%03X",
                adc_data.value);
            PRINT_PrintStringln(scilinREG,StringBuf);
        }
        else if(numArgs == 1 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_heater1]))
        {
            sprintf(StringBuf,
                "%d",
                heater1State);
            PRINT_PrintStringln(scilinREG,StringBuf);
        }
        else
        {
            PRINT_PrintStringln(PORT_UART_UART0,"\033[0;31mERROR: Invalid syntax! Bad arguments...\033[0m");
            return EPS_Err_Syntax;
        }
    }
    else if (!strcmp(command,EPS_Command[EPS_Command_write]))
    {
        if(numArgs == 1 && !strcmp(arg[0],EPS_Arg0[EPS_Arg0_heater1]))
        {
            if(!strcmp(arg[1],EPS_Arg1[EPS_Arg1_on]))
            {
                PRINT_PrintStringln(scilinREG,"HEATER 1 ON!");
                heater1State = 1;
                gioSetBit(hetPORT1,17,heater1State);
            }
            else if(!strcmp(arg[1],EPS_Arg1[EPS_Arg1_off]))
            {
                PRINT_PrintStringln(scilinREG,"HEATER 1 OFF!");
                heater1State = 0;
                gioSetBit(hetPORT1,17,heater1State);
            }
            else
            {
                PRINT_PrintStringln(PORT_UART_UART0,"\033[0;31mERROR: Invalid syntax! Bad arguments...\033[0m");
                return EPS_Err_Syntax;
            }
        }
        else
        {
            PRINT_PrintStringln(PORT_UART_UART0,"\033[0;31mERROR: Invalid syntax! Bad arguments...\033[0m");
            return EPS_Err_Syntax;
        }
    }
    else
    {
        PRINT_PrintStringln(PORT_UART_UART0,"\033[0;31mERROR: Invalid syntax! Bad command...\033[0m");
        return EPS_Err_Syntax;
    }

    return EPS_Err_NoError;
}


void printBusVoltage(uint32_t address)
{
    int value = 0;
    INA226_Err_TypeDef result = INA226_Err_NoError;

    result = INA226_ReadBusVoltage( PORT_I2C,
                                    (INA226_Address_TypeDef)address,
                                    &value );

    if (result == INA226_Err_NoError)
    {
        value = INA226_BusVoltageToUV(value);

        sprintf(StringBuf,
                "Bus voltage (0x%02X): %d.%.6d V",
                address,value/1000000,value%1000000 );

        PRINT_PrintStringln( scilinREG,StringBuf );
    }
    else
    {
        sprintf(StringBuf,
                "\033[0;31mERROR: INA226 (Address: 0x%02X)\033[0m",
                address);

        PRINT_PrintString( scilinREG,StringBuf );

        if (result & INA226_Err_AL)
        {
            sprintf(StringBuf,
                    "\033[0;31m, Arbitration Lost\033[0m");

            PRINT_PrintString( scilinREG,StringBuf );
        }

        if (result & INA226_Err_NACK)
        {
            sprintf(StringBuf,
                    "\033[0;31m, No Acknowledgment\033[0m");

            PRINT_PrintString( scilinREG,StringBuf );
        }
        PRINT_PrintChar(scilinREG,'\r');
        PRINT_PrintChar(scilinREG,'\n');

        i2cInit();
    }
}


void printBusCurrent(uint32_t address, uint32_t senseResistor )
{
    int value = 0;
    INA226_Err_TypeDef result = INA226_Err_NoError;

    result = INA226_ReadShuntVoltage( PORT_I2C,
                                    (INA226_Address_TypeDef)address,
                                    &value );

    if (result == INA226_Err_NoError)
    {
        value = INA226_ShuntVoltageToUA(value, senseResistor);

        sprintf(StringBuf,
                "Bus current (0x%02X): %d.%.3d mA",
                address,value/1000,value%1000 );

        PRINT_PrintStringln( scilinREG,StringBuf );
    }
    else
    {
        sprintf(StringBuf,
                "\033[0;31mERROR: INA226 (Address: 0x%02X)\033[0m",
                address);

        PRINT_PrintString( scilinREG,StringBuf );

        if (result & INA226_Err_AL)
        {
            sprintf(StringBuf,
                    "\033[0;31m, Arbitration Lost\033[0m");

            PRINT_PrintString( scilinREG,StringBuf );
        }

        if (result & INA226_Err_NACK)
        {
            sprintf(StringBuf,
                    "\033[0;31m, No Acknowledgment\033[0m");

            PRINT_PrintString( scilinREG,StringBuf );
        }
        PRINT_PrintChar(scilinREG,'\r');
        PRINT_PrintChar(scilinREG,'\n');

        i2cInit();
    }
}
