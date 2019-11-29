/**
 * \file
 *
 * \brief FreeRTOS demo task implementations.
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <asf.h>
#include "demotasks.h"
#include "proto_alexa_gadget.h"
#include "cJSON.h"

#include <stdio.h>              /* Required for printf */
#include <string.h>             /* Required for memset */
#include <stdarg.h>             /* Required for printf */
/**
 * \addtogroup freertos_sam0_demo_tasks_group
 *
 * @{
 */

//! \name Task configuration
//@{
#define ALEXA_GADGET_TASK_PRIORITY      (tskIDLE_PRIORITY + 3)

#define UART_TASK_PRIORITY      (tskIDLE_PRIORITY + 3)
#define UART_TASK_DELAY         (10 / portTICK_RATE_MS)

#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 2)
#define MAIN_TASK_DELAY         (100 / portTICK_RATE_MS)

#define GRAPH_TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define GRAPH_TASK_DELAY        (50 / portTICK_RATE_MS)

#define TERMINAL_TASK_PRIORITY  (tskIDLE_PRIORITY + 1)
#define TERMINAL_TASK_DELAY     (1000 / portTICK_RATE_MS)

#define ABOUT_TASK_PRIORITY     (tskIDLE_PRIORITY + 1)
#define ABOUT_TASK_DELAY        (33 / portTICK_RATE_MS)

//@}


//! \name Menu and display configuration
//@{
//@}


//! \name Global constants and variables
//@{


/**
 * \brief Instance for \ref oled1_xpro_io_group
 *
 * The extension header to use is configured with \ref OLED1_EXT_HEADER.
 */

//! Instance for \ref edbg_cdc_rx_group
static struct usart_module cdc_usart;
struct usart_module ext1_usart_instance;
struct tc_module tc_instance;

#define MAX_CDC_USART_LENGTH 2
#define MAX_EXT1_USART_LENGTH 2
uint16_t cdc_usart_rx_buffer[MAX_EXT1_USART_LENGTH];
uint16_t ext1_usart_rx_buffer[MAX_CDC_USART_LENGTH];// uart can send 9bit so use uint16_t

uint8_t alexGadget_directive[1024];
uint8_t* alexGadget_directive_payload_ptr = NULL;
int      alexGadget_directive_payload_len = 0;
uint8_t alexGadget_event[1024];
#define  QUEUE_WIDTH 1024
uint8_t _usart_queue[QUEUE_WIDTH];

