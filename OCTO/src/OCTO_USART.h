/*
 * OCTO_USART.h
 *
 * Created: 27/04/2017 23:29:38
 *  Author: Endy
 */ 


#ifndef OCTO_USART_H_
#define OCTO_USART_H_

#include <asf.h>

#define DBG_MODE

bool bt_connected;

struct usart_module bt_usart_instance;

//====================================================
// Function Prototypes
//====================================================

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);
void clean_array(int length);

void configure_usart(void);
void configure_usart_callbacks(void);

void bt_usart_write_job(uint8_t *string, uint16_t length);
void bt_usart_receive_job(void);

void bt_received(uint8_t* received_msg);
void bt_start_setup(void);
void bt_request_polling(void);
void bt_send_light_update(void);



#endif /* OCTO_USART_H_ */