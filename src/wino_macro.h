#ifndef _WINO_MACRO_H_
#define _WINO_MACRO_H_

#define DEBUG_FILE_PRINT 0
#define DEBUG_CONV_DESC 0
#define DEBUG_SIM 0

#include "wino_hw_config.h"


#define NEEDS_BIT(N, B)     (N > (1<<B) ? 1:0 )

#define LOG2CEIL(N)                            \
        (NEEDS_BIT(N,  0) + NEEDS_BIT(N,  1) + \
         NEEDS_BIT(N,  2) + NEEDS_BIT(N,  3) + \
         NEEDS_BIT(N,  4) + NEEDS_BIT(N,  5) + \
         NEEDS_BIT(N,  6) + NEEDS_BIT(N,  7) + \
         NEEDS_BIT(N,  8) + NEEDS_BIT(N,  9) + \
         NEEDS_BIT(N, 10) + NEEDS_BIT(N, 11) + \
         NEEDS_BIT(N, 12) + NEEDS_BIT(N, 13) + \
         NEEDS_BIT(N, 14) + NEEDS_BIT(N, 15) \
        /* NEEDS_BIT(N, 16) + NEEDS_BIT(N, 17) + \
         NEEDS_BIT(N, 18) + NEEDS_BIT(N, 19) + \
         NEEDS_BIT(N, 20) + NEEDS_BIT(N, 21) + \
         NEEDS_BIT(N, 22) + NEEDS_BIT(N, 23) + \
         NEEDS_BIT(N, 24) + NEEDS_BIT(N, 25) + \
         NEEDS_BIT(N, 26) + NEEDS_BIT(N, 27) + \
         NEEDS_BIT(N, 28) + NEEDS_BIT(N, 29) + \
         NEEDS_BIT(N, 30) + NEEDS_BIT(N, 31)  */ \
        )


#define CEIL_DIV(x,y)  ( ( (x) + (y) - 1) / (y) )
#define ALIGN(x,y)  ( ( (x) + (y) - 1) / (y) * (y) )


#define AP_INT_MAX_W 4096
#define BATCH_SIZE 2

#define WINO5x5_EN 0
#define WINO3x3_EN 1






#define WINO_DOMAIN_SIZE_SQUARE (WINO_DOMAIN_SIZE*WINO_DOMAIN_SIZE)


#if WINO_DOMAIN_SIZE == 4
#define WINO_OUT_SIZE 2
#define WINO_OUT_SIZE_CELL 4

#elif WINO3x3_EN

#define WINO_OUT_SIZE 4
#define WINO_OUT_SIZE_CELL 4
#else

#define WINO_OUT_SIZE 2

#endif

#define WINO_OUT_SIZE_SQUARE (WINO_OUT_SIZE * WINO_OUT_SIZE)



#define INPUT_BUFFER_DEPTH (1<<INPUT_BUFFER_DEPTH_BITWIDTH)


#define OUTPUT_BUFFER_DEPTH (1<<OUTPUT_BUFFER_DEPTH_BITWIDTH)

#define WEIGHT_BUFFER_DEPTH (1<<WEIGHT_BUFFER_DEPTH_BITWIDTH)




#define WINO_HEIGHT_BITWIDTH LOG2CEIL(WINO_HEIGHT)


#define WINO_WIDTH_BITWIDTH LOG2CEIL(WINO_WIDTH)


#define INBUFFER_WIDTH_BITWIDTH LOG2CEIL(2+WINO_OUT_SIZE_CELL*WINO_WIDTH)
#define INBUFFER_WIDTH (1<<INBUFFER_WIDTH_BITWIDTH)


#define INBUFFER_HEIGHT (1<<INBUFFER_HEIGHT_BITWIDTH)



#define INDEPTH_MINITILE_SIZE (1<<INDEPTH_MINITILE_SIZE_BITWIDTH)
#define OUTDEPTH_MINITILE_SIZE_BITWIDTH WINO_HEIGHT_BITWIDTH
#define OUTDEPTH_MINITILE_SIZE (1<<OUTDEPTH_MINITILE_SIZE_BITWIDTH)

#if WINO_HEIGHT >4
#define WEIGHT_FEED_NUMBER_PER_PORT (WINO_HEIGHT/4)
#define WEIGHT_FEED_NUMBER_PER_PORT_BITWIDTH (WINO_HEIGHT_BITWIDTH-2)
#else 
#define WEIGHT_FEED_NUMBER_PER_PORT 2
#define WEIGHT_FEED_NUMBER_PER_PORT_BITWIDTH 1
#endif


#define INBUFFER_MID_ADDR_BITWIDTH (INPUT_BUFFER_DEPTH_BITWIDTH-1-INDEPTH_MINITILE_SIZE_BITWIDTH)

#define WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP  CEIL_DIV(WINO_DOMAIN_SIZE_SQUARE*INDEPTH_MINITILE_SIZE,16)
#define WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP_BITWIDTH LOG2CEIL(WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP)





