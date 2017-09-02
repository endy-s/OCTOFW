/*
 * Common.h
 *
 * Created: 25/05/2017 20:32:38
 *  Author: Endy
 */ 


#ifndef COMMON_H_
#define COMMON_H_


//=============================================================================
// Defines
//=============================================================================
//Debug flag
//#define DBG_MODE

//Light perthousand range's ideal usage of is from 100 to 950
#define LIGHT_MAX 950
#define LIGHT_MIN 100

//Battery goes from 3,3 to 4,2 (real max is 4,25, but the percent is better using 4,2)
//Using Gas Gauge is ok to use 4,25 to check CC
#define BATT_MAX 4340
#define BATT_MIN 3300

#define LOW_POWER_LIGHT_ON_TIME 30000	//30000
#define LOW_POWER_LIGHT_STROBE_TIME 1500	//1500

#define BCAP_THRESOLD_COUNTER 20

//=============================================================================
// Local Variables
//=============================================================================
uint32_t led_timer, bcap_timer, bt_timer, low_power_timer;
uint32_t bcap_counter, bcap_touch_counter, bcap_notouch_counter;
uint32_t bcap_high, bcap_low, bcap_calibrate_counter, bcap_limit_temp;
uint32_t battery_level;
bool     low_power_update_app;
bool     batt_reached_max, batt_reached_low_power;
bool     poll_requested;
bool     bcap_enable;
bool     bcap_update_app;
bool     sos_mode, activated;
struct   port_config pin_conf;

typedef enum
{
    E_LIGHT_OFF = 0,
    E_LIGHT_ON,
    E_LIGHT_FADE,
    E_LIGHT_STROBE,
} E_LIGHT_MODE;

E_LIGHT_MODE light_bcap_previous_mode;

typedef enum
{
    E_LIGHT_SLOW = 1,
    E_LIGHT_MEDIUM = 3,
    E_LIGHT_FAST = 5,
} E_LIGHT_FREQ;


typedef struct
{
    E_LIGHT_MODE mode;
    E_LIGHT_FREQ freq;
    uint16_t     low_power_threshold;
    uint16_t     led_bright;
    uint16_t     led_max_bright;
	uint16_t	 led_low_power_time;
    bool         led_rising;
} OCTO_LIGHT;

OCTO_LIGHT light_state;

// Prototype
void configure_OCTO_peripheral(void);

bool detect_bcap_touch(void);

void change_light_mode(E_LIGHT_MODE new_mode);
void change_light_freq(E_LIGHT_FREQ new_freq);
void change_light_bright(uint16_t perthousand);
void change_light_threshold(uint16_t new_threshold);
void enter_low_power_mode();
void exit_low_power_mode();
void manage_low_power_light();
void drive_light(void);
bool update_bright(void);
void turn_lights(bool on);
void send_info(void);

void bt_poll_check(void);
uint32_t get_battery_percent(void);
uint32_t get_temperature_celsius(void);
uint32_t get_gauge_percent(void);

#endif /* COMMON_H_ */