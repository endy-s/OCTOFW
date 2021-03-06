/*
 * OCTO_I2C.h
 *
 * Created: 27/04/2017 23:30:04
 *  Author: Endy
 */ 


#ifndef OCTO_I2C_H_
#define OCTO_I2C_H_

#include <asf.h>

void configure_gas_gauge(uint32_t battery_percent);

void gas_gauge_update_percent(uint32_t new_battery_percent);
void gas_gauge_config_CC_registers(void);
void gas_gauge_config_AL_registers(void);
bool gas_gauge_read(uint32_t *value, uint32_t *percent);



#endif /* OCTO_I2C_H_ */