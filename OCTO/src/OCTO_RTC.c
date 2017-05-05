/*
 * OCTO_RTC.c
 *
 * Created: 04/05/2017 21:24:30
 *  Author: Endy
 */ 

#include "OCTO_RTC.h"

struct rtc_module rtc_instance;

void configure_rtc_count()
{
    struct rtc_count_config config_rtc_count;
    
    rtc_count_get_config_defaults(&config_rtc_count);
    
    config_rtc_count.prescaler = RTC_COUNT_PRESCALER_DIV_1;
    
#ifdef FEATURE_RTC_CONTINUOUSLY_UPDATED
    config_rtc_count.continuously_update = true;
#endif

    rtc_count_init(&rtc_instance, RTC, &config_rtc_count);
    rtc_count_enable(&rtc_instance);
}uint32_t get_tick(){    return rtc_count_get_count(&rtc_instance);}//=============================================================================
//! \brief Return the elapse milliseconds since the reference.
//!
//! \param[in] reference The reference time
//!
//! \return The number of milliseconds elapsed
//=============================================================================uint32_t tick_elapsed(uint32_t reference){    uint32_t now = rtc_count_get_count(&rtc_instance);        return  (now - reference);}