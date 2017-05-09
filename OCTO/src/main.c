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
    uint16_t led_bright = 0;
    bool     led_rising = true;
    
    // RTC timing
    uint32_t timer = get_tick();
    
    while (true) {
        
        if (tick_elapsed(timer) % 1000 == 0)
        {
            
            //if (led_rising)
            //{
                //if (led_bright >= 950)
                //{
                    //led_rising = false;
                //}
                //else
                //{
                    //led_bright += 5;
                //}
            //}
            //else
            //{
                //if (led_bright <= 50)
                //{
                    //led_rising = true;
                //}
                //else
                //{
                    //led_bright -= 5;
                //}
            //}
            
            uint32_t adc_reading = 0, reading = 0;
            
            if (gas_gauge_read(&adc_reading, &reading))
            {
                printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);
            }
            
            port_pin_toggle_output_level(LED_RED_PIN);            
        }
        
        bt_usart_receive_job();
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

    // Configure the DAC - LED stripe
    configure_dac();

    // Initial state of the LED stripe
    set_led_bright_percent(0);
       
    // Configure the RTC - Used as Tick (1ms)
    configure_rtc_count();

// Variables for testing!
uint32_t adc_reading = 0, reading = 0;

    // Configure I²C device and enable
    configure_gas_gauge();
    if (gas_gauge_read(&adc_reading, &reading))
    {
        printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);
    }
    

    // Initial configuration and read of the Internal ADC - Copy and paste this code into the function of reading
    

    configure_adc_VMPPT();
    
    get_value_VMPPT(&adc_reading, &reading);
#ifdef DBG_MODE
    printf("VMPPT ADC Read: %d \t|\t converted: %dV\n", adc_reading, reading);
#endif
    
    turn_off_adc();
    
    
    configure_adc_TEMP();
      
    get_value_TEMP(&adc_reading, &reading);
#ifdef DBG_MODE
    printf("TEMP ADC Read: %d \t|\t converted: %d.%dC\n", adc_reading, reading/10, reading%10);
#endif
}