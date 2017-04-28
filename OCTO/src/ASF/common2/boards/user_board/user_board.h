/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/**
 * \defgroup samd21_xplained_pro_features_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "USER_BOARD"

/** \name Resonator definitions
 *  @{ */
#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625
/** @} */

/** \name LED RED definitions
 *  @{ */
#define LED_RED_NAME                "LED0 (yellow)"
#define LED_RED_PIN                 PIN_PA14
#define LED_RED_ACTIVE              false
#define LED_RED_INACTIVE            !LED_RED_ACTIVE
#define LED_RED_GPIO                LED_RED_PIN
#define LED_RED                     LED_RED_PIN

//#define LED_RED_PWM4CTRL_MODULE     TCC0
//#define LED_RED_PWM4CTRL_CHANNEL    0
//#define LED_RED_PWM4CTRL_OUTPUT     0
//#define LED_RED_PWM4CTRL_PIN        PIN_PA14E_TCC0_WO0
//#define LED_RED_PWM4CTRL_MUX        MUX_PA14E_TCC0_WO0
//#define LED_RED_PWM4CTRL_PINMUX     PINMUX_PB30E_TCC0_WO0
/** @} */


/** \name LED GREEN definitions
 *  @{ */
#define LED_GREEN_NAME            "LED0 (yellow)"
#define LED_GREEN_PIN             PIN_PA15
#define LED_GREEN_ACTIVE          false
#define LED_GREEN_INACTIVE        !LED_GREEN_ACTIVE
#define LED_GREEN_GPIO            LED_GREEN_PIN
#define LED_GREEN                 LED_GREEN_PIN

//#define LED_GREEN_PWM4CTRL_MODULE     TCC0
//#define LED_GREEN_PWM4CTRL_CHANNEL    0
//#define LED_GREEN_PWM4CTRL_OUTPUT     0
//#define LED_GREEN_PWM4CTRL_PIN        PIN_PA14E_TCC0_WO0
//#define LED_GREEN_PWM4CTRL_MUX        MUX_PA14E_TCC0_WO0
//#define LED_GREEN_PWM4CTRL_PINMUX     PINMUX_PB30E_TCC0_WO0
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 2

/** \name Extension header #1 ADC definitions
 *  @{
 */
#define ADC_MODULE                 ADC
#define VMPPT_ADC_0_CHANNEL        4
#define VMPPT_ADC_0_PIN            PIN_PA04B_ADC_AIN4
#define VMPPT_ADC_0_MUX            MUX_PA04B_ADC_AIN4
#define VMPPT_ADC_0_PINMUX         PINMUX_PA04B_ADC_AIN4
#define TEMP_ADC_1_CHANNEL         7
#define TEMP_ADC_1_PIN             PIN_PA07B_ADC_AIN7
#define TEMP_ADC_1_MUX             MUX_PA07B_ADC_AIN7
#define TEMP_ADC_1_PINMUX          PINMUX_PA07B_ADC_AIN7
/** @} */

/** \name Extension header #1 I2C definitions
 *  @{
 */
#define GAS_GAUGE_I2C_MODULE              SERCOM0
#define GAS_GAUGE_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA08C_SERCOM0_PAD0
#define GAS_GAUGE_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA09C_SERCOM0_PAD1
#define GAS_GAUGE_I2C_SERCOM_DMAC_ID_TX   SERCOM0_DMAC_ID_TX
#define GAS_GAUGE_I2C_SERCOM_DMAC_ID_RX   SERCOM0_DMAC_ID_RX
/** @} */

/** \name Extension header #1 UART definitions
 *  @{
 */
#define BT_UART_MODULE              SERCOM1
#define BT_UART_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define BT_UART_SERCOM_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0
#define BT_UART_SERCOM_PINMUX_PAD1  PINMUX_PA17C_SERCOM1_PAD1
#define BT_UART_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define BT_UART_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define BT_UART_SERCOM_DMAC_ID_TX   SERCOM1_DMAC_ID_TX
#define BT_UART_SERCOM_DMAC_ID_RX   SERCOM1_DMAC_ID_RX
/** @} */

/** \name Extension header #2 UART definitions
 *  @{
 */
#define DBG_UART_MODULE              SERCOM3
#define DBG_UART_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define DBG_UART_SERCOM_PINMUX_PAD0  PINMUX_PA22C_SERCOM3_PAD0
#define DBG_UART_SERCOM_PINMUX_PAD1  PINMUX_PA23C_SERCOM3_PAD1
#define DBG_UART_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define DBG_UART_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define DBG_UART_SERCOM_DMAC_ID_TX   SERCOM3_DMAC_ID_TX
#define DBG_UART_SERCOM_DMAC_ID_RX   SERCOM3_DMAC_ID_RX
/** @} */