#define BIAS_BUFFER_NUMBER 4
#define BIAS_BUFFER_DEPTH 1024

#if WINO_DOMAIN_SIZE==6
#define IN_TRANSBIT 4
#define OUT_TRANSBIT 4
#define G_TRANSBIT 5
#define G_TRANSFACTOR 24
#else
#define IN_TRANSBIT 1
#define OUT_TRANSBIT 2
#define G_TRANSBIT 2
#define G_TRANSFACTOR 2
#endif

#define G_WIDTH 7
#define GL_WIDTH (G_WIDTH+G_TRANSBIT)
#define RG_WIDTH  (GL_WIDTH+G_TRANSBIT)


#if WINO_DOMAIN_SIZE == 6
#define IN_WIDTH 8
#define W_WIDTH 16
#else
#define IN_WIDTH 8
#define W_WIDTH 11
#endif

// #define FC_WIDTH 11



#define DB_QUANT_BIT 0
#define DB_WIDTH (IN_WIDTH+IN_TRANSBIT-DB_QUANT_BIT)

#define BTB_QUANT_BIT 0
#define BTB_WIDTH (DB_WIDTH+IN_TRANSBIT-BTB_QUANT_BIT)

#define UV_QUANT_BIT 0
#define UV_MUL_WIDTH (BTB_WIDTH+W_WIDTH+1)
#define UV_WIDTH (BTB_WIDTH+W_WIDTH+INDEPTH_MINITILE_SIZE_BITWIDTH-UV_QUANT_BIT)

#define UVA_QUANT_BIT 0
#define UVA_WIDTH (UV_WIDTH+OUT_TRANSBIT-UVA_QUANT_BIT)

#define ATA_QUANT_BIT 0
#define ATA_WIDTH (UVA_WIDTH+OUT_TRANSBIT-ATA_QUANT_BIT)




#define OUT_WIDTH 18
#define OUT_BUFFER_QUANT_BIT 0

#define OBACK_QUANT_BIT 14

#if DEBUG_FILE_PRINT
#define OUT_SAT_MAX ( (1L<<(OUT_WIDTH-1)) - 1 )
#define OUT_SAT_MIN ( -(1L<<(OUT_WIDTH-1)))
#else
#define OUT_SAT_MAX ( (1<<(OUT_WIDTH-1)) - 1 )
#define OUT_SAT_MIN ( -(1<<(OUT_WIDTH-1)))
#endif

#define ODDR_WIDTH IN_WIDTH

#if WINO_HEIGHT==2
#define OUT_PORT_BATCH_NUM 4
#else 
#define OUT_PORT_BATCH_NUM 8 
#endif


#if 0
#include "../testbench/debug.hpp"
#endif




#define FEATURE_PORT_DEPTH 524288
#define WEIGHT_PORT_DEPTH 1179648
#define BIAS_PORT_DEPTH 4096



#if INBUFFER_WIDTH <=4
#define INFM_WORD_WIDTH 64
#define INFM_WORD_BYTES 4
#else
#define INFM_WORD_WIDTH 128
#define INFM_WORD_BYTES 8
#endif

#define INPUT_PORTS_DECLARE(x)  ap_uint<128>* x##0,\
                                ap_uint<128>* x##1,\
                                ap_uint<128>* x##2,\
                                ap_uint<128>* x##3

#define INPUT_PORTS_CALL(x)     x##0,\
                                x##1,\
                                x##2,\
                                x##3



#if WINO_HEIGHT >4 
#define WEIGHT_PORT_NUM 4
#define WEIGHT_PORTS_DECLARE(x)  ap_uint<128>* x##0,\
                                ap_uint<128>* x##1,\
                                ap_uint<128>* x##2,\
                                ap_uint<128>* x##3

#define WEIGHT_PORTS_CALL(x)     x##0,\
                                x##1,\
                                x##2,\
                                x##3

#elif WINO_HEIGHT ==4
#define WEIGHT_PORT_NUM 2
#define WEIGHT_PORTS_DECLARE(x)  ap_uint<128>* x##0,\
                                ap_uint<128>* x##1

#define WEIGHT_PORTS_CALL(x)     x##0,\
                                x##1

#elif WINO_HEIGHT ==2
#define WEIGHT_PORT_NUM 1
#define WEIGHT_PORTS_DECLARE(x)  ap_uint<128>* x##0
#define WEIGHT_PORTS_CALL(x)     x##0
#endif



#ifdef __SDSCC__
#include "sds_lib.h"
#define MALLOC(x) (sds_alloc_non_cacheable(x))
#define FREE(x) (sds_free(x))
#else 
// #define MALLOC(x) (malloc(x))
// #define FREE(x) (free(x))
#define MALLOC(x) (new char[x])
#define FREE(x) (delete [] x)

#endif



#endif
