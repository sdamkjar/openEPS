/** @file eps.h 
*   @brief EPS Board Definition File
*   @date 20-Dec-2022
*   @author Stefan Damkjar, Junqi Zhu
*
*/

/** 
 *  @defgroup EPS EPS
 *  @brief Portable I2C Driver Wrapper Module for TI HAL libraries.
 *
 *	Related Files
 *   - eps.h
 *   - eps.c
 *   - stdint.h
 */

#ifndef DRIVERS_EPS_H_
#define DRIVERS_EPS_H_

#include "port_i2c.h"
#include "print.h"
#include "stdint.h"
#include "ina226.h"
#include "tca9548a.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define EPS_MAX_ARGS (10)

#define EPS_COMMAND_READ 

/*****************************************/
//  I2C Addresses
/*****************************************/

#define EPS_I2CMUX1_ADDR (TCA9548A_Addr71) 
#define EPS_I2CMUX2_ADDR (TCA9548A_Addr70)

/* I2C0 */
#define EPS_I2CADDR_MPPT1 (INA226_Addr42)
#define EPS_I2CADDR_MPPT2 (INA226_Addr43)
#define EPS_I2CADDR_MPPT3 (INA226_Addr41)
#define EPS_I2CADDR_MPPT4 (INA226_Addr40)

#define EPS_I2CADDR_EPS3V3 (INA226_Addr46)
#define EPS_I2CADDR_EPS1V2 (INA226_Addr45)
#define EPS_I2CADDR_PV3V3  (INA226_Addr47)

#define EPS_I2CADDR_3V3BUS (INA226_Addr4D) 
#define EPS_I2CADDR_1V2BUS (INA226_Addr4E)
#define EPS_I2CADDR_5V0BUS (INA226_Addr4F)
#define EPS_I2CADDR_BATBUS (INA226_Addr44) 

//#define EPS_I2CADDR_RTC    (RV3032_Addr51)

//#define EPS_I2CADDR_TEMP1  (TMP117_Addr48)
//#define EPS_I2CADDR_TEMP2  (TMP117_Addr49)
//#define EPS_I2CADDR_TEMP3  (TMP117_Addr4A)
//#define EPS_I2CADDR_TEMP4  (TMP117_Addr4B)

/* I2C1 */
#define EPS_I2CADDR_OUTPUT01 (INA226_Addr40)
#define EPS_I2CADDR_OUTPUT02 (INA226_Addr41)
#define EPS_I2CADDR_OUTPUT03 (INA226_Addr42)
#define EPS_I2CADDR_OUTPUT04 (INA226_Addr43)
#define EPS_I2CADDR_OUTPUT05 (INA226_Addr44)
#define EPS_I2CADDR_OUTPUT06 (INA226_Addr45)
#define EPS_I2CADDR_OUTPUT07 (INA226_Addr46)
#define EPS_I2CADDR_OUTPUT08 (INA226_Addr47)
#define EPS_I2CADDR_OUTPUT09 (INA226_Addr48)
#define EPS_I2CADDR_OUTPUT10 (INA226_Addr49)
#define EPS_I2CADDR_OUTPUT11 (INA226_Addr4A)
#define EPS_I2CADDR_OUTPUT12 (INA226_Addr4B)
#define EPS_I2CADDR_OUTPUT13 (INA226_Addr4C)
#define EPS_I2CADDR_OUTPUT14 (INA226_Addr4D)
#define EPS_I2CADDR_OUTPUT15 (INA226_Addr4E)
#define EPS_I2CADDR_OUTPUT16 (INA226_Addr4F)

/* I2C2 */
#define EPS_I2CADDR_OUTPUT17 (INA226_Addr4C)
#define EPS_I2CADDR_OUTPUT18 (INA226_Addr4E)

//#define EPS_I2CADDR_TEMP5   (TMP117_Addr48)
//#define EPS_I2CADDR_TEMP6   (TMP117_Addr49)
//#define EPS_I2CADDR_TEMP7   (TMP117_Addr4A)
//#define EPS_I2CADDR_TEMP8   (TMP117_Addr4B)



/*****************************************/
//  Current sense resistors in mOhm
/*****************************************/

#define EPS_SENSERESISTOR_OUTPUT01 (5)
#define EPS_SENSERESISTOR_OUTPUT02 (5)
#define EPS_SENSERESISTOR_OUTPUT03 (5)
#define EPS_SENSERESISTOR_OUTPUT04 (5)
#define EPS_SENSERESISTOR_OUTPUT05 (5)
#define EPS_SENSERESISTOR_OUTPUT06 (5)
#define EPS_SENSERESISTOR_OUTPUT07 (5)
#define EPS_SENSERESISTOR_OUTPUT08 (5)
#define EPS_SENSERESISTOR_OUTPUT09 (5)
#define EPS_SENSERESISTOR_OUTPUT10 (5)
#define EPS_SENSERESISTOR_OUTPUT11 (5)
#define EPS_SENSERESISTOR_OUTPUT12 (5)
#define EPS_SENSERESISTOR_OUTPUT13 (5)
#define EPS_SENSERESISTOR_OUTPUT14 (5)
#define EPS_SENSERESISTOR_OUTPUT15 (5)
#define EPS_SENSERESISTOR_OUTPUT16 (5)
#define EPS_SENSERESISTOR_OUTPUT17 (5)
#define EPS_SENSERESISTOR_OUTPUT18 (5)

#define EPS_SENSERESISTOR_3V3BUS   (5)
#define EPS_SENSERESISTOR_1V2BUS   (5)
#define EPS_SENSERESISTOR_5V0BUS   (5)
#define EPS_SENSERESISTOR_BATBUS   (5)

#define EPS_SENSERESISTOR_EPS3V3   (5)
#define EPS_SENSERESISTOR_EPS1V2   (5)
#define EPS_SENSERESISTOR_PV3V3    (5)

#define EPS_SENSERESISTOR_MPPT1    (5)  
#define EPS_SENSERESISTOR_MPPT2    (5)
#define EPS_SENSERESISTOR_MPPT3    (5)
#define EPS_SENSERESISTOR_MPPT4    (5)

/** 
 *  @addtogroup EPS
 *  @{
 */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
typedef enum
{
  EPS_Err_NoError     = 0, /**< No error*/
  EPS_Err_Syntax = 1  /**< Invalid syntax (see manual for help)*/
} EPS_Err_TypeDef;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

EPS_Err_TypeDef EPS_runCommand(char * function,
                            char * arg[EPS_MAX_ARGS],
                            uint8_t numArgs);

/**@}*/

#endif /* DRIVERS_EPS_H_ */
