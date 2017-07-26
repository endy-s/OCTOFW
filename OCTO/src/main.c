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

//=============================================================================
// Local Defines
//=============================================================================

#define LIGHT_MAX 990
#define LIGHT_MIN 100

//=============================================================================
// Local Variables
//=============================================================================
uint32_t led_timer, bcap_timer;
struct port_config pin_conf;

typedef enum
{
    E_LIGHT_OFF = 0,
    E_LIGHT_ON,
    E_LIGHT_FADE,
    E_LIGHT_STROBE,
} E_LIGHT_MODE;

typedef enum
{
    E_LIGHT_FAST        = 5,
    E_LIGHT_MEDIUM_FAST = 10,
    E_LIGHT_MEDIUM      = 15,
    E_LIGHT_MEDIUM_SLOW = 20,
    E_LIGHT_SLOW        = 25,
} E_LIGHT_FREQ;


typedef struct
{
    E_LIGHT_MODE mode;
    E_LIGHT_FREQ freq;
    uint16_t     led_bright;
    bool         led_rising;
} OCTO_LIGHT;

OCTO_LIGHT light_state;



//=============================================================================
// Local Prototypes
//=============================================================================
void configure_OCTO_peripheral(void);

bool detect_bcap_touch(void);

void change_light_state(E_LIGHT_MODE new_mode, E_LIGHT_FREQ new_freq);
void drive_light(void);
void turn_off_light(void);
void turn_on_light(void);



//=============================================================================
//! \brief Main Function.
//=============================================================================
int main (void)
{
    system_init();
    
    // Configure all the peripherals for the OCTO Board
    configure_OCTO_peripheral();    
    bcap_timer = get_tick();
    
    while (true)
    {
        drive_light();

        bt_usart_receive_job();
        
        if (tick_elapsed(bcap_timer) % 500 == 0)
        {
            bcap_timer = get_tick();
            
            if (detect_bcap_touch())
            {
                port_pin_toggle_output_level(LED_GREEN_PIN);
                
                E_LIGHT_MODE next_mode = light_state.mode;
                
                if (next_mode > E_LIGHT_FADE)
                {
                    next_mode = E_LIGHT_OFF;
                }
                else
                {
                    next_mode++;
                }
                
                change_light_state(next_mode, light_state.freq);
            }
        }
    }
}



//=============================================================================
//! \brief OCTO Peripheral's configuration
//=============================================================================
void configure_OCTO_peripheral()
{
    delay_init();
    system_interrupt_enable_global();
    
    port_pin_set_output_level(LED_RED_PIN, LED_RED_ACTIVE);
    
// USART
    // Configuration
    configure_usart();
    configure_usart_callbacks();

#ifdef DBG_MODE
printf("\n\nOCTO Board - %s, %s\n\n", __DATE__, __TIME__);
#endif

// DAC - LED stripe
    configure_dac();
    turn_off_light();
    change_light_state(E_LIGHT_FADE, E_LIGHT_MEDIUM);
    
    
// RTC - Tick (1ms)
    configure_rtc_count();
    
//ADC - VMPPT & TEMP
    // Initial configuration and read of the Internal ADC - Copy and paste this code into the function of reading
    uint32_t adc_reading = 0, reading = 0;

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

// Port pins - Capacitive Button
    port_get_config_defaults(&pin_conf);
    
// I²C - Gas Gaue
#ifdef DBG_MODE
configure_gas_gauge();
if (gas_gauge_read(&adc_reading, &reading))
{
    printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);
}
#endif    
}


