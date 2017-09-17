/*
 * OCTO_I2C.c
 *
 * Created: 27/04/2017 23:29:55
 *  Author: Endy
 */ 

#include "OCTO_I2C.h"


/** Address of Gas Gauge chip - LTC LTC2942 */
#define GAS_GAUGE_ADDRESS           0x64

#define TIMEOUT 100000
/** Full Scale of the Counting */
#define FULL_SCALE_GAUGE            0xFFFF

#define DATA_LENGTH 4
#define WRITE_LENGTH 2


//=============================================================================
// I²C - Local variables
//=============================================================================

struct i2c_master_module gas_gauge_instance;
static char twi_inited = false;

//To write, you need to pass the address as the first byte of the buffer, and the rest of the buffer is the data
//To configure the registers
static uint8_t configure_registers_accumulated_buffer[WRITE_LENGTH];
static uint8_t configure_registers_cc_buffer[WRITE_LENGTH] = {0x01, 0x2E};
static uint8_t configure_registers_al_buffer[WRITE_LENGTH] = {0x01, 0x3C};
    
//Didn't find how to read from an specif address (there seems to be no write and read without a stop byte between)
//Workaround by now: Read all the bytes until you reach the one you want :) sorry
static uint8_t read_buffer[DATA_LENGTH];


void configure_gas_gauge(uint32_t battery_percent)
{

    struct i2c_master_config config_gas_gauge;
    
    i2c_master_get_config_defaults(&config_gas_gauge);
    
    config_gas_gauge.baud_rate   = I2C_MASTER_BAUD_RATE_100KHZ;
    config_gas_gauge.pinmux_pad0 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD0;
    config_gas_gauge.pinmux_pad1 = GAS_GAUGE_I2C_SERCOM_PINMUX_PAD1;
    
    i2c_master_init(&gas_gauge_instance, GAS_GAUGE_I2C_MODULE, &config_gas_gauge);
    
    i2c_master_enable(&gas_gauge_instance);
    
	gas_gauge_update_percent(battery_percent);
    gas_gauge_config_CC_registers();
}

void gas_gauge_update_percent(uint32_t new_battery_percent)
{
	uint16_t new_battery_value = ((new_battery_percent * FULL_SCALE_GAUGE) / 100);
	
	configure_registers_accumulated_buffer[0] = 0x02;
	configure_registers_accumulated_buffer[1] = (new_battery_value >> 8) & 0xFF;
	
	struct i2c_master_packet packet = {
		.address = GAS_GAUGE_ADDRESS,
		.data_length = WRITE_LENGTH,
		.data = configure_registers_accumulated_buffer,
		.ten_bit_address = false,
		.high_speed = false,
		.hs_master_code = 0x0,
	};
	
	while (i2c_master_write_packet_wait(&gas_gauge_instance, &packet) != STATUS_OK);
	
	configure_registers_accumulated_buffer[0] = 0x03;
	configure_registers_accumulated_buffer[1] = new_battery_value & 0xFF;
	
	while (i2c_master_write_packet_wait(&gas_gauge_instance, &packet) != STATUS_OK);
}

void gas_gauge_config_CC_registers()
{
    /* Init i2c packet. */
    struct i2c_master_packet packet = {
        .address = GAS_GAUGE_ADDRESS,
        .data_length = WRITE_LENGTH,
        .data = configure_registers_cc_buffer,
        .ten_bit_address = false,
        .high_speed = false,
        .hs_master_code = 0x0,
    };
    
    while (i2c_master_write_packet_wait(&gas_gauge_instance, &packet) != STATUS_OK);
}

void gas_gauge_config_AL_registers()
{
    /* Init i2c packet. */
    struct i2c_master_packet packet = {
        .address = GAS_GAUGE_ADDRESS,
        .data_length = WRITE_LENGTH,
        .data = configure_registers_al_buffer,
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
        uint16_t twi_percent = ((twi_reading * 100) / FULL_SCALE_GAUGE);

        *value     = twi_reading;
        *percent   = twi_percent;

        ok = true;
    }
    
    return ok;
}