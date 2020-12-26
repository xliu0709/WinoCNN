
#include <dsp_builtins.h>
#include <etc/dsp_dp_builtins.h>
#include "../software/matrix_utility.hpp"
#include <ap_int.h>
#include <cstdlib>

#ifndef _WINO_CELL_GOLD_HPP_
#define _WINO_CELL_GOLD_HPP_




template<int dummy>
void input_transform_tile36_float( float d[36], float BTdB[36])
{
    float BT[36]= {   4,  0, -5,  0,  1,  0, 
                    0, -4, -4,  1,  1,  0,
                    0,  4, -4, -1,  1,  0,
                    0, -2, -1,  2,  1,  0,
                    0,  2, -1, -2,  1,  0,
                    0,  4,  0, -5,  0,  1};

    float* dB = new float[36];
    float* B = new float[36];
    matrix_transpose<float> (BT,B,6,6);
    matrix_mult<float, float, float >(d,6,6,B,6,6,dB,6,6);
    matrix_mult<float, float, float >(BT,6,6,dB,6,6,BTdB,6,6);
    delete[] dB;
    delete[] B;
}

template<int dummy>
void input_transform_tile16_float( float d[16], float BTdB[16])
{
    float BT[16]= {   1,  0, -1,  0, 
                    0,  1,  1,  0,
                    0, -1,  1,  0,
                    0,  1,  0, -1};

    float* dB = new float[16];
    float* B = new float[16];
    matrix_transpose<float> (BT,B,4,4);
    matrix_mult<float, float, float >(d,4,4,B,4,4,dB,4,4);
    matrix_mult<float, float, float >(BT,4,4,dB,4,4,BTdB,4,4);
    delete[] dB;
    delete[] B;
}


template<int dummy>
void output_transform_tile16_float( float  V[16], float y[4])
{


    float AT3x3[8] ={ 1,  1,  1,  0,
                    0,  1, -1, -1};
    float ATV[8];
    matrix_mult<float,float,float>(AT3x3,2,4,V,4,4,ATV,2,4);
    float A[8];
    matrix_transpose<float>(AT3x3,A,2,4);
    matrix_mult<float,float,float>(ATV,2,4,A,4,2,y,2,2);
}


template<int dummy>
void output_transform_tile36_ker3x3_float( float V[36], float y[16])
{


    float AT3x3[24] = {   1,1,1,1,1,0,
                    0,1,-1,2,-2,0,
                    0,1,1,4,4,0,
                    0,1,-1,8,-8,1};

    float ATV[24];
    matrix_mult<float,float,float>(AT3x3,4,6,V,6,6,ATV,4,6);
    float A[24];
    matrix_transpose<float>(AT3x3,A,4,6);
    matrix_mult<float,float,float>(ATV,4,6,A,6,4,y,4,4);
}

template<int dummy>
void output_transform_tile36_ker5x5_float( float V[36], float y[16])
{


    float AT5x5[24] = {1,1,1,1,1,0,
                        0,1,-1,2,-2,1,
                        0,1,1,4,4,0,
                        0,1,-1,8,-8,1};

    float ATV[24];
    matrix_mult<float,float,float>(AT5x5,4,6,V,6,6,ATV,4,6);
    float A[24];
    matrix_transpose<float>(AT5x5,A,4,6);
    matrix_mult<float,float,float>(ATV,4,6,A,6,4,y,4,4);
}





template<class T>
void input_right_mul_36( T d[36], T dB[36])
{
    T B[36]= {  4,	0,	0,	0,	0,	0,
                0,	-4,	4,	-2,	2,	4,
                -5,	-4,	-4,	-1,	-1,	0,
                0,	1,	-1,	2,	-2,	-5,
                1,	1,	1,	1,	1,	0,
                0,	0,	0,	0,	0,	1};
    matrix_mult<T, T, T >(d,6,6,B,6,6,dB,6,6);
}

template<class T>
void input_left_mul_36( T dB[36], T BTdB[36])
{
    T BT[36]= {   4,  0, -5,  0,  1,  0, 
                    0, -4, -4,  1,  1,  0,
                    0,  4, -4, -1,  1,  0,
                    0, -2, -1,  2,  1,  0,
                    0,  2, -1, -2,  1,  0,
                    0,  4,  0, -5,  0,  1};

    matrix_mult<T, T, T >(BT,6,6,dB,6,6,BTdB,6,6);
}


