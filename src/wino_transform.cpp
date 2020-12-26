
#ifndef _WINO_TRANSFORM_CPP_
#define _WINO_TRANSFORM_CPP_
#include "wino_macro.h"
#include <ap_int.h>

#define DB6x6_1(in,out,ridx,bidx) \
        out[ridx][0][bidx]=((in[ridx][0][bidx]-in[ridx][2][bidx])*4-in[ridx][2][bidx]+in[ridx][4][bidx])>>DB_QUANT_BIT;\
        out[ridx][1][bidx]=(in[ridx][3][bidx]+in[ridx][4][bidx]-(in[ridx][1][bidx]+in[ridx][2][bidx])*4)>>DB_QUANT_BIT;\
        out[ridx][2][bidx]=((in[ridx][1][bidx]-in[ridx][2][bidx])*4+in[ridx][4][bidx]-in[ridx][3][bidx])>>DB_QUANT_BIT;\
        out[ridx][3][bidx]=((in[ridx][3][bidx]-in[ridx][1][bidx])*2+in[ridx][4][bidx]-in[ridx][2][bidx])>>DB_QUANT_BIT;\
        out[ridx][4][bidx]=((in[ridx][1][bidx]-in[ridx][3][bidx])*2+in[ridx][4][bidx]-in[ridx][2][bidx])>>DB_QUANT_BIT;\
        out[ridx][5][bidx]=((in[ridx][1][bidx]-in[ridx][3][bidx])*4+in[ridx][5][bidx]-in[ridx][3][bidx])>>DB_QUANT_BIT;
    
#define BTB6x6_1(in,out,cidx,bidx) \
        out[0][cidx][bidx]=((in[0][cidx][bidx]-in[2][cidx][bidx])*4-in[2][cidx][bidx]+in[4][cidx][bidx])>>BTB_QUANT_BIT;\
        out[1][cidx][bidx]=(in[3][cidx][bidx]+in[4][cidx][bidx]-(in[1][cidx][bidx]+in[2][cidx][bidx])*4)>>BTB_QUANT_BIT;\
        out[2][cidx][bidx]=((in[1][cidx][bidx]-in[2][cidx][bidx])*4+in[4][cidx][bidx]-in[3][cidx][bidx])>>BTB_QUANT_BIT;\
        out[3][cidx][bidx]=((in[3][cidx][bidx]-in[1][cidx][bidx])*2+in[4][cidx][bidx]-in[2][cidx][bidx])>>BTB_QUANT_BIT;\
        out[4][cidx][bidx]=((in[1][cidx][bidx]-in[3][cidx][bidx])*2+in[4][cidx][bidx]-in[2][cidx][bidx])>>BTB_QUANT_BIT;\
        out[5][cidx][bidx]=((in[1][cidx][bidx]-in[3][cidx][bidx])*4+in[5][cidx][bidx]-in[3][cidx][bidx])>>BTB_QUANT_BIT;



#define DB4x4_1(in,out,ridx,bidx) \
        out[ridx][0][bidx]=(in[ridx][0][bidx]-in[ridx][2][bidx])>>DB_QUANT_BIT;\
        out[ridx][1][bidx]=(in[ridx][1][bidx]+in[ridx][2][bidx])>>DB_QUANT_BIT;\
        out[ridx][2][bidx]=(in[ridx][2][bidx]-in[ridx][1][bidx])>>DB_QUANT_BIT;\
        out[ridx][3][bidx]=(in[ridx][1][bidx]-in[ridx][3][bidx])>>DB_QUANT_BIT;

#define BTB4x4_1(in,out,cidx,bidx) \
        out[0][cidx][bidx]=(in[0][cidx][bidx]-in[2][cidx][bidx])>>BTB_QUANT_BIT;\
        out[1][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx])>>BTB_QUANT_BIT;\
        out[2][cidx][bidx]=(in[2][cidx][bidx]-in[1][cidx][bidx])>>BTB_QUANT_BIT;\
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[3][cidx][bidx])>>BTB_QUANT_BIT;


// G = [6,0,0, 
//     -4, -4, -4, 
//     -4, 4, -4,
//     1, 2, 4,
//     1, -2, 4,
//     0, 0, 1] 

#define GGGT_QUANT_BIT 7

