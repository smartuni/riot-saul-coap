/*
 * Copyright (C) 2019 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     pkg
 * @{
 *
 * @file
 * @brief       CoAP endpoint for the SAUL registry
 *
 * @author      Micha Rosenbaum <micha.rosenbaum@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "shell.h"
#include "saul_cord_ep.h"

#define MAIN_QUEUE_SIZE (4)
#define CORD_EP_ADDRESS "[fdaa:bb:cc:dd::1]:5683"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern void saul_coap_init(void);

/* we will use custom event handler for dumping cord_ep events */
static void _on_ep_event(cord_ep_standalone_event_t event)
{
    switch (event) {
        case CORD_EP_REGISTERED:
            puts("DEBUG: RD endpoint event: now registered with a RD");
            break;
        case CORD_EP_DEREGISTERED:
            puts("DEBUG: RD endpoint event: dropped client registration");
            puts("DEBUG: You should try to register again:)");
            //saul_cord_ep_register(CORD_EP_ADDRESS);
            break;
        case CORD_EP_UPDATED:
            puts("DEBUG: RD endpoint event: successfully updated client registration");
            break;
    }
}

int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, type `saul` to see all SAUL devices\n");

    saul_coap_init();

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    saul_cord_ep_register_cb(_on_ep_event);
    saul_cord_ep_register(CORD_EP_ADDRESS); 
   
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}

