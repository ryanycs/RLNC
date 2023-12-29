#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/rlnc.h"
#include "galois.h"

struct rlnc_decode_ctx *rlnc_decoder_ctx_init(struct rlnc_parameters *params) {
    struct rlnc_decode_ctx *ctx = (struct rlnc_decode_ctx*)malloc(sizeof(struct rlnc_decode_ctx));
    ctx->params = params;
    ctx->count = 0;
    ctx->finished = 0;
    ctx->rank = 0;
    ctx->matrix_coeff = (GF_ELEMENT**)calloc(params->generation_size, sizeof(GF_ELEMENT*));
    ctx->matrix_payload = (GF_ELEMENT**)calloc(params->generation_size, sizeof(GF_ELEMENT*));

    return ctx;
}

/*
 *
 */
void decode_packet(struct rlnc_decode_ctx *ctx, struct packet *pkt) {
    if (ctx->finished) {
        return;
    }

    ctx->count += 1;

    // GF_ELEMENT *coeff = calloc(ctx->generation_size, sizeof(GF_ELEMENT));
    // for (int i = 0; i < ctx->generation_size; ++i) {
    //     coeff[i] = pkt->coeff[i];
    // }

    // upper triangular matrix
    for (int i = 0; i < ctx->params->generation_size; ++i) {
        if (pkt->coeff[i] == 0) {
            continue;
        }

        if (ctx->matrix_coeff[i] == NULL) {
            ctx->matrix_coeff[i] = (GF_ELEMENT*)calloc(ctx->params->generation_size, sizeof(GF_ELEMENT));
            ctx->matrix_payload[i] = (GF_ELEMENT*)calloc(ctx->params->pkt_size, sizeof(GF_ELEMENT));
            memcpy(ctx->matrix_coeff[i], pkt->coeff, ctx->params->generation_size * sizeof(GF_ELEMENT));
            memcpy(ctx->matrix_payload[i], pkt->payload, ctx->params->pkt_size * sizeof(GF_ELEMENT));
            ctx->rank += 1;
            break;
        } else {
            GF_ELEMENT quotient = galois_div(pkt->coeff[i], ctx->matrix_coeff[i][i]);
            for (int j = 0; j < ctx->params->generation_size; ++j) {
                pkt->coeff[j] = galois_sub(pkt->coeff[j], galois_mul(quotient, ctx->matrix_coeff[i][j]));
            }
            for (int j = 0; j < ctx->params->pkt_size; ++j) {
                pkt->payload[j] = galois_sub(pkt->payload[j], galois_mul(quotient, ctx->matrix_payload[i][j]));
            }
        }
    }

    // gaussian jordan elimination
    if (ctx->rank == ctx->params->generation_size) {
        print_matrix(ctx);
        for (int i = ctx->params->generation_size - 1; i >= 0; --i) {
            int pivot = ctx->matrix_coeff[i][i];
            for (int j = 0; j < i; ++j) {
                GF_ELEMENT quotient = galois_div(ctx->matrix_coeff[j][i], pivot);
                for (int k = 0; k < ctx->params->generation_size; ++k) {
                    ctx->matrix_coeff[j][k] = galois_sub(ctx->matrix_coeff[j][k], galois_mul(quotient, ctx->matrix_coeff[i][k]));
                }
                for (int k = 0; k < ctx->params->pkt_size; ++k) {
                    ctx->matrix_payload[j][k] = galois_sub(ctx->matrix_payload[j][k], galois_mul(quotient, ctx->matrix_payload[i][k]));
                }
            }

            for (int j = 0; j < ctx->params->generation_size; ++j) {
                ctx->matrix_coeff[i][j] = galois_div(ctx->matrix_coeff[i][j], pivot);
            }
            for (int j = 0; j < ctx->params->pkt_size; ++j) {
                ctx->matrix_payload[i][j] = galois_div(ctx->matrix_payload[i][j], pivot);
            }

        }
        ctx->finished = 1;
    }

    // free(coeff);
}

void print_matrix(struct rlnc_decode_ctx *ctx) {
    for (int i = 0; i < ctx->params->generation_size; ++i) {
        for (int j = 0; j < ctx->params->generation_size; ++j) {
            printf("%2X ", ctx->matrix_coeff[i][j]);
        }
        printf("| ");
        for (int j = 0; j < ctx->params->pkt_size; ++j) {
            printf("%2X ", ctx->matrix_payload[i][j]);
        }
        printf("\n");
    }
}

