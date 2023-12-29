#include <stdio.h>
#include <stdlib.h>
#include "../include/rlnc.h"

int main() {
    struct rlnc_parameters *params = (struct rlnc_parameters*)malloc(sizeof(struct rlnc_parameters));
    params->gf_power = 8;
    params->generation_size = 8;
    params->buf_size = 128;
    params->pkt_size = 16;

    // initialize galois field
    galois_init(params->gf_power);

    // initialize encoder
    struct rlnc_encode_ctx *ctx = rlnc_encoder_ctx_init(params);

    // initialize buffer
    GF_ELEMENT buf[128] = {0};
    for (int i = 0; i < 128; ++i) {
        buf[i] = i;
    }

    // encode packet
    for (int i = 0; i < 12; ++i) {
        struct packet *pkt = rlnc_packet_init(params);
        encode_packet(ctx, pkt, buf);
        print_packet(pkt, params->generation_size, params->pkt_size);
        free_packet(pkt);
    }

    free_galois();

    return 0;
}