const uint8_t DISC_RESPONSE[] = {
0xF0, 0x02, 0x00, 0x00, 0x0A, 0x90, 0x04, 0x0A, 0x24, 0x0A, 0x0F, 0x41, 0x6C, 0x65, 0x78, 0x61,
0x2E, 0x44, 0x69, 0x73, 0x63, 0x6F, 0x76, 0x65, 0x72, 0x79, 0x12, 0x11, 0x44, 0x69, 0x73, 0x63,
0x6F, 0x76, 0x65, 0x72, 0x2E, 0x52, 0x65, 0x73, 0x70, 0x6F, 0x6E, 0x73, 0x65, 0x12, 0xE7, 0x03,
0x0A, 0xE4, 0x03, 0x0A, 0x0D, 0x42, 0x4D, 0x37, 0x38, 0x47, 0x41, 0x44, 0x47, 0x45, 0x54, 0x30,
0x30, 0x31, 0x12, 0x0A, 0x42, 0x4D, 0x37, 0x38, 0x47, 0x61, 0x64, 0x67, 0x65, 0x74, 0x1A, 0x1A,
0x4D, 0x49, 0x43, 0x52, 0x4F, 0x43, 0x48, 0x49, 0x50, 0x20, 0x42, 0x4D, 0x37, 0x38, 0x20, 0x47,
0x61, 0x64, 0x67, 0x65, 0x74, 0x20, 0x44, 0x65, 0x6D, 0x6F, 0x22, 0x09, 0x4D, 0x49, 0x43, 0x52,
0x4F, 0x43, 0x48, 0x49, 0x50, 0x5A, 0x1D, 0x0A, 0x0E, 0x41, 0x6C, 0x65, 0x78, 0x61, 0x49, 0x6E,
0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x12, 0x06, 0x41, 0x6C, 0x65, 0x72, 0x74, 0x73, 0x1A,
0x03, 0x31, 0x2E, 0x31, 0x5A, 0x38, 0x0A, 0x0E, 0x41, 0x6C, 0x65, 0x78, 0x61, 0x49, 0x6E, 0x74,
0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x12, 0x16, 0x41, 0x6C, 0x65, 0x78, 0x61, 0x2E, 0x47, 0x61,
0x64, 0x67, 0x65, 0x74, 0x2E, 0x4D, 0x75, 0x73, 0x69, 0x63, 0x44, 0x61, 0x74, 0x61, 0x1A, 0x03,
0x31, 0x2E, 0x30, 0x22, 0x09, 0x0A, 0x07, 0x0A, 0x05, 0x74, 0x65, 0x6D, 0x70, 0x6F, 0x5A, 0x3A,
0x0A, 0x0E, 0x41, 0x6C, 0x65, 0x78, 0x61, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65,
0x12, 0x17, 0x41, 0x6C, 0x65, 0x78, 0x61, 0x2E, 0x47, 0x61, 0x64, 0x67, 0x65, 0x74, 0x2E, 0x53,
0x70, 0x65, 0x65, 0x63, 0x68, 0x44, 0x61, 0x74, 0x61, 0x1A, 0x03, 0x31, 0x2E, 0x30, 0x22, 0x0A,
0x0A, 0x08, 0x0A, 0x06, 0x76, 0x69, 0x73, 0x65, 0x6D, 0x65, 0x5A, 0x6C, 0x0A, 0x0E, 0x41, 0x6C,
0x65, 0x78, 0x61, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x12, 0x1A, 0x41, 0x6C,
0x65, 0x78, 0x61, 0x2E, 0x47, 0x61, 0x64, 0x67, 0x65, 0x74, 0x2E, 0x53, 0x74, 0x61, 0x74, 0x65,
0x4C, 0x69, 0x73, 0x74, 0x65, 0x6E, 0x65, 0x72, 0x1A, 0x03, 0x31, 0x2E, 0x30, 0x22, 0x39, 0x0A,
0x08, 0x0A, 0x06, 0x61, 0x6C, 0x61, 0x72, 0x6D, 0x73, 0x0A, 0x08, 0x0A, 0x06, 0x74, 0x69, 0x6D,
0x65, 0x72, 0x73, 0x0A, 0x0B, 0x0A, 0x09, 0x72, 0x65, 0x6D, 0x69, 0x6E, 0x64, 0x65, 0x72, 0x73,
0x0A, 0x0A, 0x0A, 0x08, 0x77, 0x61, 0x6B, 0x65, 0x77, 0x6F, 0x72, 0x64, 0x0A, 0x0A, 0x0A, 0x08,
0x74, 0x69, 0x6D, 0x65, 0x69, 0x6E, 0x66, 0x6F, 0x5A, 0x24, 0x0A, 0x0E, 0x41, 0x6C, 0x65, 0x78,
0x61, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x12, 0x0D, 0x4E, 0x6F, 0x74, 0x69,
0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x73, 0x1A, 0x03, 0x31, 0x2E, 0x30, 0x62, 0x77,
0x0A, 0x03, 0x30, 0x30, 0x31, 0x12, 0x40, 0x34, 0x63, 0x65, 0x35, 0x37, 0x37, 0x38, 0x33, 0x63,
0x61, 0x65, 0x36, 0x61, 0x35, 0x65, 0x37, 0x35, 0x30, 0x62, 0x62, 0x34, 0x64, 0x34, 0x30, 0x31,
0x32, 0x61, 0x64, 0x38, 0x66, 0x62, 0x65, 0x38, 0x37, 0x35, 0x30, 0x31, 0x35, 0x31, 0x33, 0x65,
0x64, 0x63, 0x39, 0x32, 0x38, 0x30, 0x62, 0x62, 0x63, 0x39, 0x65, 0x62, 0x30, 0x32, 0x34, 0x30,
0x62, 0x33, 0x63, 0x66, 0x33, 0x39, 0x65, 0x1A, 0x01, 0x31, 0x22, 0x0E, 0x41, 0x31, 0x51, 0x34,
0x5A, 0x56, 0x5A, 0x37, 0x4F, 0x37, 0x39, 0x32, 0x34, 0x38, 0x2A, 0x0F, 0x42, 0x4D, 0x37, 0x38,
0x47, 0x61, 0x64, 0x67, 0x65, 0x74, 0x2D, 0x64, 0x65, 0x6D, 0x6F, 0x32, 0x0A, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x9B, 0x62, 0xF1,    
};
//@}
void configure_cdc_usart_callbacks(void);
void configure_ext1_usart_callbacks(void);
int printf_cdc(const char *fmt, ...);



