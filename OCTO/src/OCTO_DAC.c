/*
 * OCTO_DAC.c
 *
 * Created: 04/05/2017 18:26:37
 *  Author: Endy
 */ 

#include "OCTO_DAC.h"
#include "main.h"

#define DAC_FULL        (0x3FF)
#define DAC_LED_FULL    (DAC_FULL/3)

bool dac_enabled = true;

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
}//void set_led_bright_perthousand(uint16_t perthousand){    if (perthousand < LIGHT_MIN)    {        dac_enabled = false;        struct port_config pin_conf;
        port_get_config_defaults(&pin_conf);        // Configure the Enable of LED Stripe as output, turn it on
        port_pin_set_config(LED_DRIVER_PIN, &pin_conf);
        port_pin_set_output_level(LED_DRIVER_PIN, LED_DRIVER_INACTIVE);    }    else    {        if (!dac_enabled)        {            dac_enabled = true;            struct port_config pin_conf;
            port_get_config_defaults(&pin_conf);            // Configure the Enable of LED Stripe as output, turn it on
            port_pin_set_config(LED_DRIVER_PIN, &pin_conf);
            port_pin_set_output_level(LED_DRIVER_PIN, LED_DRIVER_ACTIVE);        }                bright_reference = perthousand;                uint16_t led_data = (perthousand*DAC_LED_FULL)/1000;                dac_chan_write(&dac_instance, DAC_CHANNEL_0, led_data);    }    }