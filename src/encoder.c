#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/rlnc.h"
#include "galois.h"

struct rlnc_encode_ctx *rlnc_encoder_ctx_init(struct rlnc_parameters *params) {
    struct rlnc_encode_ctx *ctx = (struct rlnc_encode_ctx*)malloc(sizeof(struct rlnc_encode_ctx));
    ctx->params = params;
    ctx->count = 0;

    return ctx;
}

struct packet *rlnc_packet_init(struct rlnc_parameters *params) {
    struct packet *pkt = (struct packet*)malloc(sizeof(struct packet));
    pkt->coeff = (GF_ELEMENT*)calloc(params->generation_size, sizeof(GF_ELEMENT));
    pkt->payload = (GF_ELEMENT*)calloc(params->pkt_size, sizeof(GF_ELEMENT));

    return pkt;
}

/*
 * 
 */
void encode_packet(struct rlnc_encode_ctx *ctx, struct packet *pkt, GF_ELEMENT *buf) {
    // copy buf to pkt if count < generation_size
    if (ctx->count < ctx->params->generation_size) {
        // set coefficients
        pkt->coeff[ctx->count] = 1;

        // set payload
        memcpy(pkt->payload, buf + ctx->count * ctx->params->pkt_size, ctx->params->pkt_size);

        ctx->count += 1;
        return;
    }

    // generate coded packet
    int coeff; // coefficient
    for (int i = 0; i < ctx->params->generation_size; ++i) {
        coeff = rand() % (1 << ctx->params->gf_power);

        // set coefficient
        pkt->coeff[i] = coeff;

        // set payload
        for (int j = 0; j < ctx->params->pkt_size; ++j) {
            if (coeff == 0) {
                continue;
            } else {
                // payload[j] += coeff * buf[i][j]
                pkt->payload[j] = galois_add(pkt->payload[j], galois_mul(coeff, buf[i * ctx->params->pkt_size + j]));
            }
        }
    }
    ctx->count += 1;
    return;
}

/*
 * Serialize a packet to a byte string
 */
char *serialize_packet(struct packet *pkt, struct rlnc_parameters *params) {
    char *buf = (char*)malloc((params->generation_size + params->pkt_size) * sizeof(char));
    memcpy(buf, pkt->coeff, params->generation_size * sizeof(char));
    memcpy(buf + params->generation_size, pkt->payload, params->pkt_size * sizeof(char));
}

/*
 * Deserialize a packet from a byte string
 */
struct packet *deserialize_packet(char *buf, struct rlnc_parameters *params) {
    struct packet *pkt = (struct packet*)malloc(sizeof(struct packet));
    pkt->coeff = (GF_ELEMENT*)calloc(params->generation_size, sizeof(GF_ELEMENT));
    pkt->payload = (GF_ELEMENT*)calloc(params->pkt_size, sizeof(GF_ELEMENT));
    memcpy(pkt->coeff, buf, params->generation_size * sizeof(GF_ELEMENT));
    memcpy(pkt->payload, buf + params->generation_size, params->pkt_size * sizeof(GF_ELEMENT));

    return pkt;
}

void print_packet(struct packet *pkt, int generation_size, int pkt_size) {
    for (int i = 0; i < generation_size; ++i) {
        printf("%2X ", pkt->coeff[i]);
    }
    printf("| ");

    for (int i = 0; i < pkt_size; ++i) {
        printf("%2X ", pkt->payload[i]);
    }
    printf("\n");
}

/*
 * Free memory allocated for a packet
 */
void free_packet(struct packet *pkt) {
    free(pkt->coeff);
    free(pkt->payload);
    free(pkt);
}