/** \name Extension header #3 ADC definitions
 *  @{
 */
#define EXT3_ADC_MODULE           ADC
#define EXT3_ADC_0_CHANNEL        0
#define EXT3_ADC_0_PIN            PIN_PA02B_ADC_AIN0
#define EXT3_ADC_0_MUX            MUX_PA02B_ADC_AIN0
#define EXT3_ADC_0_PINMUX         PINMUX_PA02B_ADC_AIN0
#define EXT3_ADC_1_CHANNEL        1
#define EXT3_ADC_1_PIN            PIN_PA03B_ADC_AIN1
#define EXT3_ADC_1_MUX            MUX_PA03B_ADC_AIN1
#define EXT3_ADC_1_PINMUX         PINMUX_PA03B_ADC_AIN1
/** @} */

/** \name Extension header #3 PWM for Control definitions
 *  @{
 */
#define EXT3_PWM4CTRL_MODULE      TCC2
#define EXT3_PWM4CTRL_0_CHANNEL   0
#define EXT3_PWM4CTRL_0_OUTPUT    0
#define EXT3_PWM4CTRL_0_PIN       PIN_PA12E_TCC2_WO0
#define EXT3_PWM4CTRL_0_MUX       MUX_PA12E_TCC2_WO0
#define EXT3_PWM4CTRL_0_PINMUX    PINMUX_PA12E_TCC2_WO0
#define EXT3_PWM4CTRL_1_CHANNEL   1
#define EXT3_PWM4CTRL_1_OUTPUT    1
#define EXT3_PWM4CTRL_1_PIN       PIN_PA13E_TCC2_WO1
#define EXT3_PWM4CTRL_1_MUX       MUX_PA13E_TCC2_WO1
#define EXT3_PWM4CTRL_1_PINMUX    PINMUX_PA13E_TCC2_WO1
/** @} */

/** \name Extension header #3 IRQ/External interrupt definitions
 *  @{
 */
#define EXT3_IRQ_MODULE           EIC
#define EXT3_IRQ_INPUT            8
#define EXT3_IRQ_PIN              PIN_PA28A_EIC_EXTINT8
#define EXT3_IRQ_MUX              MUX_PA28A_EIC_EXTINT8
#define EXT3_IRQ_PINMUX           PINMUX_PA28A_EIC_EXTINT8
/** @} */

/** \name Extension header #3 I2C definitions
 *  @{
 */
