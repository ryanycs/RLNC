#ifndef RLNC_H
#define RLNC_H

#include "../src/galois.h"

struct packet {
    int gid; // generation id
    GF_ELEMENT *coeff;
    GF_ELEMENT *payload;
};

struct rlnc_parameters {
    int gf_power; // power of the galois field (2^gf_power)
    int generation_size; // number of symbols in a generation
    int buf_size; // size of the buffer
    int pkt_size; // size of a packet
};

struct rlnc_encode_ctx {
    struct rlnc_parameters *params;
    // int buf_size; // size of the buffer
    // int pkt_size; // size of a packet
    // int gf_power; // power of the galois field (2^gf_power)
    // int generation_size; // number of symbols in a generation
    int count; // number of packets encoded
};

struct rlnc_decode_ctx {
    struct rlnc_parameters *params;
    // int buf_size; // size of the buffer
    // int pkt_size; // size of a packet
    // int gf_power; // power of the galois field (2^gf_power)
    // int generation_size; // number of symbols in a generation
    int count; // number of packets decoded
    int finished; // whether decoding is finished
    int rank; // rank of the decoding matrix
    GF_ELEMENT **matrix_coeff; // coefficients of the decoding matrix
    GF_ELEMENT **matrix_payload; // payloads of the decoding matrix
};

struct rlnc_encode_ctx *rlnc_encoder_ctx_init();
struct rlnc_decode_ctx *rlnc_decoder_ctx_init();
struct packet *rlnc_packet_init(struct rlnc_parameters *params);
void encode_packet(struct rlnc_encode_ctx *ctx, struct packet *pkt, GF_ELEMENT *buf);
void decode_packet(struct rlnc_decode_ctx *ctx, struct packet *pkt);
void print_packet(struct packet *pkt, int generation_size, int pkt_size);
void free_packet(struct packet *pkt);

void print_matrix(struct rlnc_decode_ctx *ctx);

#endif /* RLNC_H */