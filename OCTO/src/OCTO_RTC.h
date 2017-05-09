/*
 * OCTO_RTC.h
 *
 * Created: 04/05/2017 21:24:42
 *  Author: Endy
 */ 


#ifndef OCTO_RTC_H_
#define OCTO_RTC_H_

#include <asf.h>

void configure_rtc_count(void);
uint32_t get_tick(void);
uint32_t tick_elapsed(uint32_t reference);


#endif /* OCTO_RTC_H_ */