#define gGT3to6(in,out,cidx,bidx) \
        out[bidx][0][cidx]=in[bidx][0][cidx]*3*2;\
        out[bidx][1][cidx]=-(in[bidx][0][cidx]+in[bidx][1][cidx]+in[bidx][2][cidx])*4;\
        out[bidx][2][cidx]=(-in[bidx][0][cidx]+in[bidx][1][cidx]-in[bidx][2][cidx])*4;\
        out[bidx][3][cidx]=in[bidx][0][cidx]+in[bidx][1][cidx]*2+in[bidx][2][cidx]*4;\
        out[bidx][4][cidx]=in[bidx][0][cidx]-in[bidx][1][cidx]*2+in[bidx][2][cidx]*4;\
        out[bidx][5][cidx]=in[bidx][2][cidx]*24;

#define GgG3to6(in,out,ridx,bidx) \
        out[bidx][ridx][0]=in[bidx][ridx][0]*3*2;\
        out[bidx][ridx][1]=-(in[bidx][ridx][0]+in[bidx][ridx][1]+in[bidx][ridx][2])*4;\
        out[bidx][ridx][2]=(-in[bidx][ridx][0]+in[bidx][ridx][1]-in[bidx][ridx][2])*4;\
        out[bidx][ridx][3]=(in[bidx][ridx][0]+in[bidx][ridx][1]*2+in[bidx][ridx][2]*4);\
        out[bidx][ridx][4]=(in[bidx][ridx][0]-in[bidx][ridx][1]*2+in[bidx][ridx][2]*4);\
        out[bidx][ridx][5]=(in[bidx][ridx][2]*24);


#define gGT3to4(in,out,cidx,bidx) \
        out[bidx][0][cidx]=in[bidx][0][cidx]*2;\
        out[bidx][1][cidx]=in[bidx][0][cidx]+in[bidx][1][cidx]+in[bidx][2][cidx];\
        out[bidx][2][cidx]=in[bidx][0][cidx]-in[bidx][1][cidx]+in[bidx][2][cidx];\
        out[bidx][3][cidx]=in[bidx][2][cidx]*2;\



#define GgG3to4(in,out,ridx,bidx) \
        out[bidx][ridx][0]=in[bidx][ridx][0]*2;\
        out[bidx][ridx][1]=in[bidx][ridx][0]+in[bidx][ridx][1]+in[bidx][ridx][2];\
        out[bidx][ridx][2]=in[bidx][ridx][0]-in[bidx][ridx][1]+in[bidx][ridx][2];\
        out[bidx][ridx][3]=in[bidx][ridx][2]*2;\
        




void UVA_row( 
    ap_int<UVA_WIDTH> out[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE],
    ap_int<UV_WIDTH> in[WINO_DOMAIN_SIZE][WINO_DOMAIN_SIZE][BATCH_SIZE], 
    int ridx, 
    int bidx, 
    ap_uint<1> flag)
{
    #pragma HLS inline

    #if WINO_DOMAIN_SIZE == 4
        out[ridx][0][bidx]=(in[ridx][0][bidx]+in[ridx][1][bidx]+in[ridx][2][bidx])>>UVA_QUANT_BIT;
        out[ridx][1][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]-flag*in[ridx][3][bidx])>>UVA_QUANT_BIT;
        out[ridx][2][bidx]=(in[ridx][1][bidx]+in[ridx][2][bidx])>>UVA_QUANT_BIT;
        out[ridx][3][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+in[ridx][3][bidx])>>UVA_QUANT_BIT;
    
    #elif WINO3x3_EN && WINO5x5_EN
        out[ridx][0][bidx]=(in[ridx][0][bidx]+in[ridx][1][bidx]+in[ridx][2][bidx]+in[ridx][3][bidx]+in[ridx][4][bidx])>>UVA_QUANT_BIT;
        out[ridx][1][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+(in[ridx][3][bidx]-in[ridx][4][bidx])*2+in[ridx][5][bidx]*flag)>>UVA_QUANT_BIT;
        out[ridx][2][bidx]=(in[ridx][1][bidx]+in[ridx][2][bidx]+(in[ridx][3][bidx]+in[ridx][4][bidx])*4)>>UVA_QUANT_BIT;
        out[ridx][3][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+(in[ridx][3][bidx]-in[ridx][4][bidx])*8+in[ridx][5][bidx])>>UVA_QUANT_BIT;
    #elif WINO3x3_EN
        out[ridx][0][bidx]=(in[ridx][0][bidx]+in[ridx][1][bidx]+in[ridx][2][bidx]+in[ridx][3][bidx]+in[ridx][4][bidx])>>UVA_QUANT_BIT;
        out[ridx][1][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+(in[ridx][3][bidx]-in[ridx][4][bidx])*2)>>UVA_QUANT_BIT;
        out[ridx][2][bidx]=(in[ridx][1][bidx]+in[ridx][2][bidx]+(in[ridx][3][bidx]+in[ridx][4][bidx])*4)>>UVA_QUANT_BIT;
        out[ridx][3][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+(in[ridx][3][bidx]-in[ridx][4][bidx])*8+in[ridx][5][bidx])>>UVA_QUANT_BIT;
    #else
        out[ridx][0][bidx]=(in[ridx][0][bidx]+in[ridx][1][bidx]+in[ridx][2][bidx]+in[ridx][3][bidx]+in[ridx][4][bidx])>>UVA_QUANT_BIT;
        out[ridx][1][bidx]=(in[ridx][1][bidx]-in[ridx][2][bidx]+(in[ridx][3][bidx]-in[ridx][4][bidx])*2+in[ridx][4][bidx])>>UVA_QUANT_BIT;
    #endif

}




