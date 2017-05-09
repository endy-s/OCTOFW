/*
 * OCTO_I2C.h
 *
 * Created: 27/04/2017 23:30:04
 *  Author: Endy
 */ 


#ifndef OCTO_I2C_H_
#define OCTO_I2C_H_

#include <asf.h>

void configure_gas_gauge(void);

bool gas_gauge_read(uint32_t *value, uint32_t *percent);



#endif /* OCTO_I2C_H_ */