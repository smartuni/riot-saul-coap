/**
 * Copyright (C) 2019 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @ingroup     pkg
 * @{
 *
 * @file
 * @brief       Register SAUL registry to resource directory
 *
 * @author      Matthias Bräuer <matthias.braeuer@haw-hamburg.de>
 *
 * @}
 */

#include "saul_cord_ep.h"

/**
 * Execute the `ifconfig` command.
 */
extern int _gnrc_netif_config(int argc, char **argv);

/**
 * Create an UDP socket.
 */
static int make_sock_ep(sock_udp_ep_t *ep, const char *addr)
{
    ep->port = 0;
    if (sock_udp_str2ep(ep, addr) < 0) {
        return -1;
    }
    ep->family  = AF_INET6;
    ep->netif   = SOCK_ADDR_ANY_NETIF;
    if (ep->port == 0) {
        ep->port = COAP_PORT;
    }
    return 0;
}

/**
 * Try to register to resource directory service.
 *
 * @param address The IPv6 address of the resource directory service
 */
static int register_rd(const char* address) {
    sock_udp_ep_t remote;
    char *regif = NULL;

    if (make_sock_ep(&remote, address) < 0) {
        puts("error: unable to parse address\n");
        return 1;
    }
     
    puts("Registering with RD now, this may take a short while...");
    
    if (cord_ep_register(&remote, regif) != CORD_EP_OK) {
        puts("error: registration failed");
        return 1; 
    }
    else {
        puts("registration successful\n");
        cord_ep_dump_status();
    }

    return 0;
}

void saul_cord_ep_register_cb(cord_ep_standalone_cb_t cb) {
    cord_ep_standalone_reg_cb(cb);
}

void saul_cord_ep_register(const char* ip_address) {
    printf("DEBUG_RD_ADDRESS: %s\n", ip_address);

    while(register_rd(ip_address)){
        _gnrc_netif_config(0, NULL);
        puts("Try again to register to RD deamon");
    }
}

