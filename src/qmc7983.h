#ifndef __QMC7983__H__
#define __QMC7983__H__

#include "sipeed_i2c.h"
#include "board_config.h"

#define QMC6983_A1_D1        0
#define QMC6983_E1           1
#define QMC7983              2
#define QMC7983_LOW_SETRESET 3
#define QMC6983_E1_Metal     4
#define QMC7983_Vertical     5
#define QMC7983_Slope        6

#define QMCX983_I2CADDR  0x2C

int qmc_init(void);
void qmc_read_xyz(uint8_t *raw);

#endif  //!__QMC7983__H__