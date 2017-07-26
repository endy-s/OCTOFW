/*
 * OCTO_ADC.h
 *
 * Created: 27/04/2017 23:29:12
 *  Author: Endy
 */ 


#ifndef OCTO_ADC_H_
#define OCTO_ADC_H_

#include <asf.h>

bool configure_adc_VMPPT (void);
bool configure_adc_TEMP (void);
bool configure_adc_BCAP (void);

void turn_off_adc(void);

void get_value_VMPPT (uint32_t *value, uint32_t *converted);
void get_value_TEMP (uint32_t *value, uint32_t *converted);
void get_value_BCAP (uint32_t *value, uint32_t *converted);


#endif /* OCTO_ADC_H_ */