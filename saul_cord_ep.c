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

#define ENABLE_DEBUG    (0)
#include "debug.h"

/* stack configuration */
#define STACKSIZE           (THREAD_STACKSIZE_DEFAULT)
#define PRIO                (THREAD_PRIORITY_MAIN - 1)
#define TNAME               "cord_ep"

#define UPDATE_TIMEOUT      (0xe537)

#define TIMEOUT_US          ((uint64_t)(CORD_UPDATE_INTERVAL * US_PER_SEC))

static char _stack[STACKSIZE];

static xtimer_t _timer;
static kernel_pid_t _runner_pid;
static msg_t _msg;

static saul_cord_ep_cb_t _cb = NULL;

// The ipv6 address (and port) of the resource directory
static const char *ip = NULL;

static void _set_timer(void)
{
    xtimer_set_msg64(&_timer, TIMEOUT_US, &_msg, _runner_pid);
}

/**
 * @brief Execute the callback function if present.
 *
 * @param[in] event The event that occurred
 */
static void _notify(saul_cord_ep_event_t event) {
    if (_cb != NULL) {
        _cb(event);
    }
}

/**
 * @brief Create an UDP socket.
 *
 * @param[out] ep Will contain the enpoint of the IPv6 address
 * @param[in] addr The IPv6 address to be used for the socket
 * @return `0` on success, else `-1`
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
 * @brief Try to register to resource directory service.
 */
static int register_rd(void) {
    sock_udp_ep_t remote;
    char *regif = NULL;

    if (make_sock_ep(&remote, ip) < 0) {
        puts("error: unable to parse address\n");
        return 1;
    }
     
    puts("Registering with RD now, this may take a short while...");
    
    if (cord_ep_register(&remote, regif) != CORD_EP_OK) {
        _notify(SAUL_CORD_EP_DEREGISTERED);
        return 1; 
    }

    _notify(SAUL_CORD_EP_REGISTERED);

    return 0;
}

static void saul_cord_ep_register(void) {
    printf("DEBUG: RD-ADDRESS: %s\n", ip);

    while (register_rd()) {
        puts("Try again to register to RD deamon");
    }

    cord_ep_dump_status();
}

static void *_reg_runner(void *arg)
{
    (void)arg;
    msg_t in;

    /* prepare context and message */
    _runner_pid = thread_getpid();
    _msg.type = UPDATE_TIMEOUT;

    while (1) {
        msg_receive(&in);
        if (in.type == UPDATE_TIMEOUT) {
            if (cord_ep_update() == CORD_EP_OK) {
                _set_timer();
                _notify(SAUL_CORD_EP_UPDATED);
            }
            else {
                _notify(SAUL_CORD_EP_DEREGISTERED);
                saul_cord_ep_register();
                _set_timer(); 
            }
        }
    }

    return NULL; /* should never be reached */
}

void saul_cord_ep_register_cb(saul_cord_ep_cb_t cb) {
    _cb = cb;
}

void saul_cord_ep_create(const char* ip_address)
{
    ip = ip_address;
    thread_create(_stack, sizeof(_stack), PRIO, THREAD_CREATE_STACKTEST,
                  _reg_runner, NULL, TNAME);
}

void saul_cord_ep_run(void) {
    _set_timer();    
}

