/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
    
// Configure the board after the battery measurement
// Don't need to configure: 
// Red LED pins, GG and BT enable and BCAP input
    struct port_config pin_conf;
    port_get_config_defaults(&pin_conf);

    // Configure LEDs as outputs, turn them off 
    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;    
    port_pin_set_config(LED_GREEN_PIN, &pin_conf);
    port_pin_set_output_level(LED_GREEN_PIN, LED_GREEN_ACTIVE);

    // Configure the Enable of LED Stripe as output, turn it on 
    port_pin_set_config(LED_DRIVER_PIN, &pin_conf);
    port_pin_set_output_level(LED_DRIVER_PIN, LED_DRIVER_ACTIVE);
	
    // Configure the USB Charge external device Enable
	port_pin_set_config(USB_EN_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(USB_EN_ENABLE_PIN, USB_EN_ENABLE_ACTIVE);
}

void system_board_init_for_battery_measure(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
    
    struct port_config pin_conf;
    port_get_config_defaults(&pin_conf);

    // Configure LEDs as outputs, turn them off 
    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(LED_RED_PIN, &pin_conf);
    port_pin_set_output_level(LED_RED_PIN, LED_RED_INACTIVE);
    
    port_pin_set_config(LED_GREEN_PIN, &pin_conf);
    port_pin_set_output_level(LED_GREEN_PIN, LED_GREEN_INACTIVE);

    // Configure the Enable of LED Stripe as output, turn it on 
    port_pin_set_config(LED_DRIVER_PIN, &pin_conf);
    port_pin_set_output_level(LED_DRIVER_PIN, LED_DRIVER_INACTIVE);
    
    // Configure the Enable of BT Module as output, turn it on
    port_pin_set_config(BT_ENABLE_PIN, &pin_conf);
    port_pin_set_output_level(BT_ENABLE_PIN, BT_ENABLE_ACTIVE);
    
    // Configure the Charge Complete Pin for the Gas Gauge Module as output, turn it off
    port_pin_set_config(GAUGE_CC_ENABLE_PIN, &pin_conf);
    port_pin_set_output_level(GAUGE_CC_ENABLE_PIN, GAUGE_CC_ENABLE_INACTIVE);
	
	// Configure the USB Charge external device Disabled
	port_pin_set_config(USB_EN_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(USB_EN_ENABLE_PIN, USB_EN_ENABLE_INACTIVE);

	/* Set buttons as inputs */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = PORT_PIN_PULL_NONE;
	port_pin_set_config(INPUT_BCAP_PIN, &pin_conf);
}