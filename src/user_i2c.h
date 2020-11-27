#ifndef __USER_I2C__H__
#define __USER_I2C__H__

#include "board_config.h"
#include "fpioa.h"
#include "gpiohs.h"


/****************************************************************************/
/*!                         Functions                                       */

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs.
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *  @return void.
 *
 */
void user_delay_us(uint32_t period, void *intf_ptr);

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[out] data          : Pointer to the data buffer to store the read data.
 *  @param[in] len            : No of bytes to read.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 *  @return Status of execution
 *
 *  @retval 0 -> Success
 *  @retval > 0 -> Failure Info
 *
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[in] data           : Pointer to the data buffer whose value is to be written.
 *  @param[in] len            : No of bytes to write.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 *  @return Status of execution
 *
 *  @retval BME280_OK -> Success
 *  @retval BME280_E_COMM_FAIL -> Communication failure.
 *
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

int sp_weather_i2c_init();

#endif  //!__USER_I2C__H__
