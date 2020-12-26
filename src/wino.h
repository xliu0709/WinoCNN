#ifndef _WINO_H_
#define _WINO_H_
#include "wino_macro.h"
#include <ap_int.h>

void wino_systolic_top(
    ap_uint<128> *input_DDR0,
    ap_uint<128> *input_DDR1,
    ap_uint<128> *input_DDR2,
    ap_uint<128> *input_DDR3,
    WEIGHT_PORTS_DECLARE(weight_DDR),
    // ap_uint<128> *weight_DDR0,
    // ap_uint<128> *weight_DDR1,
    // ap_uint<128> *weight_DDR2,
    // ap_uint<128> *weight_DDR3,
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR0,
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR1,
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR2,
    ap_uint<ODDR_WIDTH*BATCH_SIZE*OUT_PORT_BATCH_NUM> *output_DDR3,
    ap_int<32> *mem_params,
    ap_int<32> *bias_mem
    #ifdef __SDSVHLS__
    ,ap_uint<1> ap_clk_div2
    #endif
    );


    
#endif