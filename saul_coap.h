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
 * @brief       Adapt SAUL registry for CoAP.
 *
 * @author      Micha Rosenbaum <micha.rosenbaum@haw-hamburg.de>
 *
 * @}
 */
#ifndef SAUL_COAP_H
#define SAUL_COAP_H

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief Initialize and register the saul-coap resources.
 *
 * @warning This function must only be called once (typically during system
 *          initialization)
 */
void saul_coap_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SAUL_COAP_H */
