/*
 * OCTO_I2C.c
 *
 * Created: 27/04/2017 23:29:55
 *  Author: Endy
 */ 

#include "OCTO_I2C.h"


/** Address of Gas Gauge chip */
#define GAS_GAUGE_ADDRESS           0x64

#define TIMEOUT 1000
/** Full Scale of the Counting */
#define FULL_SCALE_GAUGE            0xFFFF

#define DATA_LENGTH 2


//=============================================================================
// I²C - Local variables
//=============================================================================

struct i2c_master_module gas_gauge_instance;
static char twi_inited = false;

const uint8_t test_pattern[] = {0xF0, 0x01};
static uint8_t read_buffer[DATA_LENGTH] = {0x02, 0x02};


void configure_gas_gauge()
{
    struct i2c_master_config config_gas_gauge;
    
    i2c_master_get_config_defaults(&config_gas_gauge);
    
    config_gas_gauge.baud_rate   = I2C_MASTER_BAUD_RATE_100KHZ;
    config_gas_gauge.pinmux_pad0 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD0;
    config_gas_gauge.pinmux_pad1 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD1;
    
    i2c_master_init(&gas_gauge_instance, GAS_GAUGE_I2C_MODULE, &config_gas_gauge);
    
    i2c_master_enable(&gas_gauge_instance);
}

bool gas_gauge_read(uint32_t *value, uint32_t *percent)
{
    bool ok = false;
    
    /* Timeout counter. */
    uint16_t timeout = 0;
    
    
    /* Init i2c packet. */
    struct i2c_master_packet packet = {
        .address = GAS_GAUGE_ADDRESS,
        .data_length = DATA_LENGTH,
        .data = read_buffer,
        .ten_bit_address = false,
        .high_speed = false,
        .hs_master_code = 0x0,
    };
    
    /* Read from slave until success. */
    //packet.data = read_buffer;
    if (i2c_master_read_packet_wait(&gas_gauge_instance, &packet) == STATUS_OK) {
        /* Increment timeout counter and check if timed out. */
        //if (timeout++ == TIMEOUT) {
            //break;
        //}
        
        //printf("TWI SETUP READ - SUCCESS!\n");

        uint16_t twi_reading = read_buffer[0] << 8 | read_buffer[1];
        uint16_t twi_percent = (twi_reading * 1000) / FULL_SCALE_GAUGE;

        *value     = twi_reading;
        *percent   = twi_percent;

        ok = true;
        
        //uint8_t string[] = "\nI2C read: ";
        //usart_write_buffer_wait(&gas_gauge_instance, string, sizeof(string));
        //usart_write_buffer_wait(&gas_gauge_instance, read_buffer, sizeof(string));
    }
    else
    {
        //printf("TWI SETUP READ - FAILED!\n");
    }
    
    return ok;
}