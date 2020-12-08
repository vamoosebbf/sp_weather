#include "user_i2c.h"
#include "sleep.h"
#include "i2c.h"
#include "sipeed_i2c.h"
#include "qmc7983.h"
#include "stdlib.h"

int sp_weather_i2c_init(void)
{
    fpioa_set_function(WEATHER_SCL, FUNC_I2C0_SCLK + WEATHER_I2C_DEVICE * 2);
    fpioa_set_function(WEATHER_SDA, FUNC_I2C0_SDA + WEATHER_I2C_DEVICE * 2);
    maix_i2c_init(WEATHER_I2C_DEVICE, 7, 400000);
    return 0;
}

void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t addr;
    addr = *((uint8_t *)intf_ptr);
    return maix_i2c_recv_data(WEATHER_I2C_DEVICE, addr, &reg_addr, 1, data, len, 5);
}

int8_t bme280_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t addr;
    uint8_t *w_buf = (uint8_t *)malloc(len + 1);
    int ret;
    addr = *((uint8_t *)intf_ptr);
    w_buf[0] = reg_addr;
    memcpy(w_buf+1, data, len);
    ret = maix_i2c_send_data(WEATHER_I2C_DEVICE, addr, w_buf, len + 1, 5);
    free(w_buf);
    return ret;
}

void qmc_writeto_mem(uint8_t reg, uint8_t data)
{
    int8_t ret = 0;
    uint8_t buf[2] = {reg, data};
    ret = maix_i2c_send_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, buf, 2, 5);
    if (ret != 0)
    {
        printf("send reg: %x %x failed\r\n", reg, data);
    }
    return;
}

uint8_t qmc_readfrom_mem(uint8_t reg)
{
    uint8_t res;
    int8_t ret = 0;
    ret = maix_i2c_recv_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, &reg, 1, &res, 1, 5);
    if (ret != 0)
    {
        printf("recv reg: %x failed\r\n", reg);
    }
    return res;
}
