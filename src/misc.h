#ifndef _MISC_H_
#define _MISC_H_
#include <ap_int.h>

#define MISC_INBUFF_DEPTH_BITS 8
#define MISC_INBUFF_DEPTH (1<<MISC_INBUFF_DEPTH_BITS)

#define MISC_OUTBUFFER_DEPTH_BITS 8
#define MISC_OUTBUFFER_DEPTH (1<<MISC_INBUFF_DEPTH_BITS)
void misc_layer(
    ap_uint<128>* in0,
    ap_uint<128>* in1,
    ap_uint<128>* out0,
    ap_uint<128>* out1,
    ap_uint<128>* weight0,
    ap_uint<128>* weight1,
    ap_uint<128>* weight2,
    ap_uint<128>* weight3,
    MiscDesc_t &misc_desc
);

#endif