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

	/* Insert application code here, after the board has been initialized. */
    
    configure_OCTO_peripheral();
    uint8_t led_bright = 0;
    
    //! [main_loop]
    while (true) {
        
        set_led_bright_percent(led_bright);
        
        if (++led_bright >= 99) 
        {
            led_bright = 0;
        }
        
        //! [main_loop]
        //! [main_read]
       // usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
        //! [main_read]
    }
}


void configure_OCTO_peripheral()
{
    // Led Green On
    port_pin_set_output_level(LED_GREEN_PIN, LED_GREEN_ACTIVE);
    
    // 
    configure_usart();
    configure_usart_callbacks();

    // Enable global interrupts
    system_interrupt_enable_global();

// Debug USART - Header transmitting
#ifdef DBG_MODE
    uint8_t string[] = "\nOCTO Board - ";
    dbg_usart_write(string);
    uint8_t string_date[] = __DATE__;
    dbg_usart_write(string_date);
    uint8_t string_time[] = __TIME__;
    dbg_usart_write(string_time);
#endif
    
    // Configure I²C device and enable
    configure_gas_gauge();
    
    // Configure the DAC - LED stripe
    configure_dac();
    configure_dac_channel();
}