void ATA_col( 
    ap_int<ATA_WIDTH> out[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE], 
    ap_int<UVA_WIDTH> in[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE],
    int cidx, 
    int bidx, 
    ap_uint<1> flag)
{


    #pragma HLS inline

    #if WINO_DOMAIN_SIZE == 4
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]-flag*in[3][cidx][bidx])>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx])>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+in[3][cidx][bidx])>>ATA_QUANT_BIT;

    #elif WINO3x3_EN && WINO5x5_EN
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2+in[5][cidx][bidx]*flag)>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx]+(in[3][cidx][bidx]+in[4][cidx][bidx])*4)>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*8+in[5][cidx][bidx])>>ATA_QUANT_BIT;
    #elif WINO3x3_EN
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2)>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx]+(in[3][cidx][bidx]+in[4][cidx][bidx])*4)>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*8+in[5][cidx][bidx])>>ATA_QUANT_BIT;
    #else
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2+in[4][cidx][bidx])>>ATA_QUANT_BIT;
    #endif

}


void ATA_col2(
    ap_int<ATA_WIDTH> out[WINO_OUT_SIZE_CELL][WINO_OUT_SIZE_CELL][BATCH_SIZE],
    ap_int<UVA_WIDTH> in[WINO_DOMAIN_SIZE][WINO_OUT_SIZE_CELL][BATCH_SIZE],
    int cidx,
    int bidx,
    ap_uint<1> flag)
{
    #pragma HLS inline

#pragma HLS array_partition variable=out dim=1 complete
#pragma HLS array_partition variable=out dim=2 complete
#pragma HLS array_partition variable=out dim=3 complete
#pragma HLS array_partition variable=in dim=1 complete
#pragma HLS array_partition variable=in dim=2 complete
#pragma HLS array_partition variable=in dim=3 complete
    #if WINO_DOMAIN_SIZE == 4
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]-flag*in[3][cidx][bidx])>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx])>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+in[3][cidx][bidx])>>ATA_QUANT_BIT;
    #elif WINO3x3_EN && WINO5x5_EN
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2+in[5][cidx][bidx]*flag)>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx]+(in[3][cidx][bidx]+in[4][cidx][bidx])*4)>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*8+in[5][cidx][bidx])>>ATA_QUANT_BIT;
    #elif WINO3x3_EN
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2)>>ATA_QUANT_BIT;
        out[2][cidx][bidx]=(in[1][cidx][bidx]+in[2][cidx][bidx]+(in[3][cidx][bidx]+in[4][cidx][bidx])*4)>>ATA_QUANT_BIT;
        out[3][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*8+in[5][cidx][bidx])>>ATA_QUANT_BIT;
    #else
        out[0][cidx][bidx]=(in[0][cidx][bidx]+in[1][cidx][bidx]+in[2][cidx][bidx]+in[3][cidx][bidx]+in[4][cidx][bidx])>>ATA_QUANT_BIT;
        out[1][cidx][bidx]=(in[1][cidx][bidx]-in[2][cidx][bidx]+(in[3][cidx][bidx]-in[4][cidx][bidx])*2+in[4][cidx][bidx])>>ATA_QUANT_BIT;
    #endif
}






#endif
