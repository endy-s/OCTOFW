/*
 * OCTO_USART.c
 *
 * Created: 27/04/2017 23:29:30
 *  Author: Endy
 */ 

#include "OCTO_USART.h"
#include <string.h>
#include "main.h"

//====================================================
// USART - Local Variables
//====================================================

//! [module_inst]

struct usart_module dbg_usart_instance;
//! [module_inst]

bool bt_start_received = false;

//! [USART rx_buffer_var]
#define MIN_RX_BUFFER_LENGTH   1
#define MAX_RX_BUFFER_LENGTH   20

uint8_t rx_buffer[MIN_RX_BUFFER_LENGTH];
uint8_t bt_message[MAX_RX_BUFFER_LENGTH];
uint8_t bt_counting = 0;
//! [USART rx_buffer_var]


//=============================================================================
//! \brief Callback Function for USART Read (BT).
//!
//! \param[in] usart_module - The USART module to receive the callback
//=============================================================================
void usart_read_callback(struct usart_module *const usart_module)
{    
    if ((!bt_start_received) && (rx_buffer[0] == '<'))
    {
        bt_start_received = true;
    }
    else
    {
        if (rx_buffer[0] == '>')
        {
            //port_pin_toggle_output_level(LED_GREEN_PIN);
            bt_received(bt_message);
            clean_array(bt_counting);
            bt_counting = 0;
            bt_start_received = false;
        }
        else
        {
            bt_message[bt_counting] = rx_buffer[0];
            bt_counting++;
        }
    }
}

//=============================================================================
//! \brief Callback Function for USART Write (BT).
//!
//! \param[in] usart_module - The USART module to receive the callback
//=============================================================================
void usart_write_callback(struct usart_module *const usart_module)
{
    clean_array(bt_counting);
    bt_counting = 0;
    bt_start_received = false;
}

//=============================================================================
//! \brief Clean the "received array" of the BT USART.
//=============================================================================
void clean_array(int length)
{
    for (int i = 0; i < length; i++)
    {
        bt_message[i] = 0x00;
    }
}

//=============================================================================
//! \brief Setup Function for USART (Debug and BT).
//=============================================================================
void configure_usart(void)
{
// General
    bt_timer = 0;
    bt_connected = false;
    poll_requested = false;
    
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

//=============================================================================
//! \brief Write at the BT USART.
//=============================================================================
void bt_usart_write_job(uint8_t *string, uint16_t length)
{
    usart_write_buffer_job(&bt_usart_instance, string, length);
}

//=============================================================================
//! \brief Check for received messages at the BT USART.
//=============================================================================
void bt_usart_receive_job(void)
{
    usart_read_buffer_job(&bt_usart_instance, (uint8_t *)rx_buffer, MIN_RX_BUFFER_LENGTH);
}

//=============================================================================
//! \brief Treat the received messages at the BT USART.
//=============================================================================
void bt_received(uint8_t* received_msg)
{
    if (!bt_connected)
    {
        if (strcmp((const char*) received_msg, "OCTO") == 0)
        {
            bt_start_setup();
        }
    }
    else
    {
        if (received_msg[0] == 'W')
        {
            int nr_params = received_msg[2] - 0x30;
            int index = 4;
            for (int i = 0; i < nr_params; i++, index+=4)
            {
                if (received_msg[index] == 'L')
                {
                    int new_mode = (E_LIGHT_MODE) received_msg[index+2] - 0x30;
                    change_light_state(new_mode);
                }
                else if (received_msg[index] == 'F')
                {
                    change_light_freq((E_LIGHT_FREQ) received_msg[index+2] - 0x30);
                }
                else if (received_msg[index] == 'I')
                {
                    uint16_t light_perhundred = ((received_msg[index+2] - 0x30) * 10) + (received_msg[index+3] - 0x30);
                    uint16_t light_perthousand = ((light_perhundred * 850) / 100) + 100;
                    change_light_bright(light_perthousand);
                }
            }
            
            uint8_t* init_resp = "<OK>";
            usart_write_buffer_job(&bt_usart_instance, init_resp, 4);
        }
        else if (received_msg[0] == 'D')
        {
            change_light_state(E_LIGHT_ON);
            bt_connected = false;
        }
        else if (received_msg[0] == 'C')
        {
            if (received_msg[2] == '1')
            {                            
                bcap_enable = true;
            }
            else
            {
                bcap_enable = false;
            }
            uint8_t* init_resp = "<OK>";
            usart_write_buffer_job(&bt_usart_instance, init_resp, 4);
        }
        else if(strcmp((const char*) received_msg, "OK") == 0)
        {
            poll_requested = false;
        }
    }
}

//=============================================================================
//! \brief Send the handshake to BT USART.
//=============================================================================
void bt_start_setup()
{
    change_light_state(E_LIGHT_OFF);
    uint8_t* init_resp = "<BOARD>";
    usart_write_buffer_job(&bt_usart_instance, init_resp, 7);
    bt_timer = 0;
    bt_connected = true;
    poll_requested = false;
}

//=============================================================================
//! \brief Send the handshake to BT USART.
//=============================================================================
void bt_send_light_update()
{
    uint8_t light_update[8];
    sprintf(light_update, "<U;L=%u;>", light_state.mode);
    usart_write_buffer_job(&bt_usart_instance, light_update, 8);
}