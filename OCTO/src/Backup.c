/*
 * Backup.c
 *
 * Created: 04/07/2017 23:52:35
 *  Author: Endy
 */ 
/*
//TODO at main.c
//=============================================================================
//! \brief Main Function.
//=============================================================================
int main (void)
{
    system_init();
    
    // Configure all the peripherals for the OCTO Board
    configure_OCTO_peripheral();    
    //bcap_timer = get_tick();                              //TODO verify if works without this and remove
    bcap_touch_counter = 0;
    bcap_notouch_counter = 0;
    bcap_low = 0;
    bcap_high = 0x000FFFFF;
    bcap_limit_temp = 0;
    bcap_calibrate_counter = 0;
    sos_mode = false;
    activated = false;
    
    while (true)
    {
        drive_light();

        bt_usart_receive_job();
        
        if (tick_elapsed(bt_timer) % 1000 == 0)
        {
            bt_timer = get_tick();
            
            if (bt_connected)
            {
                uint8_t buf[12];
                sprintf(buf, "<B=%3u;T=%2u>", get_battery_percent(), get_temperature_celsius());
                
                bt_poll_check();
                
                bt_usart_write_job(buf, 12);
            }
        }
        
        //if (tick_elapsed(bcap_timer) % 500 == 0)
        //{
            //bcap_timer = get_tick();
            //
            //detect_bcap_touch();
            //
            //if (bcap_calibrate_counter < 5)
            //{
                //bcap_calibrate_counter++;
                //bcap_limit_temp += bcap_counter;
            //}
            //else if (bcap_calibrate_counter == 5)
            //{
                //bcap_calibrate_counter++;
                //bcap_low = (bcap_limit_temp/5);
                //bcap_low += (bcap_low*(1/5));
                //bcap_high = bcap_low + 10;
                //change_light_state(E_LIGHT_OFF);
            //}
            //else
            //{
                //if (bcap_counter > bcap_low)    //each 500ms
                //{
                    //bcap_notouch_counter = 0;
                    //bcap_touch_counter++;
                    //
                    ////if (sos_mode)
                    ////{
                    ////if (bcap_touch_counter >= 8)
                    ////{
                    //////sos_mode = false;
                    //////TODO: Create SOS Light mode
                    ////change_light_state(E_LIGHT_FADE);
                    ////
                    ////port_pin_toggle_output_level(LED_RED_PIN);
                    ////}
                    ////}
                    ////else
                    ////{
                    //if (bcap_touch_counter == 4)
                    //{
                        //port_pin_toggle_output_level(LED_GREEN_PIN);
                        //
                        //if (activated)
                        //{
                            //change_light_state(E_LIGHT_OFF, true);
                        //}
                        //else
                        //{
                            //change_light_state(E_LIGHT_ON, true);
                        //}
                    //}
                    ////else if (bcap_touch_counter == 5)
                    ////{
                    ////port_pin_toggle_output_level(LED_GREEN_PIN);
                    ////
                    ////if (activated)
                    ////{
                    ////change_light_state(E_LIGHT_OFF);
                    ////}
                    ////else
                    ////{
                    ////change_light_state(E_LIGHT_ON);
                    ////}
                    ////}
                    ////else if (bcap_touch_counter >= 8)
                    ////{
                    ////bcap_touch_counter = 0;
                    //////TODO: Create SOS Light mode
                    ////change_light_state(E_LIGHT_STROBE);
                    ////
                    ////port_pin_toggle_output_level(LED_RED_PIN);
                    ////}
                    ////}
                //}
                //else if(bcap_touch_counter > 0)
                //{
                    //bcap_notouch_counter++;
                    //
                    //if (bcap_notouch_counter > 1)
                    //{
                        ////if (!sos_mode)
                        ////{
                        //if (bcap_touch_counter >= 4)
                        //{
                            ////TODO: Add code to turn on/off (leave/enter everything in sleep) - activated?
                            //port_pin_toggle_output_level(LED_GREEN_PIN);
                            //
                            //if (activated)
                            //{
                                //activated = false;
                                ////change_light_state(E_LIGHT_OFF);
                            //}
                            //else
                            //{
                                //activated = true;
                                ////change_light_state(E_LIGHT_ON);
                            //}
                        //}
                        ////if (bcap_notouch_counter >= 8)
                        ////{
                        ////sos_mode = true;
                        ////}
                        ////}
                        //
                        //bcap_touch_counter = 0;
                    //}
                //}
            //}
            //
            //uint8_t buf[13];
            //sprintf(buf, "<%u,%u>", bcap_counter, bcap_low);
            //bt_usart_write_job(buf, 8);
        //}
    //}
}
*/