template<class T>
void input_right_mul_16( T d[16], T dB[16])
{
    T B[16]= {  1,	0,	0,	0, 
                0,	1,	-1,	1,
                -1,	1,	1,	0,
                0,	0,	0,	-1};
    matrix_mult<T, T, T >(d,4,4,B,4,4,dB,4,4);
}



template<class T>
void input_left_mul_16( T dB[16], T BTdB[16])
{
    T BT[16]= { 1,  0, -1,  0, 
                0,  1,  1,  0,
                0, -1,  1,  0,
                0,  1,  0, -1};

    matrix_mult<T, T, T >(BT,4,4,dB,4,4,BTdB,4,4);
}


template<class T>
void weight_right_mul_3to6(T g[9], T gGT[18])
{       
    T GT[18]={  6,	-4,	-4,	1,	1,	0,
                0,	-4,	4,	2,	-2,	0,
                0,	-4,	-4,	4,	4,	24};
    matrix_mult<T, T, T>(g,3,3,GT,3,6,gGT,3,6);
}

template<class T>
void weight_left_mul_3to6(T gGT[18], T GgGT[36])
{       
    T G[18]={   6,  0,  0,
                -4, -4, -4,
                -4, 4,  -4,
                1,  2,  4,
                1,  -2, 4,
                0,  0,  24};

    matrix_mult<T, T, T>(G,6,3,gGT,3,6,GgGT,6,6);
}

template<class T>
void weight_right_mul_3to4(T g[9], T gGT[12])
{       
    T GT[12]={  2,	1,	1,	0,
                0,	1,	-1,	0,
                0,	1,	1,	2};

    matrix_mult<T, T, T>(g,3,3,GT,3,4,gGT,3,4);
}

template<class T>
void weight_left_mul_3to4(T gGT[12], T GgGT[16])
{       
    T G[12]={   2,  0,  0,
                1,  1,  1,
                1,  -1,  1,
                0,  0,  2};

    matrix_mult<T, T, T>(G,4,3,gGT,3,4,GgGT,4,4);
}








template<class T>
void output_right_mul_6to4(T v[36], T vA[24])
{
    T A[24] = {    1,	0,  0,	0,
                    1,	1,  1,	1,
                    1,	-1, 1,	-1,
                    1,	2,  4,	8,
                    1,	-2, 4,	-8,
                    0,	0,  0,	1};

    matrix_mult<T, T, T>(v,6,6,A,6,4,vA,6,4); 
}

template<class T>
void output_left_mul_6to4(T vA[24], T ATvA[16])
{
    T AT[24] = {    1,  1,  1,  1,  1,  0,
                    0,  1,  -1, 2,  -2, 0,
                    0,  1,  1,  4,  4,  0,
                    0,  1,  -1, 8,  -8, 1};
    matrix_mult<T, T, T>(AT,4,6,vA,6,4,ATvA,4,4); 
}

template<class T>
void output_right_mul_6to2(T v[36], T vA[24])
{
    T A[24] = {    1,	0,  0,	0,
                    1,	1,  1,	1,
                    1,	-1, 1,	-1,
                    1,	2,  4,	8,
                    1,	-2, 4,	-8,
                    0,	1,  0,	1};

    matrix_mult<T, T, T>(v,6,6,A,6,4,vA,6,4); 
}


template<class T>
void output_left_mul_6to2(T vA[24], T ATvA[16])
{
    T AT[24] = {    1,  1,  1,  1,  1,  0,
                    0,  1,  -1, 2,  -2, 1,
                    0,  1,  1,  4,  4,  0,
                    0,  1,  -1, 8,  -8, 1};
    matrix_mult<T, T, T>(AT,4,6,vA,6,4,ATvA,4,4); 
}


template<class T>
void output_right_mul_4to2(T v[16], T vA[8])
{
    T A[8] = { 1,	0, 
                1,	1,
                1,	-1,
                0,	-1};


    matrix_mult<T, T, T>(v,4,4,A,4,2,vA,4,2); 
}



template<class T>
void output_left_mul_4to2(T vA[8], T ATvA[4])
{
    T AT[8] = {    1,  1,  1,  0,
                    0,  1,  -1, -1};
    matrix_mult<T, T, T>(AT,2,4,vA,4,2,ATvA,2,2); 
}









