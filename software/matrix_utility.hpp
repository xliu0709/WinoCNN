#ifndef _MATRIX_MULT_HPP
#define _MATRIX_MULT_HPP

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

template<class T>
void matrix_transpose(const T* input, T* output, int row_num, int col_num)
{
    for(int row_idx = 0;row_idx < row_num;row_idx++)
    for(int col_idx = 0;col_idx < col_num;col_idx++)
    {
        output[col_idx*row_num+row_idx]=input[row_idx*col_num+col_idx];
    }
}



template<class TA, class TB, class TC>
void matrix_mult( 
    TA* matrix_A, int row_num_A, int col_num_A,
    TB* matrix_B, int row_num_B, int col_num_B,
    TC* matrix_C, int row_num_C, int col_num_C)
{
    if(col_num_A != row_num_B)
    {
        std::cerr<<"["<<__FILE__<<":line "<<__LINE__<<"]:"<<"mismatched INPUT dimensions in matrix multiplication"<<std::endl;
        exit(0);
    }
    if(row_num_A != row_num_C || col_num_B != col_num_C)
    {
        std::cerr<<"["<<__FILE__<<":line "<<__LINE__<<"]:"<<"mismatched OUTPUT dimensions in matrix multiplication"<<std::endl;
        exit(0);
    }

    for(int row_idx_C = 0; row_idx_C < row_num_C; row_idx_C++)
    for(int col_idx_C = 0; col_idx_C < col_num_C; col_idx_C++)
    {
        TC temp = (TC) 0;
        for(int dot_idx =0; dot_idx < col_num_A; dot_idx++)
        {
            temp +=  ( matrix_A[row_idx_C*col_num_A+dot_idx])*( matrix_B[dot_idx*col_num_B+col_idx_C]) ;
        }
        matrix_C[row_idx_C*col_num_C+col_idx_C]=temp;
    }
}





#endif