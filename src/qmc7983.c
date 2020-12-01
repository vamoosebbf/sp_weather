#include "qmc7983.h"
#include "stdio.h"
#include "user_i2c.h"

static int mag_chip_id;

int qmc_init(void){
    uint8_t chip;
    mag_chip_id = 0;

    qmc_writeto_mem(0x09, 0x1d);
    chip = qmc_readfrom_mem(0x0d);
    printf("chip: %x\r\n", chip);
    if(chip == 0x31){
        mag_chip_id = QMC6983_E1;
    }else if( chip == 0x32){
        qmc_writeto_mem(0x2e, 0x01);
        chip = qmc_readfrom_mem(0x2f);
        if(((chip & 0x04) >> 2) != 0){
            mag_chip_id = QMC6983_E1_Metal;
        }else{
            qmc_writeto_mem(0x2e, 0x0f);
            chip = qmc_readfrom_mem(0x2f);
            if(((chip & 0x3C) >> 2) == 0x02)
                mag_chip_id = QMC7983_Vertical;
            if(((chip & 0x3C) >> 2) == 0x03)
                mag_chip_id = QMC7983_Slope;
        }
    }else{
        return -1;
    }
    printf("mag_chip_id: %x\r\n", mag_chip_id);
    qmc_writeto_mem(0x21, 0x01);
    qmc_writeto_mem(0x20, 0x40);
    if(mag_chip_id != QMC6983_A1_D1){
        qmc_writeto_mem(0x29, 0x80);
        qmc_writeto_mem(0x0a, 0x0c);
    }
    if(mag_chip_id == QMC6983_E1_Metal || mag_chip_id == QMC7983_Slope)
        qmc_writeto_mem(0x1b, 0x80);
    qmc_writeto_mem(0x0b, 0x01);
    qmc_writeto_mem(0x09, 0x1d);
    return 0;
}

void qmc_read_xyz(uint8_t *raw){
    uint8_t read_data[6], reg = 0x00;
    maix_i2c_recv_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, reg, 1, read_data, 6, 50);
    raw[0] = (read_data[1]<<8) | read_data[0];
    raw[1] = (read_data[3]<<8) | read_data[2];
    raw[2] = (read_data[5]<<8) | read_data[4];
}
