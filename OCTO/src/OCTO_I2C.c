/*
 * OCTO_I2C.c
 *
 * Created: 27/04/2017 23:29:55
 *  Author: Endy
 */ 

#include "OCTO_I2C.h"


/** Address of Gas Gauge chip */
#define GAS_GAUGE_ADDRESS           0x64

#define TIMEOUT 100000
/** Full Scale of the Counting */
#define FULL_SCALE_GAUGE            0xFFFF

#define DATA_LENGTH 4


//=============================================================================
// I²C - Local variables
//=============================================================================

struct i2c_master_module gas_gauge_instance;
static char twi_inited = false;

//To write, you need to pass the address as the first byte of the buffer, and the rest of the buffer is the data
//To configure the registers it's sending 2 more just to reuse the define and to have sure everything's ok
static uint8_t configure_registers_buffer[DATA_LENGTH] = {0x01, 0x2E, 0x2E, 0x2E};
    
//Didn't find how to read from an specif address (there seems to be no write and read without a stop byte between)
//Workaround by now: Read all the bytes until you reach the one you want :) sorry
static uint8_t read_buffer[DATA_LENGTH];


void configure_gas_gauge()
{
    struct i2c_master_config config_gas_gauge;
    
    i2c_master_get_config_defaults(&config_gas_gauge);
    
    config_gas_gauge.baud_rate   = I2C_MASTER_BAUD_RATE_100KHZ;
    config_gas_gauge.pinmux_pad0 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD0;
    config_gas_gauge.pinmux_pad1 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD1;
    
    i2c_master_init(&gas_gauge_instance, GAS_GAUGE_I2C_MODULE, &config_gas_gauge);
    
    i2c_master_enable(&gas_gauge_instance);
    
    gas_gauge_config_registers();
}

void gas_gauge_config_registers()
{
    /* Init i2c packet. */
    struct i2c_master_packet packet = {
        .address = GAS_GAUGE_ADDRESS,
        .data_length = 2,
        .data = configure_registers_buffer,
        .ten_bit_address = false,
        .high_speed = false,
        .hs_master_code = 0x0,
    };
    
    while (i2c_master_write_packet_wait(&gas_gauge_instance, &packet) != STATUS_OK);
}

bool gas_gauge_read(uint32_t *value, uint32_t *percent)
{
    bool ok = false;
    
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
    packet.data = read_buffer;
    if (i2c_master_read_packet_wait(&gas_gauge_instance, &packet) == STATUS_OK) {
        
        uint16_t twi_reading = read_buffer[2] << 8 | read_buffer[3];
        uint16_t twi_percent = (twi_reading * 100) / FULL_SCALE_GAUGE;

        *value     = twi_reading;
        *percent   = twi_percent;

        ok = true;
    }
    
    return ok;
}