/**
 * \brief Initialize tasks and resources for demo
 *
 * This function initializes the \ref oled1_xpro_io_group instance and the
 * \ref edbg_cdc_rx_group instance for reception, then creates all
 * the objects for FreeRTOS to run the demo.
 */

uint8_t * encode_alexaGadget_packet(uint8_t *payload_p, uint16_t len, uint16_t * len_encoded_p){
    static uint8_t seqId = 0;
    uint8_t chk[2];
    uint16_t checksum = 0; //Packet ID/cmd + Error ID + Payload (without escape)
    
    uint8_t *pack_payload_p;
    spp_packet_directive_header *pack_head_p;
    memset(alexGadget_event, 0, sizeof(alexGadget_event) );
    pack_head_p = alexGadget_event;
    pack_head_p->sof = SOP;
    pack_head_p->cmd = 0x02;//always 02
    pack_head_p->err = 0x00;//always 00
    pack_head_p->seqId = seqId++;
    
    pack_payload_p = alexGadget_event + sizeof(spp_packet_directive_header);
    checksum = pack_head_p->cmd + pack_head_p->err; //init the calculation
    
    
    for(uint16_t i=0; i<len; i++)
    {
        checksum += payload_p[i];
        switch( payload_p[i] ){
            case SOP:
            case EOP:
            case ESP:
                *pack_payload_p++ = ESP;
                *pack_payload_p++ = (payload_p[i]^ESP);
                break;
            default:
                *pack_payload_p++ = payload_p[i];
                break;
        }
    }
    
    //encode the checksum
    chk[0] = (uint8_t)((checksum&0xff00)>>8);
    chk[1] = (uint8_t)((checksum&0x00ff));
    for(uint16_t i=0; i<sizeof(chk); i++)
    {
        switch( chk[i] ){
            case SOP:
            case EOP:
            case ESP:
                *pack_payload_p++ = ESP;
                *pack_payload_p++ = (chk[i]^ESP);
                break;
            default:
               *pack_payload_p++ = chk[i];
               break;
        }
    }
    
    //mark the end
    *pack_payload_p++ = EOP;
    *len_encoded_p = (uint32_t)pack_payload_p - (uint32_t)pack_head_p;
    
    return pack_head_p;
}

#ifndef __ENABLE_RTOS__
//op: true for write queue, false for read queue
bool ext1_usart_queue_op(bool op, uint8_t* data_p)
{

    static uint16_t index_r = -1;//mark the initial state
    static int16_t index_w = 0;
    
    if(op)
    {
        bool overwritten = false;
        _usart_queue[index_w++] = *data_p;
        index_w = (index_w%QUEUE_WIDTH);
        
        if( index_w == index_r ){
            overwritten = true;
            printf_cdc("ext1 usart overwritten\r\n");
        }
        else
        overwritten = false;
        
        return overwritten;
    }
    else
    {
        if( ((index_r+1)%QUEUE_WIDTH) == index_w )
        return false;//empty
        else
        {
            index_r++;
            index_r = index_r%QUEUE_WIDTH;
            
            *data_p = _usart_queue[index_r];
            _usart_queue[index_r] = 0;//clear
            return true;
        }
    }
}
#endif

void LED_blink_timer_callback_to_on_led(struct tc_module *const module_inst)
{
    port_pin_set_output_level(LED_0_PIN, false);// turn on
}
void LED_blink_timer_callback_to_off_led(struct tc_module *const module_inst)
{
    port_pin_set_output_level(LED_0_PIN, true);// turn off
}

