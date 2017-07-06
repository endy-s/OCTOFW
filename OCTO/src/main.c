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

void bt_poll_check(void);
uint32_t get_battery_percent(void);
uint32_t get_temperature_celsius(void);
uint32_t get_gauge_percent(void);

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
    //bcap_timer = get_tick();                              //TODO verify if works without this and remove
    bcap_touch_counter = 0;
    bcap_notouch_counter = 0;
    bcap_low = 0;
    bcap_high = 0x000FFFFF;
    bcap_limit_temp = 0;
    bcap_calibrate_counter = 0;
    sos_mode = false;
    activated = false;
    
    while (true)
    {  
        drive_light();
        bt_usart_receive_job();
        
        if (tick_elapsed(bt_timer) % 5000 == 0)
        {           
            bt_timer = get_tick();
            
            delay_us(1000);
            
            if (bt_connected)
            {
                uint8_t buf[8]; //13
                //sprintf(buf, "<B=%3u;T=%2u;>", get_battery_percent(), get_temperature_celsius());
                //sprintf(buf, "<B=%3u;>", get_battery_percent());
                get_battery_percent();
                sprintf(buf, "<B=%3u;>", get_gauge_percent());
            
                bt_poll_check();
            
                bt_usart_write_job(buf, 8); //13
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
    change_light_state(light_state.mode, false);
    
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
    configure_gas_gauge();
    if (gas_gauge_read(&adc_reading, &reading))
    {
#ifdef DBG_MODE
        printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);
#endif
    }
}


//=============================================================================
//! \brief  "Translation" of the CapacitiveSensor Arduino's Library (same comments)
//=============================================================================
bool detect_bcap_touch(void)
{
    bool ok = true;
    bcap_counter = 0;
    
    // receivePin to OUTPUT
    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(DRIVER_BCAP_PIN, &pin_conf);
    // pin is now LOW AND OUTPUT
    port_pin_set_output_level(DRIVER_BCAP_PIN, BCAP_ENABLE_INACTIVE);
    
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
    while ((ioport_get_pin_level(DRIVER_BCAP_PIN) != DRIVER_BCAP_ACTIVE) && (bcap_counter < bcap_high))
    {
        bcap_counter++;
    }
    
    // Check if was a touch - timeout
    if (bcap_counter > bcap_high)
    {
        return ok;
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
    while ((ioport_get_pin_level(DRIVER_BCAP_PIN) == DRIVER_BCAP_ACTIVE) && (bcap_counter < bcap_high))
    {
        bcap_counter++;
    }
        
    // Check if was not a touch
    if (bcap_counter < bcap_low)
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
void change_light_state(E_LIGHT_MODE new_mode, bool update_app)
{
    light_state.mode = new_mode;
    if (update_app) 
    {
        bt_send_light_update();
    }
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

//=============================================================================
//! \brief Check the BT coonection using a logic related to messages of polling
//=============================================================================
void bt_poll_check()
{
    //poll_requested turns false into OCTO_USART.c file
    //If it stills true, the board didn't received the response
    if (poll_requested) 
    {
        bt_connected = false;
        poll_requested = false;
        port_pin_toggle_output_level(LED_RED_PIN);
        port_pin_toggle_output_level(LED_GREEN_PIN);
    }
    else 
    {
        poll_requested = true;
    }
}


//=============================================================================
//! \brief Get the Battery info
//=============================================================================
uint32_t get_battery_percent()
{   
    uint32_t adc_reading = 0, reading = 0;
    uint8_t batt_value = 0;
    
    turn_off_adc();
    configure_adc_VMPPT();
    
    get_value_VMPPT(&adc_reading, &reading);
    
    if (reading > BATT_MAX)
    {
        // Sign to Gas Gauge that the Charge is Complete
        pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
        port_pin_set_config(GAUGE_CC_ENABLE_PIN, &pin_conf);
        port_pin_set_output_level(GAUGE_CC_ENABLE_PIN, GAUGE_CC_ENABLE_INACTIVE);
        
        batt_value = 100;
    }
    else if (reading < BATT_MIN)
    {
        batt_value = 0;
    }
    else
    {
        batt_value = ((reading-BATT_MIN) * 100) / (BATT_MAX - BATT_MIN);
    }
    
    return batt_value;
}


//=============================================================================
//! \brief Get the Temperature info
//=============================================================================
uint32_t get_temperature_celsius()
{
    uint32_t adc_reading = 0, reading = 0;
    uint8_t temp_value = 0;
    
    turn_off_adc();
    configure_adc_TEMP();
    
    get_value_TEMP(&adc_reading, &reading);
    
    temp_value = reading/10;
    
    return temp_value;
}

//=============================================================================
//! \brief Get the Gas Gauge's Battery percent info
//=============================================================================
uint32_t get_gauge_percent()
{
    uint32_t i2c_reading = 0, percent = 0;
    
    gas_gauge_read(&i2c_reading, &percent);
    
    return percent;
}