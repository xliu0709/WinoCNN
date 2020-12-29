#include <cstdlib>
#include <iostream>
#include "../src/wino_macro.h"
// #include <ap_int.h>
#ifndef _WINO_GOLD_H_
#define _WINO_GOLD_H_


#include "../src/wino_struct.h"



#define GOLD_OUT_WIDTH 32

void wino_model_int(
    int8_t* input,
    int input_depth,
    int input_height,
    int input_width,
    int8_t* output,
    int output_depth,
    int output_height,
    int output_width,
    int8_t* weight,
    short* bias,
    int kernel_size,
    int pad_size,
    int stride,
    bool relu_flag,
    int Scale_oback_int
);

void wino_model_float(
    float* input,
    int input_depth,
    int input_height,
    int input_width,
    float* output,
    int output_depth,
    int output_height,
    int output_width,
    float* weight,
    float* bias,
    int kernel_size,
    int pad_size,
    float Scale_oback
);

void wino_model_float_offset(
    float* input,
    int input_depth,
    int input_height,
    int input_width,
    float* output,
    int output_depth,
    int output_height,
    int output_width,
    float* weight,
    float* bias,
    int kernel_size,
    int pad_size,
    int stride,
    bool relu_flag,
    int Scale_oback_int
);


// void winograd_algorithm_gold_raw(
//     ap_int<8>* input,
//     int input_depth,
//     int input_height,
//     int input_width,
//     ap_int<GOLD_OUT_WIDTH>* output,
//     int output_depth,
//     int output_height,
//     int output_width,
//     ap_int<8>* weight,
//     int wino_output_tile_size,
//     int pad_size
// );



// void winograd_algorithm_gold_row_wise(
//     ap_int<8>* input_DDR1,
//     ap_int<8>* input_DDR2,
//     ap_int<8>* input_DDR3,
//     ap_int<8>* input_DDR4,
//     int input_depth,
//     int input_height,
//     int input_width,
//     ap_int<GOLD_OUT_WIDTH>* output1,
//     ap_int<GOLD_OUT_WIDTH>* output2,
//     int output_depth,
//     int output_height,
//     int output_width,
//     ap_int<8>* weight,
//     int wino_input_tile_size,
//     int wino_output_tile_size,
//     int weight_outdepth_load_number,
//     int pad_size
// );

// void winograd_algorithm_gold_rowwise(
//     ap_int<8>* input0,
//     ap_int<8>* input1,
//     ap_int<GOLD_OUT_WIDTH>* output0,
//     ap_int<GOLD_OUT_WIDTH>* output1,
//     ap_int<8>* weight,
//     ConvDesc_t conv_desc);


#endif