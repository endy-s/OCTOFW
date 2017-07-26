/*
 * OCTO_RTC.c
 *
 * Created: 04/05/2017 21:24:30
 *  Author: Endy
 */ 

#include "OCTO_RTC.h"

struct rtc_module rtc_instance;
uint32_t tick_counter = 0;

void configure_rtc_count()
{
    struct rtc_count_config config_rtc_count;
    
    rtc_count_get_config_defaults(&config_rtc_count);
    
    config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1;
    config_rtc_count.mode                = RTC_COUNT_MODE_16BIT;
    config_rtc_count.continuously_update = true;


    rtc_count_init(&rtc_instance, RTC, &config_rtc_count);
    rtc_count_enable(&rtc_instance);
    
    rtc_count_register_callback(&rtc_instance, rtc_overflow_callback, RTC_COUNT_CALLBACK_OVERFLOW);
    rtc_count_enable_callback(&rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW);
    rtc_count_set_period(&rtc_instance, 1);
}void rtc_overflow_callback(void)
{
    tick_counter++;
}uint32_t get_tick(){    return tick_counter;}//=============================================================================
//! \brief Return the elapse milliseconds since the reference.
//!
//! \param[in] reference The reference time
//!
//! \return The number of milliseconds elapsed
//=============================================================================uint32_t tick_elapsed(uint32_t reference){    return  (tick_counter - reference);}