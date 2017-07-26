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

//Light perthousand range's ideal usage of is from 100 to 950
#define LIGHT_MAX 950
#define LIGHT_MIN 100

//Battery goes from 3,3 to 4,2 (real max is 4,25, but the percent is better using 4,2)
//Using Gas Gauge is ok to use 4,25 to check CC
#define BATT_MAX 4250
#define BATT_MIN 3300

#define BCAP_THRESOLD_HIGH 120
#define BCAP_THRESOLD_LOW  110

//=============================================================================
// Local Variables
//=============================================================================
uint32_t led_timer, bcap_timer, bt_timer;
uint32_t bcap_counter, bcap_touch_counter, bcap_notouch_counter;
uint32_t bcap_high, bcap_low, bcap_calibrate_counter, bcap_limit_temp;
bool     poll_requested, sos_mode, activated;
struct   port_config pin_conf;

typedef enum
{
    E_LIGHT_OFF = 0,
    E_LIGHT_ON,
    E_LIGHT_FADE,
    E_LIGHT_STROBE,
} E_LIGHT_MODE;

typedef enum
{
    E_LIGHT_FAST = 1,
    E_LIGHT_MEDIUM,
    E_LIGHT_SLOW,
} E_LIGHT_FREQ;


typedef struct
{
    E_LIGHT_MODE mode;
    E_LIGHT_FREQ freq;
    uint16_t     led_bright;
    uint16_t     led_max_bright;
    bool         led_rising;
} OCTO_LIGHT;

OCTO_LIGHT light_state;


// Prototype
void configure_OCTO_peripheral(void);

bool detect_bcap_touch(void);

void change_light_state(E_LIGHT_MODE new_mode, bool update_app);
void change_light_freq(E_LIGHT_FREQ new_freq);
void change_light_bright(uint16_t perthousand);
void drive_light(void);
bool update_bright(void);
void turn_lights(bool on);
void send_info(void);



#endif /* COMMON_H_ */