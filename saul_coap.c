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

#include <assert.h>
#include <stdio.h>
#include "saul_reg.h"
#include "net/gcoap.h"

extern char *make_msg(char *, ...);

static ssize_t _saul_cnt_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);

/* CoAP resources. Must be sorted by path (ASCII order). */
static const coap_resource_t _resources[] = {
    { "/saul", COAP_GET, _saul_cnt_handler, NULL },
};

static gcoap_listener_t _listener = {
    &_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    NULL
};

static ssize_t _saul_cnt_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx)
{
    saul_reg_t *dev = saul_reg;
    int i = 0;
    char *payl;

    (void)ctx;

    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    if (!dev) {
	payl = make_msg("No devices found");
    }
    else {
        while (dev) {
            /*
            payl = make_msg("%s%i,%s\n",
                            payl,
                            i++,
                            saul_class_to_str(dev->driver->type),
                            dev->name);
            */
            i++;
            dev = dev->next;
        }

	payl = make_msg("%d devices available", i);
    }

    if (pdu->payload_len >= strlen(payl)) {
        memcpy(pdu->payload, payl, strlen(payl));
        free(payl)
        return resp_len + strlen(payl);
    }
    else {
        printf("saul_coap: msg buffer (size: %d) too small"
               " for payload (size: %d)\n",
               pdu->payload_len, strlen(payl));
        free(payl)
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }

    return 0;
}

void saul_coap_init(void)
{
    gcoap_register_listener(&_listener);
}
