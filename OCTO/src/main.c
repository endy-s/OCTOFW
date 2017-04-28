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

//====================================================
// Local Defines
//====================================================


//=============================================================================
//! \brief Main Function.
//=============================================================================
int main (void)
{
	system_init();

	/* Insert application code here, after the board has been initialized. */
    
    //! [setup_init]
    configure_usart();
    configure_usart_callbacks();
    //! [setup_init]

    //! [main]
    //! [enable_global_interrupts]
    system_interrupt_enable_global();
    //! [enable_global_interrupts]

    //! [main_send_string]
    //uint8_t string[] = "Hello World!\r\n";
    //usart_write_buffer_wait(&usart_instance, string, sizeof(string));
    //! [main_send_string]
    
    /* Configure device and enable. */
    configure_gas_gauge();
    
    //! [main_loop]
    while (true) {
        //! [main_loop]
        //! [main_read]
       // usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
        //! [main_read]
    }
}
