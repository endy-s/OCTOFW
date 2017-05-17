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
void configure_extint_channel(void);
void extint_detection_callback(void);

//====================================================
// Local Variables
//====================================================
uint32_t bcap_counter = 0;

struct port_config pin_conf;



void configure_extint_channel(void)
{
    //struct extint_chan_conf config_extint_chan;
    //extint_chan_get_config_defaults(&config_extint_chan);
    //config_extint_chan.gpio_pin = BCAP_RX_PIN;
    //config_extint_chan.gpio_pin_mux = BCAP_RX_MUX;
    //config_extint_chan.gpio_pin_pull = EXTINT_PULL_NONE;
    //config_extint_chan.detection_criteria = EXTINT_DETECT_HIGH;
    //extint_chan_set_config(BCAP_RX_INPUT, &config_extint_chan);
       //
    //extint_register_callback(extint_detection_callback, BCAP_RX_INPUT, EXTINT_CALLBACK_TYPE_DETECT);
    //extint_chan_enable_callback(BCAP_RX_INPUT, EXTINT_CALLBACK_TYPE_DETECT);
}void extint_detection_callback(void)
{
    //bool pin_state = port_pin_get_input_level(BCAP_RX_PIN);
    //if (pin_state)
    //{
        
        
    //}
    //else
    //{
        port_pin_toggle_output_level(LED_GREEN_PIN);
        
        printf("\nCounter Value: %d", tick_elapsed(bcap_counter));
        
        bcap_counter = get_tick();
    //}
    
    port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_INACTIVE);
}


//=============================================================================
//! \brief Main Function.
//=============================================================================
int main (void)
{
	system_init();
        
    // Configure all the peripherals for the OCTO Board
    configure_OCTO_peripheral();        delay_init();
    // Used with the DAC for the led brightness
    uint16_t led_bright = 0;
    bool     led_rising = true;
    
    // RTC timing
    uint32_t timer = get_tick();
    
    while (true) 
    {
        if (tick_elapsed(timer) % 2000 == 0)
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
            
             bcap_counter = 0;
            
            port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_INACTIVE);  // sendPin Register low
             
            pin_conf.direction  = PORT_PIN_DIR_INPUT;                          // receivePin to input (pullups are off)
            pin_conf.input_pull = PORT_PIN_PULL_NONE;
            port_pin_set_config(BUTTON_0_PIN, &pin_conf);
             
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            port_pin_set_config(BUTTON_0_PIN, &pin_conf);                       // receivePin to OUTPUT      
            port_pin_set_output_level(BUTTON_0_PIN, BCAP_ENABLE_INACTIVE);     // pin is now LOW AND OUTPUT
 
            delay_us(10);                                                      
 
            /* Set buttons as inputs */
            pin_conf.direction  = PORT_PIN_DIR_INPUT;                          // receivePin to input (pullups are off)
            pin_conf.input_pull = PORT_PIN_PULL_NONE;
            port_pin_set_config(BUTTON_0_PIN, &pin_conf);
            
            port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_ACTIVE); // sendPin High
             
            while ((ioport_get_pin_level(BUTTON_0_PIN) != BUTTON_0_ACTIVE) && (bcap_counter < 0xFFFFFFFF))
            {
                bcap_counter++;
            } 
            
            //if (bcap_counter > 0xFFFFFFFF)
            //{
                //
            //}                
             
            // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            port_pin_set_config(BUTTON_0_PIN, &pin_conf);                       
            port_pin_set_output_level(BUTTON_0_PIN, BCAP_ENABLE_ACTIVE);     // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
             
            /* Set buttons as inputs */
            pin_conf.direction  = PORT_PIN_DIR_INPUT;                          
            pin_conf.input_pull = PORT_PIN_PULL_NONE;
            port_pin_set_config(BUTTON_0_PIN, &pin_conf);	                    // receivePin to INPUT (pullup is off)
                       
            port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_INACTIVE);  // sendPin LOW
             
            while ((ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) && (bcap_counter < 0xFFFFFFFF))
            {
                bcap_counter++;
            }
             
            if (bcap_counter < 0xFFFFFFFF)
            {
                port_pin_toggle_output_level(LED_GREEN_PIN);
                printf("\nbcap counter = %d | %d\n", bcap_counter, timer);
            }
            
             
            
            //uint32_t adc_reading = 0, reading = 0;            
            
            //if (gas_gauge_read(&adc_reading, &reading))
            //{
                //printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);
            //}
            
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
    printf("VMPPT ADC Read: %d \t|\t converted: %d.%d V\n", adc_reading, reading/1000, reading%1000);
#endif
    
    turn_off_adc();
    
    
    configure_adc_TEMP();
      
    get_value_TEMP(&adc_reading, &reading);
#ifdef DBG_MODE
    printf("TEMP ADC Read: %d \t|\t converted: %d.%d C\n", adc_reading, reading/10, reading%10);
#endif

    //turn_off_adc();
//
//
    //configure_adc_BCAP();
//
    //get_value_BCAP(&adc_reading, &reading);
    //#ifdef DBG_MODE
    //printf("BCAP ADC Read: %d \t|\t converted: %d\n", adc_reading, reading);
    //#endif
//
    
    port_get_config_defaults(&pin_conf);
}