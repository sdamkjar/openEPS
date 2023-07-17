/** @file port_i2c.c 
*   @brief Portable frontend for I2C Peripheral using TI HAL libraries.
*   @date 19-Dec-2022
*   @author Stefan Damkjar, Junqi Zhu
*
*/

#include "port_i2c.h"
#include "i2c.h"
#include "stdint.h"

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Send a block of data over i2c peripheral.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of slave, in 7 bit format.
 *
 * @param[in] length
 *   Number of uint8_t data words to transfer
 *
 * @param[in] data
 *   Pointer to data to send.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/

PORT_I2C_Err_TypeDef PORT_I2C_Send(PORT_I2C_Reg_TypeDef *i2c,
                            uint32_t addr,
                            uint32_t length,
                            uint8_t *data   )
{

  uint32_t i = 0;

  /* Configure address of Slave to talk to */
  i2cSetSlaveAdd(i2c, addr);

  /* Set direction to Transmitter */
  /* Note: Optional - It is done in Init */
  i2cSetDirection(i2c, I2C_TRANSMITTER);

  /* Set mode as Master */
  i2cSetMode(i2c, I2C_MASTER);

  /* Configure Data count */
  /* Data Count + 1 ( Word Address) */
  i2cSetCount(i2c, length);

  /* Set Stop after programmed Count */
  i2cSetStop(i2c);

  /* Transmit Start Condition */
  i2cSetStart(i2c);

  /* Transmit data in Polling mode */
  i2cSend(i2c, length, data);

  /* Wait until Bus Busy is cleared */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsBusBusy(i2c) == false) break;
  }

  /* Wait until Stop is detected */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsStopDetected(i2c) == true) break;
  }

  /* Clear the Stop condition */
  i2cClearSCD(i2c);

  /* wait until MST bit gets cleared, this takes few cycles after Bus Busy is
  * cleared */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsMasterReady(i2c) == true) break;
  }

  return (PORT_I2C_Err_TypeDef)i2cRxError(i2c);
}

/***************************************************************************//**
 * @brief
 *   Receive a block of data over i2c peripheral.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of slave, in 7 bit format.
 *
 * @param[in] length
 *   Number of uint8_t data words to transfer
 *
 * @param[in] data
 *   Pointer to data to send.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
PORT_I2C_Err_TypeDef PORT_I2C_Receive(PORT_I2C_Reg_TypeDef *i2c,
                            uint32_t addr,
                            uint32_t length,
                            uint8_t *data   )
{

  uint32_t i = 0;

  /* Configure address of Slave to talk to */
  i2cSetSlaveAdd(i2c, addr);

  /* Set direction to Transmitter */
  /* Note: Optional - It is done in Init */
  i2cSetDirection(i2c, I2C_RECEIVER);

  /* Set mode as Master */
  i2cSetMode(i2c, I2C_MASTER);

  /* Configure Data count */
  /* Data Count + 1 ( Word Address) */
  i2cSetCount(i2c, length);

  /* Set Stop after programmed Count */
  i2cSetStop(i2c);

  /* Transmit Start Condition */
  i2cSetStart(i2c);

  /* Receive data in Polling mode */
  i2cReceive(i2c, length, data);

  /* Wait until Bus Busy is cleared */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsBusBusy(i2c) == false) break;
  }

  /* Wait until Stop is detected */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsStopDetected(i2c) == true) break;
  }

  /* Clear the Stop condition */
  i2cClearSCD(i2c);

  /* wait until MST bit gets cleared, this takes few cycles after Bus Busy is
  * cleared */
  for(i=0;i<PORT_I2C_MAX_RETRIES;i++)
  {
    if (i2cIsMasterReady(i2c) == true) break;
  }

  return (PORT_I2C_Err_TypeDef)i2cRxError(i2c);
}