//=============================================================================
//! \brief  "Translation" of the CapacitiveSensor Arduino's Library
//=============================================================================
bool detect_bcap_touch(void)
{
    bool ok = true;
    uint32_t bcap_counter = 0;
    
    // sendPin Register low
    port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_INACTIVE);
    
    // receivePin to Input (pullups are off)
    pin_conf.direction  = PORT_PIN_DIR_INPUT;
    pin_conf.input_pull = PORT_PIN_PULL_NONE;
    port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
    
    // receivePin to OUTPUT
    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
    // pin is now LOW AND OUTPUT 
    port_pin_set_output_level(DRIVER_BCAP_PIN, BCAP_ENABLE_INACTIVE);     
    
    delay_us(10);
    
    // receivePin to input (pullups are off)
    pin_conf.direction  = PORT_PIN_DIR_INPUT;                          
    pin_conf.input_pull = PORT_PIN_PULL_NONE;
    port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
    
    // sendPin High
    port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_ACTIVE); 
    
    // Wait for the detection
    while ((ioport_get_pin_level(DRIVER_BCAP_PIN) != DRIVER_BCAP_ACTIVE) && (bcap_counter < 0xFFFF))
    {
        bcap_counter++;
    }
    
    // Check if was not a touch - timeout
    if (bcap_counter < 0xFFFF)
    {
        // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
        pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
        port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
        // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
        port_pin_set_output_level(DRIVER_BCAP_PIN, BCAP_ENABLE_ACTIVE);
        
        // receivePin to INPUT (pullup is off)
        pin_conf.direction  = PORT_PIN_DIR_INPUT;
        pin_conf.input_pull = PORT_PIN_PULL_NONE;
        port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
        
        // sendPin LOW
        port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_INACTIVE);
        
        // Wait for the detection
        while ((ioport_get_pin_level(DRIVER_BCAP_PIN) == DRIVER_BCAP_ACTIVE) && (bcap_counter < 0xFFFF))
        {
            bcap_counter++;
        }
        
        // Check if was not a touch - timeout
        if (bcap_counter < 0xFFFF)
        {
            ok = false;
            
            #ifdef DBG_MODE
            printf("\nNOT TOUCH\n");
            #endif
        }
    }
    
    return ok;
}

//=============================================================================
//! \brief  Update the light struct
//=============================================================================
void change_light_state(E_LIGHT_MODE new_mode, E_LIGHT_FREQ new_freq)
{
    light_state.mode = new_mode;
    light_state.freq = new_freq;
    
    if (light_state.mode == E_LIGHT_OFF)
    {
        turn_off_light();
    }
    else if (light_state.mode == E_LIGHT_ON)
    {
        turn_on_light();
    }
    
    printf("\n NEW LIGHT MODE: %d\n", new_mode) ;
}

//=============================================================================
//! \brief  "Drive" the LED Stripe
//=============================================================================
void drive_light()
{
    if (light_state.mode > E_LIGHT_ON)
    {
        if (tick_elapsed(led_timer) % light_state.freq == 0)
        {
            led_timer = get_tick();
            
            set_led_bright_percent(light_state.led_bright);
            
            if (light_state.led_rising)
            {
                if (light_state.mode == E_LIGHT_FADE)
                {
                    if (light_state.led_bright >= LIGHT_MAX)
                    {
                        light_state.led_rising = false;
                    }
                    else
                    {
                        light_state.led_bright += 1;
                    }
                }
                else // if E_LIGHT_STROBE
                {
                    turn_on_light();
                }
            }
            else
            {
                if (light_state.mode == E_LIGHT_FADE)
                {
                    if (light_state.led_bright <= LIGHT_MIN)
                    {
                        light_state.led_rising = true;
                    }
                    else
                    {
                        light_state.led_bright -= 1;
                    }
                }
                else // if E_LIGHT_STROBE
                {
                    turn_off_light();
                }
                
            }
        }            
    }
}

//=============================================================================
//! \brief Turn OFF the LED Stripe
//=============================================================================
void turn_off_light()
{
    set_led_bright_percent(0);
    light_state.led_rising = true;
}

//=============================================================================
//! \brief Turn ON the LED Stripe
//=============================================================================
void turn_on_light()
{
    light_state.led_rising = false;
    set_led_bright_percent(LIGHT_MAX);
}