/*
 * OCTO_DAC.c
 *
 * Created: 04/05/2017 18:26:37
 *  Author: Endy
 */ 

#include "OCTO_DAC.h"

#define DAC_FULL        (0x3FF)
#define DAC_LED_FULL    (DAC_FULL/3)

//
struct dac_module dac_instance;

//
void configure_dac()
{
    struct dac_config config_dac;
    dac_get_config_defaults(&config_dac);
    config_dac.reference = DAC_REFERENCE_INT1V;
    dac_init(&dac_instance, DAC, &config_dac);
    dac_enable(&dac_instance);
}//void set_led_bright_percent(uint16_t perthousand){    uint16_t led_data = (perthousand*DAC_LED_FULL)/1000;        dac_chan_write(&dac_instance, DAC_CHANNEL_0, led_data);}