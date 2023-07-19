/** @file tca9548a.h 
*   @brief TCA9548A I2C Multiplexer Driver Definition File
*   @date 16-Jul-2023
*   @author Stefan Damkjar
*/

/** 
 *  @defgroup TCA9548A TCA9548A
 *  @brief TCA9548A I2C Multiplexer Module.
 *  
 *  The TCA9548A is an I2C bus multiplexer that allows multiple devices to share
 *  a single I2C bus. It has 8 channels, each with an individual enable bit.
 *
 *	Related Files
 *   - tca9548a.h
 *   - tca9548a.c
 *   - port_i2c.h
 *   - stdint.h
 */

#ifndef DRIVERS_TCA9548A_H_
#define DRIVERS_TCA9548A_H_

#include "port_i2c.h"
#include "stdint.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
 /* I2C address of the TCA9548A */  /*   A2  |  A1  |  A0   */
                                    /* -------------------- */
#define TCA9548A_ADDR70    (0x70U)  /*   GND |  GND |  GND  */
#define TCA9548A_ADDR71    (0x71U)  /*   GND |  GND |  VCC  */
#define TCA9548A_ADDR72    (0x72U)  /*   GND |  VCC |  GND  */
#define TCA9548A_ADDR73    (0x73U)  /*   GND |  VCC |  VCC  */
#define TCA9548A_ADDR74    (0x74U)  /*   VCC |  GND |  GND  */
#define TCA9548A_ADDR75    (0x75U)  /*   VCC |  GND |  VCC  */
#define TCA9548A_ADDR76    (0x76U)  /*   VCC |  VCC |  GND  */
#define TCA9548A_ADDR77    (0x77U)  /*   VCC |  VCC |  VCC  */

#define TCA9548A_CHANNEL_ALL (0xFFU)  /* All channels enable bit */
#define TCA9548A_CHANNEL_0 (0x01U)    /* Channel 0 enable bit    */
#define TCA9548A_CHANNEL_1 (0x02U)    /* Channel 1 enable bit    */
#define TCA9548A_CHANNEL_2 (0x04U)    /* Channel 2 enable bit    */
#define TCA9548A_CHANNEL_3 (0x08U)    /* Channel 3 enable bit    */
#define TCA9548A_CHANNEL_4 (0x10U)    /* Channel 4 enable bit    */
#define TCA9548A_CHANNEL_5 (0x20U)    /* Channel 5 enable bit    */
#define TCA9548A_CHANNEL_6 (0x40U)    /* Channel 6 enable bit    */
#define TCA9548A_CHANNEL_7 (0x80U)    /* Channel 7 enable bit    */
#define TCA9548A_CHANNEL_NONE (0x00U) /* No channels enable bit  */

/** 
 *  @addtogroup TCA9548A
 *  @{
 */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** @enum TCA9548A_Address_TypeDef
*   @brief Alias names for TCA9548A I2C addresses.
*/
typedef enum
{
  TCA9548A_Addr70 = TCA9548A_ADDR70, /**< A2 = GND  |  A1 = GND  |  A0 = GND */
  TCA9548A_Addr71 = TCA9548A_ADDR71, /**< A2 = GND  |  A1 = GND  |  A0 = VCC */
  TCA9548A_Addr72 = TCA9548A_ADDR72, /**< A2 = GND  |  A1 = VCC  |  A0 = GND */
  TCA9548A_Addr73 = TCA9548A_ADDR73, /**< A2 = GND  |  A1 = VCC  |  A0 = VCC */
  TCA9548A_Addr74 = TCA9548A_ADDR74, /**< A2 = VCC  |  A1 = GND  |  A0 = GND */
  TCA9548A_Addr75 = TCA9548A_ADDR75, /**< A2 = VCC  |  A1 = GND  |  A0 = VCC */
  TCA9548A_Addr76 = TCA9548A_ADDR76, /**< A2 = VCC  |  A1 = VCC  |  A0 = GND */
  TCA9548A_Addr77 = TCA9548A_ADDR77  /**< A2 = VCC  |  A1 = VCC  |  A0 = VCC */
} TCA9548A_Address_TypeDef;

/** @enum TCA9548A_Channel_TypeDef
*   @brief Alias names for TCA9548A channel numbers.
*/
typedef enum
{
  TCA9548A_Channel_0 = TCA9548A_CHANNEL_0, /**< Channel 0 */
  TCA9548A_Channel_1 = TCA9548A_CHANNEL_1, /**< Channel 1 */
  TCA9548A_Channel_2 = TCA9548A_CHANNEL_2, /**< Channel 2 */
  TCA9548A_Channel_3 = TCA9548A_CHANNEL_3, /**< Channel 3 */
  TCA9548A_Channel_4 = TCA9548A_CHANNEL_4, /**< Channel 4 */
  TCA9548A_Channel_5 = TCA9548A_CHANNEL_5, /**< Channel 5 */
  TCA9548A_Channel_6 = TCA9548A_CHANNEL_6, /**< Channel 6 */
  TCA9548A_Channel_7 = TCA9548A_CHANNEL_7  /**< Channel 7 */
} TCA9548A_Channel_TypeDef;

/** @enum TCA9548A_Err_TypeDef
*   @brief Alias names for TCA9548A error codes.
*/
typedef enum
{
  TCA9548A_Err_NoError   = 0U,                /**< No error*/
  TCA9548A_Err_AL        = PORT_I2C_Err_AL,   /**< Arbitration lost*/
  TCA9548A_Err_NACK      = PORT_I2C_Err_NACK  /**< No acknowledgment */
} TCA9548A_Err_TypeDef;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

TCA9548A_Err_TypeDef TCA9548A_RegisterSet(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    uint8_t val);

TCA9548A_Err_TypeDef TCA9548A_RegisterGet(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    uint8_t *val);

TCA9548A_Err_TypeDef TCA9548A_SetChannel(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    TCA9548A_Channel_TypeDef channel,
                                    uint8_t channel_status);

TCA9548A_Err_TypeDef TCA9548A_GetChannel(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    TCA9548A_Channel_TypeDef channel,
                                    uint8_t *channel_status);

TCA9548A_Err_TypeDef TCA9548A_Reset(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr);

/**@}*/

#endif /* DRIVERS_TCA9548A_H_ */
