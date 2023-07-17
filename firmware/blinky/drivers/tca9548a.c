/** @file tca9548a.c 
*   @brief TCA9548A I2C Multiplexer Driver Implementation File
*   @date 16-Jul-2023
*   @author Stefan Damkjar
*/

#include "tca9548a.h"
#include "port_i2c.h"

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Set the content of a register in the TCA9548A multiplexer.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of the TCA9548A multiplexer.
 *
 * @param[in] val
 *   Value to write to the register.
 *
 * @return
 *   Returns TCA9548A_Err_TypeDef indicating the status of the operation.
 ******************************************************************************/
TCA9548A_Err_TypeDef TCA9548A_RegisterSet(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    uint8_t val)
{
  /* Prepare the data to be sent */
  uint8_t data[1];
  data[0] = val;

  /* Send the data to the specified address */
  TCA9548A_Err_TypeDef ret = (TCA9548A_Err_TypeDef)PORT_I2C_Send(i2c, addr, 1, data);

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Get the content of a register in the TCA9548A multiplexer.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of the TCA9548A multiplexer.
 *
 * @param[out] val
 *   Pointer to store the value read from the register.
 *
 * @return
 *   Returns TCA9548A_Err_TypeDef indicating the status of the operation.
 ******************************************************************************/
TCA9548A_Err_TypeDef TCA9548A_RegisterGet(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    uint8_t *val)
{
  /* Receive the data from the specified address */
  TCA9548A_Err_TypeDef ret = (TCA9548A_Err_TypeDef)PORT_I2C_Receive(i2c, addr, 1, val);

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Set the status of a channel in the TCA9548A multiplexer.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of the TCA9548A multiplexer.
 *
 * @param[in] channel
 *   The channel to set the status for. Use the TCA9548A_Channel_TypeDef enumeration.
 *
 * @param[in] channel_status
 *   The status of the channel (enabled or disabled).
 *
 * @return
 *   Returns TCA9548A_Err_TypeDef indicating the status of the operation.
 ******************************************************************************/
TCA9548A_Err_TypeDef TCA9548A_SetChannel(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    TCA9548A_Channel_TypeDef channel,
                                    uint8_t channel_status)
{
  /* Read the current channel status register */
  uint8_t reg_val;
  TCA9548A_Err_TypeDef ret = TCA9548A_RegisterGet(i2c, addr, &reg_val);

  if (ret != TCA9548A_Err_NoError)
  {
    return ret;
  }

  /* Set the status of the specified channel */
  if (channel_status)
  {
    reg_val |= channel;
  }
  else
  {
    reg_val &= ~channel;
  }

  /* Write the updated channel status register */
  ret = TCA9548A_RegisterSet(i2c, addr, reg_val);

  return ret;
}

/***************************************************************************//**
 * @brief
 *   Get the status of a channel in the TCA9548A multiplexer.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of the TCA9548A multiplexer.
 *
 * @param[out] channel
 *   Pointer to store the active channel. Use the TCA9548A_Channel_TypeDef enumeration.
 *
 * @param[out] channel_status
 *   Pointer to store the status of the active channel (enabled or disabled).
 *
 * @return
 *   Returns TCA9548A_Err_TypeDef indicating the status of the operation.
 ******************************************************************************/
TCA9548A_Err_TypeDef TCA9548A_GetChannel(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr,
                                    TCA9548A_Channel_TypeDef channel,
                                    uint8_t *channel_status)
{
  /* Read the current channel status register */
  uint8_t reg_val;
  TCA9548A_Err_TypeDef ret = TCA9548A_RegisterGet(i2c, addr, &reg_val);

  if (ret != TCA9548A_Err_NoError)
  {
    return ret;
  }

  /* Determine the active channel and its status */
  *channel_status = (reg_val & (0x1<<channel)) ? 1 : 0;

  return ret;
}


/***************************************************************************//**
 * @brief
 *   Reset the TCA9548A multiplexer.
 *
 * @param[in] i2c
 *   Pointer to the I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address of the TCA9548A multiplexer.
 *
 * @return
 *   Returns TCA9548A_Err_TypeDef indicating the status of the operation.
 ******************************************************************************/
TCA9548A_Err_TypeDef TCA9548A_Reset(PORT_I2C_Reg_TypeDef *i2c,
                                    TCA9548A_Address_TypeDef addr)
{
  /* Write 0x00 to the control register to reset the multiplexer */
  TCA9548A_Err_TypeDef ret = TCA9548A_RegisterSet(i2c, addr, 0x00);

  return ret;
}
