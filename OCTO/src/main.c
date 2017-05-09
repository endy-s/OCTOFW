/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>

#include "OCTO_ADC.h"
#include "OCTO_I2C.h"
#include "OCTO_USART.h"
#include "OCTO_DAC.h"
#include "OCTO_RTC.h"


//====================================================
// Local Prototypes
//====================================================
void configure_OCTO_peripheral(void);

//====================================================
// Local Defines
//====================================================


//=============================================================================
//! \brief Main Function.
//=============================================================================
int main (void)
{
	system_init();
    
    delay_init();
    
    system_interrupt_enable_global();

	/* Insert application code here, after the board has been initialized. */
    
    // Configure all the peripherals for the OCTO Board
    configure_OCTO_peripheral();
    // Used with the DAC for the led brightness
    uint32_t led_bright = 0;
    uint16_t led_test = 0;
    
    // RTC timing
    uint32_t timer = get_tick();
    
    while (true) {
        
        if (tick_elapsed(timer) % 1000 == 0)
        {
            port_pin_toggle_output_level(LED_RED_PIN);
            set_led_bright_percent(0);
            
            if (++led_bright >= 99)
            {
                led_bright = 0;
            }
        }
        
        //! [main_loop]
        //! [main_read]
        bt_usart_receive_job();
        //! [main_read]
    }
}


void configure_OCTO_peripheral()
{
    // Led Red On
    port_pin_set_output_level(LED_RED_PIN, LED_RED_ACTIVE);
    
    // Enable global interrupts
    system_interrupt_enable_global();
    
    // USART Configuration
    configure_usart();
    configure_usart_callbacks();

    // Debug USART - Header transmitting
#ifdef DBG_MODE
    printf("\n\nOCTO Board - %s, %s\n\n", __DATE__, __TIME__);
#endif

    bt_usart_write_job("1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|");
    bt_usart_write_job("1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|");
    bt_usart_write_job("1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|1234567890|");
    
    // Configure I²C device and enable
    configure_gas_gauge();
    
    // Configure the DAC - LED stripe
    configure_dac();
    configure_dac_channel();
        
    // Configure the RTC - Used as Tick (1ms)
    configure_rtc_count();
}