void LED_blink_timer_callback_for_overflow(struct tc_module *const module_inst){
    port_pin_set_output_level(LED_0_PIN, true);// turn off
}
void LED_blink_timer_init(void)
{
    //! [setup_config]
    struct tc_config config_tc;
    //! [setup_config]
    //! [setup_config_defaults]
    tc_get_config_defaults(&config_tc);
    //! [setup_config_defaults]

    //! [setup_change_config]
    config_tc.counter_size = TC_CTRLA_MODE_COUNT8;
    #if (SAMR30E)
    config_tc.clock_source = GCLK_GENERATOR_0;
    #else
    config_tc.clock_source = GCLK_GENERATOR_1;
    #endif
    config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1024;
    config_tc.counter_8_bit.period = 100;
    config_tc.counter_8_bit.compare_capture_channel[0] = config_tc.counter_8_bit.period*0.1;
    config_tc.counter_8_bit.compare_capture_channel[1] = config_tc.counter_8_bit.period*0.2;
    //! [setup_change_config]

    //! [setup_set_config]
    tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);
    //! [setup_set_config]
    //enable tc
    tc_enable(&tc_instance);
    
    //register callback
    //! [setup_register_callback]
    tc_register_callback(&tc_instance, LED_blink_timer_callback_for_overflow,
    TC_CALLBACK_OVERFLOW);
    tc_register_callback(&tc_instance, LED_blink_timer_callback_to_on_led,
    TC_CALLBACK_CC_CHANNEL0);
    tc_register_callback(&tc_instance, LED_blink_timer_callback_to_off_led,
    TC_CALLBACK_CC_CHANNEL1);
    //! [setup_register_callback]

    //! [setup_enable_callback]
    tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
    tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
    tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL1);
}
void LED_blink_timer_start(void)
{
    tc_start_counter(&tc_instance);
}
void LED_blink_timer_stop(void)
{
    tc_stop_counter(&tc_instance);
}

void ext1_usart_init(void)
{
    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);
    
    config_usart.baudrate = 115200;
    config_usart.mux_setting = EXT1_UART_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EXT1_UART_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EXT1_UART_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EXT1_UART_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EXT1_UART_SERCOM_PINMUX_PAD3;
    config_usart.stopbits = USART_STOPBITS_1;

    while( usart_init(&ext1_usart_instance, EXT1_UART_MODULE, &config_usart) );
    // Enable the UART transceiver
    usart_enable(&ext1_usart_instance);
    usart_enable_transceiver(&ext1_usart_instance, USART_TRANSCEIVER_TX);
    usart_enable_transceiver(&ext1_usart_instance, USART_TRANSCEIVER_RX);
    
    configure_ext1_usart_callbacks();
}
void cdc_usart_init(void)
{
    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);
    
    config_usart.baudrate = 115200;
    config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
    config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
    config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
    config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
    config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
    config_usart.stopbits = USART_STOPBITS_1;

    while( usart_init(&cdc_usart, EDBG_CDC_MODULE, &config_usart) );
    // Enable the UART transceiver
    usart_enable(&cdc_usart);
    usart_enable_transceiver(&cdc_usart, USART_TRANSCEIVER_TX);
    usart_enable_transceiver(&cdc_usart, USART_TRANSCEIVER_RX);
    
    configure_cdc_usart_callbacks();
}
void demotasks_init(void)
{
    uint8_t string[] = "\r\nHello, Alexa gadget Demo with BM78!\r\n-----------------------------------\r\n";
    //it takes too much timer to init timer and that will cause mcu to miss uart data
    
     port_pin_set_output_level(LED_0_PIN, false);
    
    LED_blink_timer_init();
    ext1_usart_init();
    //usart_write_buffer_wait(&ext1_usart_instance, string, sizeof(string));
    //usart_write_buffer_job(&ext1_usart_instance, string, sizeof(string));
    //configure_ext1_usart_callbacks();
    
    cdc_usart_init();
    //configure_cdc_usart_callbacks();
    //usart_write_buffer_wait(&cdc_usart, string, sizeof(string)-1/*exclude the 0 terminated*/);
    printf_cdc("%s",string);
    LED_blink_timer_stop();
}


