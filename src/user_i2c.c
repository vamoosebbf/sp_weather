#include "user_i2c.h"
#include "sleep.h"
#include "i2c.h"
#include "sipeed_i2c.h"

int sp_weather_i2c_init()
{
    fpioa_set_function(WEATHER_SCL, FUNC_I2C0_SCLK + WEATHER_I2C_DEVICE * 2);
    fpioa_set_function(WEATHER_SDA, FUNC_I2C0_SDA + WEATHER_I2C_DEVICE * 2);
    maix_i2c_init(WEATHER_I2C_DEVICE, 7, 400000);
    return 0;
}

void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t addr;
    addr = *((uint8_t *)intf_ptr);
    return maix_i2c_recv_data(WEATHER_I2C_DEVICE, addr, &reg_addr, 1, data, len, 50);
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t addr;
    addr = *((uint8_t *)intf_ptr);
    return maix_i2c_send_data(WEATHER_I2C_DEVICE, addr, data, len, 50);
}