
#include "bme280.h"
#include "sleep.h"
#include "user_i2c.h"
#include "sysctl.h"
#include "qmc7983.h"

/*!
 * @brief This API used to print the sensor temperature, pressure and humidity data.
 */
void print_sensor_data(struct bme280_data *comp_data, uint8_t *raw)
{
#ifdef BME280_FLOAT_ENABLE
    printf("temp: %0.2f, pres: %0.2f, hum: %0.2f\r\n", comp_data->temperature, comp_data->pressure / 100, comp_data->humidity);
#else
    printf("temp: %d, pres: %d, hum: %d\r\n", comp_data->temperature / 100, comp_data->pressure / 10000, comp_data->humidity / 1000);
#endif
    printf("%d, %d ,%d\r\n", raw[0], raw[1], raw[2]);
}
/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    int8_t rslt;
    uint8_t settings_sel;
	uint32_t req_delay;
    struct bme280_data comp_data;
    uint8_t raw[3];

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, dev);
    printf("settings_sel: %x\r\n",settings_sel);

	/*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    printf("Temperature, Pressure, Humidity\r\n");
    /* Continuously stream sensor data */
    while (1) {
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        /* Wait for the measurement to complete and print data @25Hz */
        dev->delay_us(req_delay*1000, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        qmc_read_xyz(raw);
        print_sensor_data(&comp_data, raw);
    }
    return rslt;
}

int8_t stream_sensor_data_normal_mode(struct bme280_dev *dev)
{
	int8_t rslt;
	uint8_t settings_sel;
	struct bme280_data comp_data;
    uint8_t raw[3];

	/* Recommended mode of operation: Indoor navigation */
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

	printf("Temperature, Pressure, Humidity\r\n");
	while (1) {
		/* Delay while the sensor completes a measurement */
		dev->delay_us(7000, dev->intf_ptr);
		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        qmc_read_xyz(raw);
		print_sensor_data(&comp_data, raw);
	}

	return rslt;
}

int main(int argc, char const *argv[])
{
    struct bme280_dev dev;
    int8_t rslt = BME280_OK;
    uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

    uint32_t freq = 0;
    freq = sysctl_pll_set_freq(SYSCTL_PLL0, 800000000);
    printf("pll freq: %dhz\r\n", freq);

    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_I2C_INTF;
    dev.read = bme280_i2c_read;
    dev.write = bme280_i2c_write;
    dev.delay_us = bme280_delay_us;

    // initialization of qmc7983 and bme280
    sp_weather_i2c_init();
    rslt = qmc_init();
    if(rslt != 0){
        printf("Failed to initialize qmc7983\r\n");
        return 0;
    }
    rslt = bme280_init(&dev);
    if(rslt != BME280_OK)
    {
        printf("Failed to initialize the device (code %+d).\r\n", rslt);
        return 0;
    }

    // read data and print
    printf("stream_sensor_data_forced_mode start\r\n");
    rslt = stream_sensor_data_normal_mode(&dev);
    if(rslt != BME280_OK)
    {
        printf("Failed to stream sensor data (code %+d).\r\n", rslt);
    }

    return 0;
}
