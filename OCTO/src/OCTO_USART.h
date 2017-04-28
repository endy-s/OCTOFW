/*
 * OCTO_USART.h
 *
 * Created: 27/04/2017 23:29:38
 *  Author: Endy
 */ 


#ifndef OCTO_USART_H_
#define OCTO_USART_H_

#include <asf.h>

//====================================================
// Function Prototypes
//====================================================

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);




#endif /* OCTO_USART_H_ */