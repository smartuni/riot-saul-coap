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
 * @brief       Register SAUL registry to resource directory and update it periodically.
 *
 * @author      Matthias Bräuer <matthias.braeuer@haw-hamburg.de>
 *
 * @}
 */
#ifndef SAUL_CORD_EP_H
#define SAUL_CORD_EP_H

#include "net/nanocoap.h"
#include "net/cord/config.h"
#include "net/cord/ep.h"
#include "net/sock/util.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief Possible types of events when interacting with a Resource Directory. 
 */
typedef enum {
   SAUL_CORD_EP_REGISTERED,
   SAUL_CORD_EP_DEREGISTERED,
   SAUL_CORD_EP_UPDATED,
} saul_cord_ep_event_t;

/**
 * @brief Callback function signature for RD endpoint state synchronization.
 *
 * The registered callback function is executed in the context of the dedicated
 * RD endpoint's thread.
 *
 * @param[in] event Type of event
 */
typedef void (*saul_cord_ep_cb_t)(saul_cord_ep_event_t event);

/**
 * @brief Create the saul-cord thread.
 *
 * @param[in] ip_address The IPv6 address of the resource directory service
 *
 * @warning This function must only be called once (typically during system
 *          initialization)
 */
void saul_cord_ep_create(const char *ip_address);

/**
 * @brief Run the saul-cord thread. 
 *
 * @warning This function must only be called once (typically during system
 *          initialization)
 */
void saul_cord_ep_run(void);

/**
 * @brief Register a callback to be notified about RD endpoint state changes.
 *
 * Only a single callback can be active at any point in time, so setting a new
 * callback will override the existing one.
 *
 * @pre @p cb != NULL
 *
 * @param[in] cb The callback to be executed on RD endpoint state changes 
 */
void saul_cord_ep_register_cb(saul_cord_ep_cb_t cb);


#ifdef __cplusplus
}
#endif

#endif /* SAUL_CORD_EP_H */

