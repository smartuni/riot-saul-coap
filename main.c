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
#include "net/cord/config.h"
#include "net/cord/ep.h"
#include "net/nanocoap.h"
#include "net/sock/util.h"
#include "net/cord/ep_standalone.h"

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
            break;
        case CORD_EP_UPDATED:
            puts("DEBUG: RD endpoint event: successfully updated client registration");
            break;
    }
}

//static int make_sock_ep(sock_udp_ep_t *ep, const char *addr)
//{
//    ep->port = 0;
//    if (sock_udp_str2ep(ep, addr) < 0) {
//        return -1;
//    }
//    ep->family  = AF_INET6;
//    ep->netif   = SOCK_ADDR_ANY_NETIF;
//    if (ep->port == 0) {
//        ep->port = COAP_PORT;
//    }
//    return 0;
//}
//
//static int _register(char* address) {
//    sock_udp_ep_t remote;
//    char *regif = NULL;
//
//    if (make_sock_ep(&remote, address) < 0) {
//        puts("error: unable to parse address\n");
//        return 1;
//    }
//     
//    puts("Registering with RD now, this may take a short while...");
//    
//    if (cord_ep_register(&remote, regif) != CORD_EP_OK) {
//        puts("error: registration failed");
//        return 1; 
//    }
//    else {
//        puts("registration successful\n");
//        cord_ep_dump_status();
//    }
//
//    return 0;
//}
//
int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, type `saul` to see all SAUL devices\n");

    saul_coap_init();

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    cord_ep_standalone_reg_cb(_on_ep_event);
    //cord_ep_standalone_run();
   
    printf("DEBUG_RD_ADDRESS: %s", CORD_EP_ADDRESS);
    
   // while(_register(CORD_EP_ADDRESS)){
   //     puts("Try again to register to RD deamon");
   // }
   
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
