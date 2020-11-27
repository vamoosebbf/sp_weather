#include "qmc7983.h"
#include "stdio.h"
typedef struct qmc_t{
    int mag_chip_id;
    
}qmc_t;

static qmc_t qmc;


void qmc_writeto_mem(uint8_t reg, uint8_t data){
    int8_t ret = 0;
    uint8_t buf[2] = {reg, data};
    ret = maix_i2c_send_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, buf, 2, 5);
    if(ret != 0){
        printf("send reg: %x %x failed\r\n",reg, data);
    }
    return;
}

uint8_t qmc_readfrom_mem(uint8_t reg){
    uint8_t res;
    int8_t ret = 0;
    ret = maix_i2c_recv_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, &reg, 1, &res, 1, 5);
    if(ret != 0){
        printf("recv reg: %x failed\r\n",reg);
    }
    return res;
}


int qmc_init(void){
    uint8_t chip;
    qmc.mag_chip_id = 0;

    qmc_writeto_mem(0x09, 0x1d);
    chip = qmc_readfrom_mem(0x0d);
    printf("chip: %x\r\n", chip);
    if(chip == 0x31){
        qmc.mag_chip_id = QMC6983_E1;
    }else if( chip == 0x32){
        qmc_writeto_mem(0x2e, 0x01);
        chip = qmc_readfrom_mem(0x2f);
        if(((chip & 0x04) >> 2) != 0){
            qmc.mag_chip_id = QMC6983_E1_Metal;
        }else{
            qmc_writeto_mem(0x2e, 0x0f);
            chip = qmc_readfrom_mem(0x2f);
            if(((chip & 0x3C) >> 2) == 0x02)
                qmc.mag_chip_id = QMC7983_Vertical;
            if(((chip & 0x3C) >> 2) == 0x03)
                qmc.mag_chip_id = QMC7983_Slope;
        }
    }else{
        return -1;
    }
    printf("qmc.mag_chip_id: %x\r\n", qmc.mag_chip_id);
    qmc_writeto_mem(0x21, 0x01);
    qmc_writeto_mem(0x20, 0x40);
    if(qmc.mag_chip_id != QMC6983_A1_D1){
        qmc_writeto_mem(0x29, 0x80);
        qmc_writeto_mem(0x0a, 0x0c);
    }
    if(qmc.mag_chip_id == QMC6983_E1_Metal || qmc.mag_chip_id == QMC7983_Slope)
        qmc_writeto_mem(0x1b, 0x80);
    qmc_writeto_mem(0x0b, 0x01);
    qmc_writeto_mem(0x09, 0x1d);
    return 0;
}

void qmc_read_xyz(uint16_t *raw){
    uint8_t read_data[6], reg = 0x00;
    maix_i2c_recv_data(WEATHER_I2C_DEVICE, QMCX983_I2CADDR, reg, 1, read_data, 6, 50);
    // read_data = self.i2c.readfrom_mem(self.address, 0x00, 6)
    raw[0] = (read_data[1]<<8) | read_data[0];
    raw[1] = (read_data[3]<<8) | read_data[2];
    raw[2] = (read_data[5]<<8) | read_data[4];
}
