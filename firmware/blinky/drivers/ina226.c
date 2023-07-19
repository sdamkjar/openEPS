/** @file ina226.c 
*   @brief INA226 Driver Implementation File
*   @date 16-Dec-2022
*   @author Stefan Damkjar, Junqi Zhu
*
*/

#include "ina226.h"
#include "port_i2c.h"
#include "stdint.h"
#include <stdlib.h>


/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

void INA226_Init(INA226_TypeDef* const ina226,
              PORT_I2C_Reg_TypeDef *i2c,
              INA226_Address_TypeDef addr,
              uint8_t muxChan,
              uint32_t senseResistor,
              INA226_Err_TypeDef (*RegisterGet)(INA226_TypeDef* const ina226,
                                                INA226_Register_TypeDef reg,
                                                uint16_t *val),
              INA226_Err_TypeDef (*RegisterSet)(INA226_TypeDef* const ina226,
                                                INA226_Register_TypeDef reg,
                                                uint16_t val))
{
  /* initialize attributes */
  ina226->i2c = i2c;
  ina226->addr = addr;
  ina226->muxChan = muxChan;
  ina226->senseResistor = senseResistor;

  /* initialize function pointers */
  ina226->RegisterGet = RegisterGet;
  ina226->RegisterSet = RegisterSet;
}

/***************************************************************************//**
 * @brief
 *   Set content of a register.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[in] reg
 *   Register to write (input register cannot be written).
 *
 * @param[in] val
 *   Pointer to value used when writing to register.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_RegisterSet(INA226_TypeDef* const ina226,
                         INA226_Register_TypeDef reg,
                         uint16_t val)
{
  uint8_t data[3];

  data[0] = ((uint8_t)reg);
  data[1] = (uint8_t)(val >> 8);
  data[2] = (uint8_t)val;

  return (INA226_Err_TypeDef)PORT_I2C_Send(ina226->i2c, ina226->addr, 3, data);

}


/***************************************************************************//**
 * @brief
 *   Get current content of a register.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[in] reg
 *   Register to read.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_RegisterGet(INA226_TypeDef* const ina226,
                         INA226_Register_TypeDef reg,
                         uint16_t *val)
{

  uint8_t regid[1];
  uint8_t data[2];

  regid[0] = ((uint8_t)reg);

  INA226_Err_TypeDef ret = INA226_Err_NoError;

  /*****************************************/
  //  Send address of register to be read
  /*****************************************/

  ret = (INA226_Err_TypeDef)PORT_I2C_Send(ina226->i2c, ina226->addr, 1, regid);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  /*****************************************/
  //  Start receving the data From Slave
  /*****************************************/

  ret = (INA226_Err_TypeDef)PORT_I2C_Receive(ina226->i2c, ina226->addr, 2, data);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  /* Simple delay before starting Next Block */
  /* Depends on how quick the Slave gets ready */
  //for(i=0;i<INA226_DELAY;i++);

  /* Save result */
  *val = (((uint16_t)(data[0])) << 8) | data[1];

  return ret;

}

INA226_TypeDef *INA226_Create(PORT_I2C_Reg_TypeDef *i2c,
                               INA226_Address_TypeDef addr,
                               uint8_t muxChan,
                               uint32_t senseResistor)
{
  INA226_TypeDef *ina226 = (INA226_TypeDef *)malloc(sizeof(INA226_TypeDef));
  INA226_Init(ina226, i2c, addr, muxChan, senseResistor, INA226_RegisterGet, INA226_RegisterSet);
  return ina226;
}

void INA226_Destroy(INA226_TypeDef *ina226)
{
  free(ina226);
}


/***************************************************************************//**
 * @brief
 *   Read Current Shunt Voltage Register.
 * 
 * @details
 *   - Negative numbers are represented in two's complement format.
 *   - If averaging is enabled, this register displays the averaged value.
 *   - Full-scale range: -81.92 mV (0xFFFF) to + 81.92 mV (0x7FFF).
 *   - LSB represents 2.5 uV.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_ReadShuntVoltage(INA226_TypeDef* const ina226,
                         int *val)
{

  INA226_Err_TypeDef ret = INA226_Err_NoError;
  uint16_t tmp = 0;

  ret = ina226->RegisterGet(ina226,INA226_RegShuntV,&tmp);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  /* Convert two's complement if number is negative */
  if(tmp >> 15)
  {
  	*val = -(tmp & 0x7FFF);
  }
  else *val = tmp;

  return(ret);

}


