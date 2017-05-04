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
struct usart_module bt_usart_instance, dbg_usart_instance;
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
    
    uint8_t answer_string[] = "Received: ";
    usart_write_buffer_wait(&bt_usart_instance, answer_string, sizeof(rx_buffer));
    
    usart_write_buffer_wait(&bt_usart_instance, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}

//=============================================================================
//! \brief Callback Function for USART Write (BT).
//!
//! \param[in] usart_module - The USART module to receive the callback
//=============================================================================
void usart_write_callback(struct usart_module *const usart_module)
{
    port_pin_toggle_output_level(LED_RED_PIN);
}

//=============================================================================
//! \brief Setup Function for USART (Debug and BT).
//=============================================================================
void configure_usart(void)
{
    // General

    //! [setup_config]
    struct usart_config config_usart;
    //! [setup_config]
    //! [setup_config_defaults]
    usart_get_config_defaults(&config_usart);
    //! [setup_config_defaults]
    
// Debug USART
#ifdef DBG_MODE
    //! [setup_change_config]
    config_usart.baudrate    = 115200;
    config_usart.mux_setting = DBG_UART_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = DBG_UART_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = DBG_UART_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = DBG_UART_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = DBG_UART_SERCOM_PINMUX_PAD3;
    //! [setup_change_config]

    //! [setup_set_config]
    while (usart_init(&dbg_usart_instance, DBG_UART_MODULE, &config_usart) != STATUS_OK)
    {
    }
    //! [setup_set_config]

    //! [setup_enable]
    usart_enable(&dbg_usart_instance);
    //! [setup_enable]
#endif


    // BT USART
    //! [setup_change_config]
    config_usart.baudrate    = 115200;
    config_usart.mux_setting = BT_UART_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = BT_UART_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = BT_UART_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = BT_UART_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = BT_UART_SERCOM_PINMUX_PAD3;
    //! [setup_change_config]

    //! [setup_set_config]
    while (usart_init(&bt_usart_instance, BT_UART_MODULE, &config_usart) != STATUS_OK)
    {
    }
    //! [setup_set_config]

    //! [setup_enable]
    usart_enable(&bt_usart_instance);
    //! [setup_enable]
    
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


void dbg_usart_write(uint8_t *string)
{
// Debug USART
#ifdef DBG_MODE
    usart_write_buffer_wait(&dbg_usart_instance, string, sizeof(string));
#endif
}
