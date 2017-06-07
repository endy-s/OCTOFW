/*
 * OCTO_USART.c
 *
 * Created: 27/04/2017 23:29:30
 *  Author: Endy
 */ 

#include "OCTO_USART.h"
#include <string.h>
#include "main.h"
#include "OCTO_DAC.h"
#include "OCTO_ADC.h"

//====================================================
// USART - Local Variables
//====================================================

//! [module_inst]
struct usart_module bt_usart_instance;
struct usart_module dbg_usart_instance;
//! [module_inst]

bool bt_connected = false;
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
            received_bt(bt_message);
            
            port_pin_toggle_output_level(LED_GREEN_PIN);
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
void received_bt(uint8_t* received_msg)
{
    if (!bt_connected)
    {
        int compare_result = strcmp((const char*) received_msg, "OCTO");
        if (compare_result == 0)
        {
            change_light_state(E_LIGHT_OFF);
            uint8_t* init_resp = "<BOARD>";
            usart_write_buffer_job(&bt_usart_instance, init_resp, 7);
            bt_connected = true;
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
                    change_light_bright((uint16_t)(received_msg[index+2] - 0x30)*100);
                }
            }
            
            uint8_t* init_resp = "<OK>";
            usart_write_buffer_job(&bt_usart_instance, init_resp, 4);
        }
        else if (received_msg[0] == 'S')
        {
            
            
        }
    }
}

//=============================================================================
//! \brief Get the info about the board and send it to the BT USART.
//=============================================================================
void send_board_info()
{
    uint32_t adc_reading = 0, reading = 0;
    uint8_t batt_value = 0, temp_value = 0;
    
    configure_adc_VMPPT();
    
    get_value_VMPPT(&adc_reading, &reading);
    
    if (reading > BATT_MAX)
    {
        batt_value = 100;
    }
    else
    {
        batt_value = (reading * 100) / BATT_MAX;
    }
    
    turn_off_adc();
    configure_adc_TEMP();
    
    get_value_TEMP(&adc_reading, &reading);
    
    temp_value = reading;
    
    uint8_t buf[12];
    sprintf(buf, "<b=%d;T=%d>", batt_value, temp_value);
    usart_write_buffer_job(&bt_usart_instance, buf, 12);
    printf("%s", buf);
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