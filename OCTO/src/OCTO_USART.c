/*
 * OCTO_USART.c
 *
 * Created: 27/04/2017 23:29:30
 *  Author: Endy
 */ 

#include "OCTO_USART.h"


//====================================================
// USART - Local Variables
//====================================================

//! [module_inst]
struct usart_module bt_usart_instance;
struct usart_module dbg_usart_instance;
//! [module_inst]

//! [USART rx_buffer_var]
#define MAX_RX_BUFFER_LENGTH   5

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];
//! [USART rx_buffer_var]


//=============================================================================
//! \brief Callback Function for USART Read (BT).
//!
//! \param[in] usart_module - The USART module to receive the callback
//=============================================================================
void usart_read_callback(struct usart_module *const usart_module)
{
    usart_write_buffer_job(&bt_usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
    
    port_pin_toggle_output_level(LED_GREEN_PIN);
    
    usart_write_buffer_wait(&bt_usart_instance, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}

//=============================================================================
//! \brief Callback Function for USART Write (BT).
//!
//! \param[in] usart_module - The USART module to receive the callback
//=============================================================================
void usart_write_callback(struct usart_module *const usart_module)
{
    port_pin_toggle_output_level(LED_GREEN_PIN);
}

//=============================================================================
//! \brief Setup Function for USART (Debug and BT).
//=============================================================================
void configure_usart(void)
{
// General
    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);
    
// Debug USART
#ifdef DBG_MODE
    config_usart.baudrate    = 9600;
    config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;

    while (usart_init(&dbg_usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {}
    usart_enable(&dbg_usart_instance);
    
    stdio_serial_init(&dbg_usart_instance, EDBG_CDC_MODULE, &config_usart);

    port_pin_set_output_level(LED_GREEN_PIN, LED_GREEN_ACTIVE);
#endif


//BT USART
    config_usart.baudrate    = 9600;
    config_usart.mux_setting = BT_UART_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = BT_UART_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = BT_UART_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = BT_UART_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = BT_UART_SERCOM_PINMUX_PAD3;

    while (usart_init(&bt_usart_instance, BT_UART_MODULE, &config_usart) != STATUS_OK) {}
   
    usart_enable(&bt_usart_instance);
    
}

//=============================================================================
//! \brief Configure callback Function for USART (BT).
//=============================================================================
void configure_usart_callbacks(void)
{
    //! [setup_register_callbacks]
    usart_register_callback(&bt_usart_instance, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&bt_usart_instance, usart_read_callback,  USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_register_callbacks]

    //! [setup_enable_callbacks]
    usart_enable_callback(&bt_usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&bt_usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_enable_callbacks]
}


void bt_usart_write_job(uint8_t *string)
{
    usart_write_buffer_job(&bt_usart_instance, string, sizeof(string));
}

void bt_usart_receive_job(void)
{
    usart_read_buffer_job(&bt_usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}
