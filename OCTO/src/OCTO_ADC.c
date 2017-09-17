/*
 * OCTO_ADC.c
 *
 * Created: 27/04/2017 23:28:22
 *  Author: Endy
 */ 

#include "OCTO_ADC.h"


#define D_ADC_VREF          (3300)
#define D_ADC_RESOLUTION    (1 << 12)

#define VMPPT_PULL_UP       100
#define VMPPT_PULL_DOWN     220


struct adc_module adc_instance;

//=============================================================================
//! \brief Initialize ADC peripheral.
//! \return TRUE if that ADC was successfully configured.
//=============================================================================
bool configure_adc_VMPPT (void)
{
    struct adc_config conf_adc;

    adc_get_config_defaults(&conf_adc);

    conf_adc.reference = ADC_REFERENCE_AREFA;
    conf_adc.positive_input = VMPPT_ADC_0_PIN;
    
    //system_voltage_reference_enable(ADC_REFERENCE_AREFA);

    adc_init(&adc_instance, ADC_MODULE, &conf_adc);

    adc_enable(&adc_instance);

    return true;
}

bool configure_adc_TEMP (void)
{
    struct adc_config conf_adc;

    adc_get_config_defaults(&conf_adc);

    conf_adc.reference = ADC_REFERENCE_AREFA;
    conf_adc.positive_input = TEMP_ADC_1_PIN;
    
    //system_voltage_reference_enable(ADC_REFERENCE_AREFA);

    adc_init(&adc_instance, ADC_MODULE, &conf_adc);

    adc_enable(&adc_instance);

    return true;
}


void turn_off_adc(void)
{
    adc_disable(&adc_instance);
}


void get_value_VMPPT (uint32_t *value, uint32_t *converted)
{
    adc_start_conversion(&adc_instance);
    
    uint32_t adc_reading, reading;
    
    do {
        /* Wait for conversion to be done and read out result */
    } while (adc_read(&adc_instance, &adc_reading) == STATUS_BUSY);        reading = (((D_ADC_VREF * adc_reading) / D_ADC_RESOLUTION) * (VMPPT_PULL_UP + VMPPT_PULL_DOWN) / VMPPT_PULL_DOWN);
    
    *value = adc_reading;
    *converted = reading;
    
    return;
}


void get_value_TEMP (uint32_t *value, uint32_t *converted)
{
    adc_start_conversion(&adc_instance);
    
    uint32_t adc_reading, reading;
    
    do {
        /* Wait for conversion to be done and read out result */
    } while (adc_read(&adc_instance, &adc_reading) == STATUS_BUSY);        reading = D_ADC_VREF * adc_reading / D_ADC_RESOLUTION;        uint32_t temp = (1858300 - 1000 * reading) / 1167; // + 2731; // Remove the earlier commentary for convertion to Kelvin    
    *value = adc_reading;
    *converted = temp;
    
    return;
}