// template<int dummy>
// void weight_transform_5to6_float(float V[25], float y[36])
// {

// }

// template<int dummy>
// void weight_transform_3to6_float(float V[9], float y[36])
// {
//     void 
// }


// template<int dummy>
// void weight_transform_3to4_float(float V[9], float y[36])
// {
    
// }



template<int INPUT_WIDTH>
void input_transform_tile36( ap_int<INPUT_WIDTH> d[36], ap_int<INPUT_WIDTH+8> BTdB[36])
{
    int BT[36]= {   4,  0, -5,  0,  1,  0, 
                    0, -4, -4,  1,  1,  0,
                    0,  4, -4, -1,  1,  0,
                    0, -2, -1,  2,  1,  0,
                    0,  2, -1, -2,  1,  0,
                    0,  4,  0, -5,  0,  1};

    ap_int<INPUT_WIDTH+4>* dB = new ap_int<INPUT_WIDTH+4>[36];
    int* B = new int[36];
    matrix_transpose<int> (BT,B,6,6);
    matrix_mult<ap_int<INPUT_WIDTH>, int, ap_int<INPUT_WIDTH+4> >(d,6,6,B,6,6,dB,6,6);
    matrix_mult<int, ap_int<INPUT_WIDTH+4>, ap_int<INPUT_WIDTH+8> >(BT,6,6,dB,6,6,BTdB,6,6);
    delete[] dB;
    delete[] B;
}



template<int INPUT_WIDTH>
void input_transform_tile16( ap_int<INPUT_WIDTH> d[16], ap_int<INPUT_WIDTH+2> BTdB[16])
{
    int BT[16]= {   1,  0, -1,  0, 
                    0,  1,  1,  0,
                    0, -1,  1,  0,
                    0,  1,  0, -1};

    ap_int<INPUT_WIDTH+1>* dB = new ap_int<INPUT_WIDTH+1>[16];
    int* B = new int[16];
    matrix_transpose<int> (BT,B,4,4);
    matrix_mult<ap_int<INPUT_WIDTH>, int, ap_int<INPUT_WIDTH+1> >(d,4,4,B,4,4,dB,4,4);
    matrix_mult<int, ap_int<INPUT_WIDTH+1>, ap_int<INPUT_WIDTH+2> >(BT,4,4,dB,4,4,BTdB,4,4);
    delete[] dB;
    delete[] B;
}



template<int OUTPUT_WIDTH>
void output_transform_tile16( ap_int<OUTPUT_WIDTH-4> V[16], ap_int<OUTPUT_WIDTH> y[4])
{


    int AT3x3[8] ={ 1,  1,  1,  0,
                    0,  1, -1, -1};


    int  Vint[16];
    for(int i=0;i<16;i++)
    {
        Vint[i]=V[i];
    }

    int ATV[8];
    matrix_mult<int,int,int>(AT3x3,2,4,Vint,4,4,ATV,2,4);
    int A[8];
    matrix_transpose<int>(AT3x3,A,2,4);
    int yint[8];
    matrix_mult<int,int,int>(ATV,2,4,A,4,2,yint,2,2);

    for(int i=0;i<4;i++)
    {
        y[i]=yint[i];
    }
}

template<int INPUT_WIDTH,  int WEIGHT_WIDTH, int OUTPUT_WIDTH>
void wino_gold_tile16(ap_int<INPUT_WIDTH> d[16], ap_int<WEIGHT_WIDTH> g[16], ap_int<OUTPUT_WIDTH> y[4])
{

    ap_int<INPUT_WIDTH+2>* BTdB = new ap_int<INPUT_WIDTH+2>[16];
    input_transform_tile16<INPUT_WIDTH>(d,BTdB);
    ap_int<OUTPUT_WIDTH-4>* V = new ap_int<OUTPUT_WIDTH-4>[16];
    for(int i=0;i<16;i++)
    {
        V[i] =  BTdB[i] *g[i];
    }
    output_transform_tile16<OUTPUT_WIDTH>(V,y);
    delete[] BTdB;
    delete[] V;
}





