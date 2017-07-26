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

#include "main.h"



//=============================================================================
// Local Prototypes
//=============================================================================

//void received_bt(void);

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
        
          if (tick_elapsed(bcap_timer) % 900 == 0)
          {
              bcap_timer = get_tick();
              
              if (detect_bcap_touch())
              {
                  //#ifdef DBG_MODE
                  //printf("\nTOUCH OK - Counter: %d\n", bcap_counter);
                  //#endif
                  if (bcap_counter > 0xFFFF) 
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
                  
                    change_light_state(next_mode);
                  }                  
                  
                  
              }
              else
              {
                  port_pin_toggle_output_level(LED_RED_PIN);
                  E_LIGHT_MODE next_mode = light_state.mode;
              }
              
              uint8_t buf[12];
              sprintf(buf, "<b=%u>", bcap_counter);
              bt_usart_write_job(buf, 12);
              
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
    
    port_pin_toggle_output_level(LED_GREEN_PIN);
    
// USART
    // Configuration
    configure_usart();
    configure_usart_callbacks();

#ifdef DBG_MODE
printf("\n\nOCTO Board - %s, %s\n\n", __DATE__, __TIME__);
#endif

// DAC - LED stripe
    configure_dac();
    light_state.mode = E_LIGHT_ON;
    light_state.freq = E_LIGHT_MEDIUM;
    light_state.led_rising = false;
    light_state.led_bright = LIGHT_MAX;
    change_light_state(light_state.mode);
    
    
// RTC - Tick (1ms)
    configure_rtc_count();
    
//ADC - VMPPT & TEMP
    // Initial configuration and read of the Internal ADC - Copy and paste this code into the function of reading
    uint32_t adc_reading = 0, reading = 0;

    configure_adc_VMPPT();
    
    get_value_VMPPT(&adc_reading, &reading);
#ifdef DBG_MODE
printf("VMPPT ADC Read: %d \t|\t converted: %d mV\n", adc_reading, reading);
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

    send_board_info();
}


//=============================================================================
//! \brief  "Translation" of the CapacitiveSensor Arduino's Library
//=============================================================================
bool detect_bcap_touch(void)
{
    bool ok = true;
    bcap_counter = 0;
    
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
    
    delay_us(100);
    
    // receivePin to input (pullups are off)
    pin_conf.direction  = PORT_PIN_DIR_INPUT;                          
    pin_conf.input_pull = PORT_PIN_PULL_NONE;
    port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
    
    // sendPin High
    port_pin_set_output_level(BCAP_ENABLE_PIN, BCAP_ENABLE_ACTIVE); 
    
    // Wait for the detection
    while ((ioport_get_pin_level(DRIVER_BCAP_PIN) != DRIVER_BCAP_ACTIVE) && (bcap_counter < BCAP_THRESOLD))
    {
        bcap_counter++;
    }
    
    // Check if was not a touch - timeout
    if (bcap_counter > BCAP_THRESOLD)
    {
        return false;
    }        
        
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
    while ((ioport_get_pin_level(DRIVER_BCAP_PIN) == DRIVER_BCAP_ACTIVE) && (bcap_counter < BCAP_THRESOLD))
    {
        bcap_counter++;
    }
        
    // Check if was not a touch - timeout
    if (bcap_counter >= BCAP_THRESOLD)
    {
        ok = false;

        #ifdef DBG_MODE
        printf("\nNOT TOUCH\n");
        #endif
    }
    
    return ok;
}

//=============================================================================
//! \brief  Update the light struct
//=============================================================================
void change_light_state(E_LIGHT_MODE new_mode)
{
    light_state.mode = new_mode;
    
    //if (light_state.mode <= E_LIGHT_ON)
    //{
        //light_state.led_rising = light_state.mode;
        //light_state.led_bright = 0;
        //
        //turn_lights(light_state.mode);
    //}
}

void change_light_freq(E_LIGHT_FREQ new_freq)
{
    light_state.freq = new_freq;
}

void change_light_bright(uint16_t perthousand)
{
    light_state.led_bright = perthousand;
}

//=============================================================================
//! \brief  "Drive" the LED Stripe
//=============================================================================
void drive_light()
{
    if (light_state.mode > E_LIGHT_ON)
    {
        if (tick_elapsed(led_timer) % (light_state.freq*5) == 0)
        {
            led_timer = get_tick();
            
            if (light_state.mode == E_LIGHT_FADE)
            {
                set_led_bright_perthousand(light_state.led_bright);
            }
            if (update_bright())
            {
                turn_lights(light_state.led_rising);
            }
        }
    }
    else if (light_state.mode == E_LIGHT_ON)
    {
        turn_lights(true);
    }
    else
    {
        turn_lights(false);
    }
}

//=============================================================================
//! \brief Update the level of the brightness
//=============================================================================
bool update_bright()
{   
    bool cycle_complete = false;
         
    if (light_state.led_rising)
    {
        light_state.led_bright++;
        
        if (light_state.led_bright >= LIGHT_MAX-1)
        {
            light_state.led_rising = false;
            cycle_complete = true;
        }
    }
    else
    {
        light_state.led_bright--;
        
        if (light_state.led_bright <= LIGHT_MIN-1)
        {
            light_state.led_rising = true;
            cycle_complete = true;
        }
    }
    
    return cycle_complete;
}


//=============================================================================
//! \brief Turn ON/OFF the LED Stripe
//! \param[in] bool on The on/off - TRUE = ON | FALSE = OFF
//=============================================================================
void turn_lights(bool on)
{
    if (on)
    {
        set_led_bright_perthousand(LIGHT_MAX);
    }
    else
    {
        set_led_bright_perthousand(0);
    }
}