#define EXT3_I2C_MODULE              SERCOM2
#define EXT3_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA08D_SERCOM2_PAD0
#define EXT3_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA09D_SERCOM2_PAD1
#define EXT3_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EXT3_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Extension header #3 UART definitions
 *  @{
 */
#define EXT3_UART_MODULE              SERCOM4
#define EXT3_UART_SERCOM_MUX_SETTING  USART_RX_3_TX_2_XCK_3
#define EXT3_UART_SERCOM_PINMUX_PAD0  PINMUX_UNUSED
#define EXT3_UART_SERCOM_PINMUX_PAD1  PINMUX_UNUSED
#define EXT3_UART_SERCOM_PINMUX_PAD2  PINMUX_PB10D_SERCOM4_PAD2
#define EXT3_UART_SERCOM_PINMUX_PAD3  PINMUX_PB11D_SERCOM4_PAD3
#define EXT3_UART_SERCOM_DMAC_ID_TX   SERCOM4_DMAC_ID_TX
#define EXT3_UART_SERCOM_DMAC_ID_RX   SERCOM4_DMAC_ID_RX
/** @} */

/** \name Extension header #3 SPI definitions
 *  @{
 */
#define EXT3_SPI_MODULE              SERCOM5
#define EXT3_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EXT3_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB16C_SERCOM5_PAD0
#define EXT3_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB17C_SERCOM5_PAD1
#define EXT3_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB22D_SERCOM5_PAD2
#define EXT3_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB23D_SERCOM5_PAD3
#define EXT3_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EXT3_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX
/** @} */


/** \name USB definitions
 * @{
 */
#define USB_ID
#define USB_TARGET_DP_PIN            PIN_PA25G_USB_DP
#define USB_TARGET_DP_MUX            MUX_PA25G_USB_DP
#define USB_TARGET_DP_PINMUX         PINMUX_PA25G_USB_DP
#define USB_TARGET_DM_PIN            PIN_PA24G_USB_DM
#define USB_TARGET_DM_MUX            MUX_PA24G_USB_DM
#define USB_TARGET_DM_PINMUX         PINMUX_PA24G_USB_DM
#define USB_VBUS_PIN                 PIN_PA14
#define USB_VBUS_EIC_LINE            14
#define USB_VBUS_EIC_MUX             MUX_PA14A_EIC_EXTINT14
#define USB_VBUS_EIC_PINMUX          PINMUX_PA14A_EIC_EXTINT14
#define USB_ID_PIN                   PIN_PA03
#define USB_ID_EIC_LINE              3
#define USB_ID_EIC_MUX               MUX_PA03A_EIC_EXTINT3
#define USB_ID_EIC_PINMUX            PINMUX_PA03A_EIC_EXTINT3
/** @} */

/** \name Embedded debugger GPIO interface definitions
 * @{
 */
#define EDBG_GPIO0_PIN            PIN_PA27
#define EDBG_GPIO1_PIN            PIN_PA28
#define EDBG_GPIO2_PIN            PIN_PA20
#define EDBG_GPIO3_PIN            PIN_PA21
/** @} */

/** \name Embedded debugger USART interface definitions
 * @{
 */
#define EDBG_UART_MODULE          -1 /* Not available on this board */
#define EDBG_UART_RX_PIN          -1 /* Not available on this board */
#define EDBG_UART_RX_MUX          -1 /* Not available on this board */
#define EDBG_UART_RX_PINMUX       -1 /* Not available on this board */
#define EDBG_UART_RX_SERCOM_PAD   -1 /* Not available on this board */
#define EDBG_UART_TX_PIN          -1 /* Not available on this board */
#define EDBG_UART_TX_MUX          -1 /* Not available on this board */
#define EDBG_UART_TX_PINMUX       -1 /* Not available on this board */
#define EDBG_UART_TX_SERCOM_PAD   -1 /* Not available on this board */
/** @} */

/** \name Embedded debugger I2C interface definitions
 * @{
 */
#define EDBG_I2C_MODULE              SERCOM2
#define EDBG_I2C_SERCOM_PINMUX_PAD0  PINMUX_PA08D_SERCOM2_PAD0
#define EDBG_I2C_SERCOM_PINMUX_PAD1  PINMUX_PA09D_SERCOM2_PAD1
#define EDBG_I2C_SERCOM_DMAC_ID_TX   SERCOM2_DMAC_ID_TX
#define EDBG_I2C_SERCOM_DMAC_ID_RX   SERCOM2_DMAC_ID_RX
/** @} */

/** \name Embedded debugger SPI interface definitions
 * @{
 */
#define EDBG_SPI_MODULE              SERCOM5
#define EDBG_SPI_SERCOM_MUX_SETTING  SPI_SIGNAL_MUX_SETTING_E
#define EDBG_SPI_SERCOM_PINMUX_PAD0  PINMUX_PB16C_SERCOM5_PAD0
#define EDBG_SPI_SERCOM_PINMUX_PAD1  PINMUX_PB31D_SERCOM5_PAD1
#define EDBG_SPI_SERCOM_PINMUX_PAD2  PINMUX_PB22D_SERCOM5_PAD2
#define EDBG_SPI_SERCOM_PINMUX_PAD3  PINMUX_PB23D_SERCOM5_PAD3
#define EDBG_SPI_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define EDBG_SPI_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX
/** @} */

/** \name Embedded debugger CDC Gateway USART interface definitions
 * @{
 */
#define EDBG_CDC_MODULE              SERCOM3
#define EDBG_CDC_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define EDBG_CDC_SERCOM_PINMUX_PAD0  PINMUX_PA22C_SERCOM3_PAD0
#define EDBG_CDC_SERCOM_PINMUX_PAD1  PINMUX_PA23C_SERCOM3_PAD1
#define EDBG_CDC_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_DMAC_ID_TX   SERCOM3_DMAC_ID_TX
#define EDBG_CDC_SERCOM_DMAC_ID_RX   SERCOM3_DMAC_ID_RX
/** @} */


/**
 * \brief Turns off the specified LEDs.
 *
 * \param led_gpio LED to turn off (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio,true)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led_gpio LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio,false)

/**
 * \brief Toggles the specified LEDs.
 *
 * \param led_gpio LED to toggle (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)

/** @} */

#ifdef __cplusplus
}
#endif

#endif // USER_BOARD_H
