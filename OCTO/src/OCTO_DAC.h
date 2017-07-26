/*
 * OCTO_DAC.h
 *
 * Created: 04/05/2017 18:26:51
 *  Author: Endy
 */ 


#ifndef OCTO_DAC_H_
#define OCTO_DAC_H_

#include <asf.h>

uint16_t bright_reference;

void configure_dac(void);
void configure_dac_channel(void);
void set_led_bright_perthousand(uint16_t perthousand);

#endif /* OCTO_DAC_H_ */