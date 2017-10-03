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
//! \brief Main Function.
//=============================================================================
int main (void)
{	
	system_init();
	
	// Configure all the peripherals for the OCTO Board
	configure_OCTO_peripheral();
    
    while (true)
    {
        drive_light();
        bt_usart_receive_job();
                
        if (tick_elapsed(bt_timer) % 2000 == 0) //1000 ~= 2 secs
        {
            bt_timer = get_tick();
            delay_ms(1);
            
            battery_level = get_gauge_percent();
            
            if (battery_level <= light_state.low_power_threshold)
            {
                if (!batt_reached_low_power)
                {
                    enter_low_power_mode();
                }
            } 
            else if (batt_reached_low_power)
            {
                exit_low_power_mode();
            }
            
            if (bt_connected)
            {                
                if (low_power_update_app)
                {
                    low_power_update_app = false;
                    uint8_t light_update[8];
                    sprintf(light_update, "<U;P=%1u;>", batt_reached_low_power);
                    bt_usart_write_job(light_update, 8);
                }
                else if (bcap_update_app)
                {
                    bcap_update_app = false;
                    uint8_t light_update[8];
                    sprintf(light_update, "<U;L=%1u;>", light_state.mode);
                    bt_usart_write_job(light_update, 8);
                }
                else
                {
                    uint8_t buf[8]; //13 //20
                    //sprintf(buf, "<B=%3u;T=%2u;>", get_battery_percent(), get_temperature_celsius());
                    //sprintf(buf, "<B=%3u;>", get_battery_percent());
                    get_battery_percent();
                    //sprintf(buf, "<B=%3u;V=%4u;T=%2u;>", get_gauge_percent(), get_battery_percent(), get_temperature_celsius());
                    sprintf(buf, "<B=%3u;>", battery_level);
                    bt_usart_write_job(buf, 8); //13 //20
                }
                
                bt_poll_check();
            } else if (poll_requested)
            {
				poll_requested = false;
				change_bt_enable(true);
            }
        }
        
        if (bcap_enable)
        {
            if (tick_elapsed(bcap_timer) % 100 == 0)
            {
                bcap_timer = get_tick();
                delay_ms(1);
            
                if (ioport_get_pin_level(INPUT_BCAP_PIN) == INPUT_BCAP_ACTIVE)
                {
                    if (bcap_touch_counter < BCAP_THRESOLD_COUNTER * 2)
                    {
                        bcap_touch_counter++;
                    
                        if (bcap_touch_counter == BCAP_THRESOLD_COUNTER)
                        {
                            if (bt_connected)
							{
								bcap_update_app = true;	
							}
                            if (light_state.mode == E_LIGHT_OFF)
                            {
                                change_light_mode(light_bcap_previous_mode);
                            }
                            else
                            {
                                light_bcap_previous_mode = light_state.mode;
                                change_light_mode(E_LIGHT_OFF);
                            }
                        }
                    }
                }
                else
                {
                    bcap_touch_counter = 0;
                }
            }
        }
    }
}



//=============================================================================
//! \brief OCTO Peripheral's configuration
//=============================================================================
void configure_OCTO_peripheral()
{
// Init system to measure battery level
	system_board_init_for_battery_measure();
	
    delay_init();
    system_interrupt_enable_global();
	
//Flags
	battery_level = 0;
	bcap_touch_counter = 0;
	bcap_notouch_counter = 0;
	strobe_counter = 0;
	poll_requested = false;
	
	low_power_update_app = false;
	bcap_enable = true;
	bcap_update_app = false;
	batt_reached_max = false;
	batt_reached_low_power = false;
	//TBD
	sos_mode = false;
	activated = false;
	
// DAC - LED stripe
	configure_dac();
	light_state.mode = E_LIGHT_OFF;
	light_state.freq = E_LIGHT_MEDIUM;
	light_state.low_power_threshold = 10;
	light_state.led_rising = false;
	light_state.led_bright = LIGHT_MIN;
	light_state.led_max_bright = LIGHT_MAX/2;
	light_state.led_low_power_time = LOW_POWER_LIGHT_ON_TIME;
	light_bcap_previous_mode = E_LIGHT_ON;
	fade_bright_step = light_state.led_max_bright/LIGHT_MIN;

//ADC - VMPPT & TEMP
	// Initial configuration and read of the Internal ADC - Copy and paste this code into the function of reading
	uint32_t adc_reading = 0, reading = 0;

// Measure battery
	battery_level = get_battery_percent();

// Temp is not used yet
// This legacy code is for future usage
#ifdef DBG_MODE
turn_off_adc();
configure_adc_TEMP();
	
get_value_TEMP(&adc_reading, &reading);
#endif

// After battery measurement, init to run
	system_board_init();

// I²C - Gas Gauge
	configure_gas_gauge(battery_level);
	battery_level = get_gauge_percent();
	
#ifdef DBG_MODE
if (gas_gauge_read(&adc_reading, &reading))
{
	printf("Gas gauge read: %d \t|\t percent: %d\n", adc_reading, reading);		
}
#endif

// USART
	// Configuration
	configure_usart();
	configure_usart_callbacks();
	
#ifdef DBG_MODE
printf("\n\nOCTO Board - %s, %s\n\n", __DATE__, __TIME__);
#endif

// RTC - Tick (1ms)
    configure_rtc_count();
	
// Port pins - Capacitive Button
	port_get_config_defaults(&pin_conf);    
}