/**
 * \brief Main demo task
 *
 * This task keeps track of which screen the user has selected, which tasks
 * to resume/suspend to draw the selected screen, and also draws the menu bar.
 *
 * The menu bar shows which screens the user can select by clicking the
 * corresponding buttons on the OLED1 Xplained Pro:
 * - \ref graph_task() "graph" (selected at start-up)
 * - \ref terminal_task() "term."
 * - \ref about_task() "about"
 *
 * \param params Parameters for the task. (Not used.)
 */

/**
 * \brief Graph task
 *
 * This task runs in the background to draw a pseudo-random graph to a dedicated
 * display buffer. If the user selects a different screen than the graph, it
 * will continue to update even though it is not visible until the graph screen
 * is selected again.
 *
 * \param params Parameters for the task. (Not used.)
 */

/**
 * \brief Terminal task
 *
 * This task prints the terminal text buffer to the display.
 *
 * \param params Parameters for the task. (Not used.)
 */

/**
 * \brief About task
 *
 * This task prints a short text about the demo, with a simple zooming
 * animation.
 *
 * \param params Parameters for the task. (Not used.)
 */

/**
 * \brief UART task
 *
 * This task runs in the background to handle the queued, incoming terminal
 * characters and write them to the terminal text buffer. It does not print
 * anything to the display -- that is done by \ref terminal_task().
 *
 * \param params Parameters for the task. (Not used.)
 */

/**
 * \internal
 * \brief UART interrupt handler for reception on EDBG CDC UART
 *
 * This function is based on the interrupt handler of the SERCOM USART callback
 * driver (\ref _usart_interrupt_handler()). It has been modified to only handle
 * the receive interrupt and to push the received data directly into the queue
 * for terminal characters (\ref terminal_in_queue), and echo the character back
 * to the sender.
 *
 * \param instance Instance number of SERCOM that generated interrupt.
 */


/* ************************************************************************
    replace normal printf for debugging
   
    input : ARG as normal printf("........\n\r");
    return : length been print

*/
char    _dstr[120];
int printf_cdc(const char *fmt, ...)
{
    SercomUsart *const usart_hw = (SercomUsart *)EDBG_CDC_MODULE;
    uint16_t interrupt_status;
    va_list argptr;
    int len = 1;

    memset(&_dstr, 'a', sizeof(_dstr) );
    va_start(argptr, fmt);
    len = vsprintf(_dstr, fmt, argptr);
    va_end(argptr);

   
        if ( (len > 0) && (len < sizeof(_dstr) ) )
        {   
            usart_write_buffer_wait(&cdc_usart,(uint8_t *)_dstr, len);
        }
    return len;
}


void cdc_usart_rx_request(void)
{
    usart_read_job(&cdc_usart, (uint8_t *)cdc_usart_rx_buffer);
}
void cdc_usart_rx_callback(struct usart_module *const usart_module)
{
    uint8_t data = (uint8_t)cdc_usart_rx_buffer[0];
    
    //request new data
    usart_read_job(&cdc_usart, (uint8_t *)cdc_usart_rx_buffer);
}

void cdc_usart_tx_request(uint8_t *tx_buffer, uint16_t len)
{
    usart_write_buffer_job(&cdc_usart,
    (uint8_t *)tx_buffer, len);
}
void cdc_usart_tx_callback(struct usart_module *const usart_module)
{
    //port_pin_toggle_output_level(LED_0_PIN);
}

void configure_cdc_usart_callbacks(void)
{
    //! [setup_register_callbacks]
    usart_register_callback(&cdc_usart,cdc_usart_tx_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&cdc_usart,cdc_usart_rx_callback, USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_register_callbacks]

    //! [setup_enable_callbacks]
    usart_enable_callback(&cdc_usart, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&cdc_usart, USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_enable_callbacks]
}
//========================================
void ext1_usart_rx_request(void)
{
    usart_read_job(&ext1_usart_instance, (uint8_t *)ext1_usart_rx_buffer);
}
void ext1_usart_rx_callback(struct usart_module *const usart_module)
{
    uint8_t data = (uint8_t)ext1_usart_rx_buffer[0];
    //store received data into queue
    if(! ext1_usart_queue_op(true, &data)){
        //overwritten!
    }
    //request new data: need?
    usart_read_job(&ext1_usart_instance, (uint8_t *)ext1_usart_rx_buffer);
}

