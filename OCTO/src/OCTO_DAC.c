/*
 * OCTO_DAC.c
 *
 * Created: 04/05/2017 18:26:37
 *  Author: Endy
 */ 

#include "OCTO_DAC.h"

#define DAC_FULL        (0x3FF)
#define DAC_LED_FULL    (0X1FF)

//
struct dac_module dac_instance;

//
void configure_dac()
{
    struct dac_config config_dac;
    dac_get_config_defaults(&config_dac);
    config_dac.reference = DAC_REFERENCE_INT1V;
    //
    dac_init(&dac_instance, DAC, &config_dac);
}

//
void configure_dac_channel()
{
    struct dac_chan_config config_dac_chan;
    dac_chan_get_config_defaults(&config_dac_chan);
    
    
    
    dac_chan_set_config(&dac_instance, DAC_CHANNEL_0, &config_dac_chan);
    dac_chan_enable(&dac_instance, DAC_CHANNEL_0);
}