//=============================================================================
//! \brief  Update the light struct
//=============================================================================
void change_light_mode(E_LIGHT_MODE new_mode)
{
    light_state.mode = new_mode;
	light_state.led_rising = false;
	strobe_counter = 0;
}

void change_light_freq(E_LIGHT_FREQ new_freq)
{
    light_state.freq = new_freq;
}

void change_light_bright(uint16_t perthousand)
{
    light_state.led_max_bright = perthousand;
	fade_bright_step = light_state.led_max_bright/LIGHT_MIN;
}

void change_light_threshold(uint16_t new_threshold)
{
    light_state.low_power_threshold = new_threshold;
}

//=============================================================================
//! \brief Low Power functions
//=============================================================================
void enter_low_power_mode()
{    
    low_power_timer = get_tick;
    
    change_light_mode(E_LIGHT_ON);
    change_light_bright(LIGHT_MAX / 2);
	change_light_freq(E_LIGHT_MEDIUM);
	light_state.led_low_power_time = LOW_POWER_LIGHT_ON_TIME;
    
    batt_reached_low_power = true;
	if (bt_connected)
	{
		low_power_update_app = true;
	}
	
	delay_ms(1);
}

void exit_low_power_mode()
{
    change_light_mode(E_LIGHT_ON);
    change_light_bright(LIGHT_MAX / 2);
    
    batt_reached_low_power = false;
    if (bt_connected)
    {
	    low_power_update_app = true;
    }
}

void manage_low_power_light()
{    
    if (tick_elapsed(low_power_timer) % light_state.led_low_power_time == 0)
    {
        if (light_state.mode == E_LIGHT_ON)
        {
            change_light_mode(E_LIGHT_STROBE);
			light_state.led_low_power_time = LOW_POWER_LIGHT_STROBE_TIME;
        }
        else if (light_state.mode == E_LIGHT_STROBE)
        {
            change_light_mode(E_LIGHT_ON);
			light_state.led_low_power_time = LOW_POWER_LIGHT_ON_TIME;
        }
		
		low_power_timer = get_tick();
		delay_us(500);
    }
}

//=============================================================================
//! \brief  "Drive" the LED Stripe
//=============================================================================
void drive_light()
{	
	if (batt_reached_low_power)
	{
		manage_low_power_light();
	}

    if (light_state.mode == E_LIGHT_ON)
    {
        turn_lights(true);
    }
    else if (light_state.mode == E_LIGHT_OFF)
    {
        turn_lights(false);
    }
    else
    {
        if (tick_elapsed(led_timer) % ((light_state.freq * 5) + 9) == 0)
        {
			led_timer = get_tick();
            
            if (light_state.mode == E_LIGHT_FADE)
            {
                set_led_bright_perthousand(light_state.led_bright);
				update_bright();
            }
            
            if (light_state.mode == E_LIGHT_STROBE)
            {
				if (strobe_counter > LIGHT_MAX/2)
				{
					strobe_counter = 0;
					light_state.led_rising = !light_state.led_rising;
					turn_lights(light_state.led_rising);
				}
				else
				{
					strobe_counter++;
				}
            }
        }
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
        light_state.led_bright += fade_bright_step;
        
        if (light_state.led_bright >= light_state.led_max_bright - fade_bright_step)
        {
            light_state.led_rising = false;
            cycle_complete = true;
        }
    }
    else
    {
        light_state.led_bright -= fade_bright_step;
        
        if (light_state.led_bright <= LIGHT_MIN + fade_bright_step)
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
        set_led_bright_perthousand(light_state.led_max_bright);
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
    // The poll_requested flag turns false into OCTO_USART.c file
	//when receive the poll response message, when BT connects and is turned off after lose connection
    // If run this function and it stills true, the board didn't received the response
    if (poll_requested) 
    {
        bt_connected = false;
		
		change_bt_enable(false);
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
    uint32_t batt_value = 0;
    
    configure_adc_VMPPT();
    
    get_value_VMPPT(&adc_reading, &reading);
    
    if (reading > BATT_MAX)
    {
        if (!batt_reached_max)
        {
            batt_reached_max = true;
            
            // Sign to Gas Gauge that the Charge is Complete
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            port_pin_set_config(GAUGE_CC_ENABLE_PIN, &pin_conf);
            port_pin_set_output_level(GAUGE_CC_ENABLE_PIN, GAUGE_CC_ENABLE_ACTIVE);
        }
        
        batt_value = 100;
    }
    else
    {
        if (batt_reached_max)
        {
            batt_reached_max = false;
            
            // Sign to Gas Gauge that the Charge is Complete
            pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
            port_pin_set_config(GAUGE_CC_ENABLE_PIN, &pin_conf);
            port_pin_set_output_level(GAUGE_CC_ENABLE_PIN, GAUGE_CC_ENABLE_INACTIVE);
        }
        
        if (reading < BATT_MIN)
        {
            batt_value = 0;
        }
        else
        {
            batt_value = ((reading-BATT_MIN) * 100) / (BATT_MAX - BATT_MIN);
        }
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