void ext1_usart_tx_request(uint8_t *tx_buffer, uint16_t len)
{
    usart_write_buffer_job(&ext1_usart_instance,
    (uint8_t *)tx_buffer, len);
}
void ext1_usart_tx_callback(struct usart_module *const usart_module)
{
    port_pin_toggle_output_level(LED_0_PIN);
}

void configure_ext1_usart_callbacks(void)
{
    //! [setup_register_callbacks]
    usart_register_callback(&ext1_usart_instance,ext1_usart_tx_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&ext1_usart_instance,ext1_usart_rx_callback, USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_register_callbacks]

    //! [setup_enable_callbacks]
    usart_enable_callback(&ext1_usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&ext1_usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
    //! [setup_enable_callbacks]
}

void alexaGadget_task(void *params)
{
    PACK_PHASE pack_phase = INIT_PHASE;
    static int index = 0;
    static bool escape = false;
    uint32_t i = 0;
    
    uint8_t current_char;
    ext1_usart_rx_request(); 
    for (;;) {
        // Any characters queued? Handle them!
        if( ext1_usart_queue_op(false, &current_char))
        {
            //printf_cdc("%02x ", current_char);
			switch (pack_phase) {
			case INIT_PHASE:
                if( current_char == SOP ){
                    escape = FALSE;
                    index = 0;
                    memset(alexGadget_directive, 0, sizeof(alexGadget_directive));
                    
                    alexGadget_directive[index++] = current_char;
                    pack_phase = HEADER_PHASE;
                }                                                      
				break;

			case HEADER_PHASE:
                if( current_char == SOP ){
                    //a new SOP so drop the previous one.
                    pack_phase = INIT_PHASE;
                    escape = FALSE;
                    index = 0;
                    memset(alexGadget_directive, 0, sizeof(alexGadget_directive));
                }
                else{
                    alexGadget_directive[index++] = current_char;
                    if(index == sizeof(spp_packet_directive_header))
                        pack_phase = PAYLOAD_PHASE;
                }                         
                break;
                
            case PAYLOAD_PHASE:
                if( current_char == ESP ){
                    escape = TRUE;
                    continue;
                }
                
                if(escape == TRUE){
                    alexGadget_directive[index++] = (current_char^ESP);
                    escape = FALSE;
                }
                else
                    alexGadget_directive[index++] = current_char;
                    
                if(current_char == EOP)
                    pack_phase = TRAIL_PHASE;
                                        
                break;
                
            case TRAIL_PHASE:
                break;

			default:
				// For all other characters, just move head to next char
                break;
			}              
		}
        else{
            //printf_cdc("alexa has no message. going to sleep: %d\r\n", i++);
            //check the button status: failing edge or rising edge
            //send edge event to the skill and play the prompt
            //on failing edge: play 'Button is pressed'
            //on rising edge: play 'Button is released'
        }            
        
        //handle the directives
        if(pack_phase == TRAIL_PHASE){
            //parse the payload
            GADET_PACK_TYPE pack_type = decode_directive( (alexGadget_directive+sizeof(spp_packet_directive_header)),
                                                          (index - sizeof(spp_packet_directive_header) - sizeof(spp_packet_directive_trailer)),
                                                           &alexGadget_directive_payload_ptr,
                                                           &alexGadget_directive_payload_len );
            switch( pack_type ){
                case PACK_SET_ALERT:
                    printf_cdc("PACK_SET_ALERT instructive:\r\n");
                    break;
                case PACK_DEL_ALERT:
                    printf_cdc("PACK_DEL_ALERT instructive:\r\n");
                    break;
                case PACK_DISCOVER:
                    printf_cdc("PACK_DISCOVER instructive:\r\n");
                    uint16_t len = 0;
                    uint8_t* payload_p = encode_alexaGadget_discover_response_event(&len);
                    
                    //usart_write_buffer_wait(&cdc_usart,(uint8_t *)payload_p, len);
                    
                    uint16_t len_encode = 0;
                    uint8_t* pack_p =  encode_alexaGadget_packet(payload_p, len, &len_encode);
                    ext1_usart_tx_request(pack_p, len_encode);
                    
                    //indicate the gadget link ready
                    LED_blink_timer_start();
                    break;
                case PACK_TEMPO:
                    printf_cdc("PACK_TEMPO instructive:\r\n");
                    break;
                case PACK_SPEECH_MARKS:
                    printf_cdc("PACK_SPEECH_MARKS instructive:\r\n");
                    break;
                case PACK_STATE_UPATE:
                    printf_cdc("PACK_STATE_UPATE instructive:\r\n");
                    break;
                case PACK_SET_INDICATOR:
                    printf_cdc("PACK_SET_INDICATOR instructive:\r\n");
                    break;
                case PACK_CLEAR_INDICATOR:
                    printf_cdc("PACK_CLEAR_INDICATOR instructive:\r\n");
                    break;
                case PACK_CUSTOM_IF:
                    if( alexGadget_directive_payload_ptr ){
                         printf_cdc("alexGadget_directive_payload_ptr:%s", alexGadget_directive_payload_ptr);
                         cJSON *ledCtrl_json = cJSON_Parse(alexGadget_directive_payload_ptr);
                         const cJSON *ledId_cJSON, *ledState_cJSON;
                         if(ledCtrl_json){
                             ledId_cJSON = cJSON_GetObjectItem(ledCtrl_json, "ledId");
                             ledState_cJSON = cJSON_GetObjectItem(ledCtrl_json, "ledState");
                                                      
                             //printf_cdc("ledId=%d,ledState=%s\r\n", ledId_cJSON->valueint, ledState_cJSON->valuestring);
                             if(0==strcmp("on", ledState_cJSON->valuestring)){
                                 LED_blink_timer_stop();
                                 port_pin_set_output_level(LED_0_PIN, false);
                             }
                             else if(0==strcmp("off", ledState_cJSON->valuestring)){
                                 LED_blink_timer_stop();
                                 port_pin_set_output_level(LED_0_PIN, true);
                             }
                             else if(0==strcmp("blink", ledState_cJSON->valuestring)){
                                 LED_blink_timer_start();
                             }
                                                      
                                                      
                             //clean up
                             cJSON_Delete(ledCtrl_json);
                         }
                         
                         //clean up
                         free(alexGadget_directive_payload_ptr);
                         alexGadget_directive_payload_ptr = NULL;
                    }
                
                    printf_cdc("PACK_CUSTOM_IF instructive:\r\n");
                    break;
                case PACK_TYPE_INVALID:
                    printf_cdc("PACK_TYPE_INVALID instructive:\r\n");
                    break;
                case PACK_TYPE_UNKNOWN:
                default:
                    printf_cdc("PACK_TYPE_UNKNOWN instructive:\r\n");
                    break;
            }
            
            //reset the state
            pack_phase = INIT_PHASE;
            escape = FALSE;
            index = 0;
            memset(alexGadget_directive, 0, sizeof(alexGadget_directive));
        }

    }
}

void extint_detection_callback(void)
{
    int32_t len = 256, len_protocol;
    uint8_t buff[len];
    
        
    bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
    port_pin_set_output_level(LED_0_PIN, pin_state);

    char* namespace = "Custom.BM78";
    char* name = "BUTTON";
    char* payload_rising = "{\"edge\":\"rising\", \"id\" : 0}";
    char* payload_failing = "{\"edge\":\"failing\", \"id\" : 0}";

    if(pin_state)
      len_protocol = encode_custom_event(buff, len, namespace, name, payload_rising);
    else
      len_protocol = encode_custom_event(buff, len, namespace, name, payload_failing);
      
    usart_write_buffer_wait(&cdc_usart,(uint8_t *)buff, len);
    
    uint16_t len_encode = 0;
    uint8_t* pack_p =  encode_alexaGadget_packet(buff, len_protocol, &len_encode);
    ext1_usart_tx_request(pack_p, len_encode);
    
}

/** @} */
