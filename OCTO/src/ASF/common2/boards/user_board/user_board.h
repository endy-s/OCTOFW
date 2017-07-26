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
#define LED_RED_NAME                "LED0 (bicolor)"
#define LED_RED_PIN                 PIN_PA14
#define LED_RED_ACTIVE              true
#define LED_RED_INACTIVE            !LED_RED_ACTIVE
#define LED_RED_GPIO                LED_RED_PIN
#define LED_RED                     LED_RED_PIN
/** @} */


/** \name LED GREEN definitions
 *  @{ */
#define LED_GREEN_NAME              "LED1 (bicolor)"
#define LED_GREEN_PIN               PIN_PA15
#define LED_GREEN_ACTIVE            true
#define LED_GREEN_INACTIVE          !LED_GREEN_ACTIVE
#define LED_GREEN_GPIO              LED_GREEN_PIN
#define LED_GREEN                   LED_GREEN_PIN
/** @} */

/** \name LED Driver definitions
 *  @{ */
#define LED_DRIVER_NAME             "LED Driver"
#define LED_DRIVER_PIN              PIN_PA01
#define LED_DRIVER_ACTIVE           true
#define LED_DRIVER_INACTIVE         !LED_DRIVER_ACTIVE
#define LED_DRIVER_GPIO             LED_DRIVER_PIN
#define LED_DRIVER                  LED_DRIVER_PIN
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 3

/** \name BT Module definitions
 *  @{ */
#define BT_ENABLE_NAME              "BT Module"
#define BT_ENABLE_PIN               PIN_PA03
#define BT_ENABLE_ACTIVE            true
#define BT_ENABLE_INACTIVE          !BT_ENABLE_ACTIVE
#define BT_ENABLE_GPIO              BT_ENABLE_PIN
#define BT_ENABLE                   BT_ENABLE_PIN
/** @} */

/** \name BCAP definitions
 *  @{ */
#define BCAP_ENABLE_NAME            "BCAP Enable"
#define BCAP_ENABLE_PIN             PIN_PA19
#define BCAP_ENABLE_ACTIVE          true
#define BCAP_ENABLE_INACTIVE        !BCAP_ENABLE_ACTIVE
#define BCAP_ENABLE_GPIO            BCAP_ENABLE_PIN
#define BCAP_ENABLE                 BCAP_ENABLE_PIN
/** @} */


/**
 * \name DRIVER_BCAP definitions
 *
 * Wrapper macros for SW0, to ensure common naming across all Xplained Pro
 * boards.
 *
 *  @{ */
#define SW0_PIN                   PIN_PA18
#define SW0_ACTIVE                true
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA18A_EIC_EXTINT2
#define SW0_EIC_MUX               MUX_PA18A_EIC_EXTINT2
#define SW0_EIC_PINMUX            PINMUX_PA18A_EIC_EXTINT2
#define SW0_EIC_LINE              2
/** @} */


#define DRIVER_BCAP_NAME          "SW0"
#define DRIVER_BCAP_PIN           SW0_PIN
#define DRIVER_BCAP_ACTIVE        SW0_ACTIVE
#define DRIVER_BCAP_INACTIVE      SW0_INACTIVE
#define DRIVER_BCAP_EIC_PIN       SW0_EIC_PIN
#define DRIVER_BCAP_EIC_MUX       SW0_EIC_MUX
#define DRIVER_BCAP_EIC_PINMUX    SW0_EIC_PINMUX
#define DRIVER_BCAP_EIC_LINE      SW0_EIC_LINE
/** @} */


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


/** \name Embedded debugger GPIO interface definitions
 * @{
 */
#define EDBG_GPIO0_PIN            PIN_PA27
#define EDBG_GPIO1_PIN            PIN_PA28
#define EDBG_GPIO2_PIN            PIN_PA20
#define EDBG_GPIO3_PIN            PIN_PA21
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


/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()     \
		extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()    \
		extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()      \
		extint_chan_clear_detected(AT86RFX_IRQ_CHAN);

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()   \
		{ extint_chan_disable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT)

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()   \
		extint_chan_enable_callback(AT86RFX_IRQ_CHAN, EXTINT_CALLBACK_TYPE_DETECT); }

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
