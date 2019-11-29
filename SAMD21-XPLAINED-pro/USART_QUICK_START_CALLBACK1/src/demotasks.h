/**
 * \file
 *
 * \brief FreeRTOS demo tasks header.
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

#ifndef DEMOTASKS_H
#define DEMOTASKS_H

/**
 * \defgroup freertos_sam0_demo_tasks_group FreeRTOS demo tasks
 *
 * The demo tasks demonstrate basic use of FreeRTOS, with inter-task
 * communication using queues and mutexes.
 *
 * For details on how the demo works, see \ref appdoc_intro.
 *
 * For detailed information on the tasks, see:
 * - \ref main_task()
 * - \ref graph_task()
 * - \ref terminal_task()
 * - \ref about_task()
 * - \ref uart_task()
 *
 * The demo tasks depend on the following drivers:
 * - \ref oled1_xpro_io_group
 * - \ref edbg_cdc_rx_group
 * - \ref asfdoc_common2_gfx_mono
 *
 * @{
 */
#define SOP 0xF0
#define EOP 0xF1
#define ESP 0xF2
typedef enum{
    INIT_PHASE = 0,
    HEADER_PHASE = 1,
    PAYLOAD_PHASE = 2,
    TRAIL_PHASE = 3,
}PACK_PHASE;

#define TRUE 1
#define FALSE 0

typedef struct
{
    uint8_t sof;
    uint8_t cmd;
    uint8_t err;
    uint8_t seqId;
} __attribute__((packed)) spp_packet_directive_header;

typedef struct
{
    uint16_t checksum;
    uint8_t eof;
}__attribute__((packed)) spp_packet_directive_trailer;

typedef struct
{
    spp_packet_directive_header header;
    uint8_t data[];
}__attribute__((packed))spp_directive_response_packet;

void demotasks_init(void);
void extint_detection_callback(void);
void alexaGadget_task(void *params);
/** @} */

#endif // DEMO_TASKS_H
