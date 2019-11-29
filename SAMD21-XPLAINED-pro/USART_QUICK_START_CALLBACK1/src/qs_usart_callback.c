/**
 * \file
 *
 * \brief SAM USART Quick Start
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include "demotasks.h"

void configure_extint_channel(void);
void configure_extint_callbacks(void);

//! [setup]
void configure_extint_channel(void)
{
    //! [setup_1]
    struct extint_chan_conf config_extint_chan;
    //! [setup_1]
    //! [setup_2]
    extint_chan_get_config_defaults(&config_extint_chan);
    //! [setup_2]

    //! [setup_3]
    config_extint_chan.gpio_pin           = BUTTON_0_EIC_PIN;
    config_extint_chan.gpio_pin_mux       = BUTTON_0_EIC_MUX;
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
    //! [setup_3]
    //! [setup_4]
    extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
    //! [setup_4]
}

void configure_extint_callbacks(void)
{
    //! [setup_5]
    extint_register_callback(extint_detection_callback,
    BUTTON_0_EIC_LINE,
    EXTINT_CALLBACK_TYPE_DETECT);
    //! [setup_5]
    //! [setup_6]
    extint_chan_enable_callback(BUTTON_0_EIC_LINE,
    EXTINT_CALLBACK_TYPE_DETECT);
    //! [setup_6]
}

//! [setup_7]

//! [setup_7]
//! [setup]

int main(void)
{
    uint32_t tmp = 0x55AA55AA;
	system_init();
    demotasks_init( );
    
    //INIT INTR FOR BUTTON0
    configure_extint_channel();
    configure_extint_callbacks();
    
	system_interrupt_enable_global();
	while (true) {
        alexaGadget_task(&tmp);
	}
}

