#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/rlnc.h"

int main() {
    srand(time(NULL));

    struct rlnc_parameters *params = (struct rlnc_parameters*)malloc(sizeof(struct rlnc_parameters));
    params->gf_power = 8;
    params->generation_size = 4;
    params->buf_size = 128;
    params->pkt_size = 16; // RTP + G.711 = 12 + 160 = 172

    galois_init(params->gf_power);
    struct rlnc_encode_ctx *encode_ctx = rlnc_encoder_ctx_init(params);
    struct rlnc_decode_ctx *decode_ctx = rlnc_decoder_ctx_init(params);

    GF_ELEMENT buf[128] = {0};
    for (int i = 0; i < 128; ++i) {
        buf[i] = i % (1 << params->gf_power);
    }

    struct packet **pkts = (struct packet**)malloc(24 * sizeof(struct packet*));
    for (int i = 0; i < 24; ++i) {
        pkts[i] = rlnc_packet_init(params);
    }

    printf("Original packets:\n");
    for (int i = 0; i < 24; ++i) {
        encode_packet(encode_ctx, pkts[i], buf);
        print_packet(pkts[i], params->generation_size, params->pkt_size);
    }

    printf("\n");

    for (int i = 0; i < 24; ++i) {
        decode_packet(decode_ctx, pkts[rand() % 24]);
    }

    // for (int i = 4; i < 24 && decode_ctx->finished == 0; ++i) {
    //     decode_packet(decode_ctx, pkts[i]);
    // }

    printf("\n");

    printf("Decode, received %d packets\n", decode_ctx->count);
    print_matrix(decode_ctx);

    if (decode_ctx->finished) {
        printf("Decoding finished.\n");
    } else {
        printf("Decoding not finished.\n");
        printf("Rank: %d\n", decode_ctx->rank);
    }

    return 0;
}