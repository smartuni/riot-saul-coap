/*
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

#include "net/nanocoap.h"
#include "net/cord/config.h"
#include "net/cord/ep.h"
#include "net/sock/util.h"
#include "net/cord/ep_standalone.h"

/**
 * Register saul registry to a resource directory service.
 *
 * @param ip_address The IPv6 address of the resource directory service
 */
void saul_cord_ep_register(const char *ip_address);

/**
 * Register a callback to resource directory functions.
 *
 * This callback is called when an answer from the resource
 * directory is received.
 *
 * @param cb The callback to be called
 */
void saul_cord_ep_register_cb(cord_ep_standalone_cb_t cb);