template<int OUTPUT_WIDTH>
void output_transform_tile36_ker3x3( ap_int<OUTPUT_WIDTH-8> V[36], ap_int<OUTPUT_WIDTH> y[16])
{


int AT3x3[24] = {   1,1,1,1,1,0,
                    0,1,-1,2,-2,0,
                    0,1,1,4,4,0,
                    0,1,-1,8,-8,1};

    int  Vint[36];
    for(int i=0;i<36;i++)
    {
        Vint[i]=V[i];
    }

    int ATV[24];

    matrix_mult<int,int,int>(AT3x3,4,6,Vint,6,6,ATV,4,6);
    int A[24];


    int yint[24];
    matrix_transpose<int>(AT3x3,A,4,6);
    matrix_mult<int,int,int>(ATV,4,6,A,6,4,yint,4,4);

    for(int i=0;i<16;i++)
    {
        y[i]=yint[i];
    }

}

template<int OUTPUT_WIDTH>
void output_transform_tile36_ker5x5_hw( ap_int<OUTPUT_WIDTH-8> V[36], ap_int<OUTPUT_WIDTH> y[16])
{

int AT3x3[24] = { 1,1,1,1,1,0,
                        0,1,-1,2,-2,1,
                        0,1,1,4,4,0,
                        0,1,-1,8,-8,1};

    int  Vint[36];
    for(int i=0;i<36;i++)
    {
        Vint[i]=V[i];
    }

    int ATV[24];

    matrix_mult<int,int,int>(AT3x3,4,6,Vint,6,6,ATV,4,6);
    int A[24];


    int yint[24];
    matrix_transpose<int>(AT3x3,A,4,6);
    matrix_mult<int,int,int>(ATV,4,6,A,6,4,yint,4,4);

    for(int i=0;i<16;i++)
    {
        y[i]=yint[i];
    }
}

template<int OUTPUT_WIDTH>
void output_transform_ker5x5_int( ap_int<OUTPUT_WIDTH-8> V[36], ap_int<OUTPUT_WIDTH> y[16])
{

int AT5x5[24] = { 1,1,1,1,1,0,
                        0,1,-1,2,-2,1};

    ap_int<OUTPUT_WIDTH-4>* ATV = new ap_int<OUTPUT_WIDTH-4>[12];
    matrix_mult< int, ap_int<OUTPUT_WIDTH-8>,ap_int<OUTPUT_WIDTH-4> >(AT5x5,2,6,V,6,6,ATV,2,6);
    int* A = new int[12];
    matrix_transpose<int>(AT5x5,A,2,6);
    matrix_mult<ap_int<OUTPUT_WIDTH-4>,int,ap_int<OUTPUT_WIDTH> >(ATV,2,6,A,6,2,y,2,2);
    delete[] ATV;
    delete[] A;
}


template<int INPUT_WIDTH,  int WEIGHT_WIDTH, int OUTPUT_WIDTH>
void wino_gold_tile36_3x3(ap_int<INPUT_WIDTH> d[36], ap_int<WEIGHT_WIDTH> g[36], ap_int<OUTPUT_WIDTH> y[16])
{

    ap_int<INPUT_WIDTH+8>* BTdB = new ap_int<INPUT_WIDTH+8>[36];
    input_transform_tile36<INPUT_WIDTH>(d,BTdB);
    ap_int<OUTPUT_WIDTH-8>* V = new ap_int<OUTPUT_WIDTH-8>[36];
    for(int i=0;i<36;i++)
    {
        V[i] =  BTdB[i] *g[i];
    }
    output_transform_tile36_ker3x3<OUTPUT_WIDTH>(V,y);
    delete[] BTdB;
    delete[] V;
}

template<int INPUT_WIDTH,  int WEIGHT_WIDTH, int OUTPUT_WIDTH>
void wino_gold_tile36_5x5(ap_int<INPUT_WIDTH> d[36], ap_int<WEIGHT_WIDTH> g[36], ap_int<OUTPUT_WIDTH> y[16])
{

    ap_int<INPUT_WIDTH+8>* BTdB = new ap_int<INPUT_WIDTH+8>[36];
    input_transform_tile36<INPUT_WIDTH>(d,BTdB);
    ap_int<OUTPUT_WIDTH-8>* V = new ap_int<OUTPUT_WIDTH-8>[36];
    for(int i=0;i<36;i++)
    {
        V[i] =  BTdB[i] *g[i];
    }
    output_transform_ker5x5_int<OUTPUT_WIDTH>(V,y);
    delete[] BTdB;
    delete[] V;
}


#endif