/***************************************************************************//**
 * @brief
 *   Read Bus Voltage Register.
 * 
 * @details
 *   - Negative values are not supported.
 *   - If averaging is enabled, this register displays the averaged value.
 *   - Full-scale range: 0 V (0x0000) to + 40.96 V (0x7FFF).
 *   - LSB represents 1.25 mV.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_ReadBusVoltage(INA226_TypeDef* const ina226,
                         int *val)
{

  INA226_Err_TypeDef ret = INA226_Err_NoError;

  uint16_t tmp = 0;

  ret = ina226->RegisterGet(ina226,INA226_RegBusV,&tmp);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  *val = tmp & 0x7FFF;

  return(ret);

}


/***************************************************************************//**
 * @brief
 *   Read Current Register.
 * 
 * @details
 *   - Negative numbers are represented in two's complement format.
 *   - If averaging is enabled, this register displays the averaged value.
 *   - The value of the current register is calculated by multiplying the
 *     decimal value in the Shunt Voltage Register with the decimal value of the
 *     Calibration Register and then dividing my 2048.
 * 
 *        Current = (ShuntVoltage × CalibrationRegister) / 2048
 * 
 *   - Full-scale range: 0 V (0x0000) to + 40.96 mV (0x7FFF).
 *   - LSB represents 1.25 mV.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_ReadCurr(INA226_TypeDef* const ina226,
                         int *val)
{

  INA226_Err_TypeDef ret = INA226_Err_NoError;

  uint16_t tmp = 0;

  ret = ina226->RegisterGet(ina226,INA226_RegCurr,&tmp);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  if(tmp >> 15)
  {
  	*val = -(tmp & 0x7FFF);
  }
  else *val = tmp;

  return(ret);

}


/***************************************************************************//**
 * @brief
 *   Read Power Register.
 * 
 * @details
 *   - If averaging is enabled, this register displays the averaged value.
 *   - The value of the power register is calculated by multiplying the
 *     decimal value in the Current Register with the decimal value of the
 *     Bus Voltage Register and then dividing by 20000.
 * 
 *        Current = (CurrentRegister × BusVoltage) / 20000
 * 
 *   - Full-scale range: 0x0000 to 0xFFFF.
 *   - LSB represents 25 times the Current Register LSB.
 * 
 * @details
 *   - Negative values are not supported.
 *   - If averaging is enabled, this register displays the averaged value.
 *   - Full-scale range: 0 V (0x0000) to + 40.96 mV (0x7FFF).
 *   - LSB represents 1.25 mV.
 *
 * @param[in] ina226
 *  Pointer to INA226 object.
 *
 * @param[out] val
 *   Reference to place register read.
 *
 * @return
 *   Returns 0 if no error.
 ******************************************************************************/
INA226_Err_TypeDef INA226_ReadPower(INA226_TypeDef* const ina226,
                         int *val)
{

  INA226_Err_TypeDef ret = INA226_Err_NoError;

  uint16_t tmp = 0;

  ret = ina226->RegisterGet(ina226,INA226_RegPower,&tmp);

  if (ret != INA226_Err_NoError)
  {
    return ret;
  }

  *val = tmp;

  return(ret);

}


/***************************************************************************//**
 * @brief
 *   Convert Bus Voltage Register value to uV.
 * 
 * @param[in] busVoltageRegister
 *   Value from Bus Voltage Register read to be converted.
 * 
 * @return
 *   Returns bus voltage in uV.
 ******************************************************************************/
int INA226_BusVoltageToUV(int val)
{
  /* Convert Bus Voltage Register (with LSB equal to 1.25mV) to uV*/
  return val * INA226_BUSVOLTAGELSB;
}


/***************************************************************************//**
 * @brief
 *   Calculate current in uA from Shunt Voltage Register and sense resistor
 *   value in mOhm.
 * 
 * @param[in] shuntVoltageRegister
 *   Value from Shunt Voltage Register read to be converted.
 * 
 * @param[in] senseResistor
 *   Sense resistor value in mOhm.
 * 
 * @return
 *   Returns current in uA.
 ******************************************************************************/
int INA226_ShuntVoltageToUA(int val, uint32_t senseResistor )
{
  /* Divide Shunt Voltage Register (with LSB equal to 2.5uV) by sense resistor
   * in mOhm to get current in uA */
  return val * INA226_SHUNTVOLTAGELSB / senseResistor;
}
