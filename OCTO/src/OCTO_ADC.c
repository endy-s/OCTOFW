/*
 * OCTO_ADC.c
 *
 * Created: 27/04/2017 23:28:22
 *  Author: Endy
 */ 

#include "OCTO_ADC.h"


#define D_ADC_VREF                                  (3300)
#define D_ADC_RESOLUTION                            (1 << 10)

struct adc_module adc_instance;

//=============================================================================
//! \brief Init ADC peripheral.
//! \return TRUE if that ADC was successfully configured.
//=============================================================================

char configure_adc (void)
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