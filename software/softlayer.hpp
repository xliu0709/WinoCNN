#ifndef __SOFTLAYER_H_
#define __SOFTLAYER_H_
#include <stdlib.h>
#include <cstdio>
#include <iostream>
template<class T>
void max_pooling_soft(
    T* input,
    T* output,
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int depth,
    int pad_size,
    int window_size,
    int stride)
{

    std::cout<<"running maxpool soft layer"<<std::endl;
    for(int depth_idx=0;depth_idx<depth;depth_idx++)
    {
        for(int row=0;row<outheight;row++)
        {
            for(int col=0;col<outwidth;col++)
            {
                T temp= 0;
                int bank_idx=0;
                for(int rowoffset=0;rowoffset<window_size;rowoffset++)
                for(int coloffset=0;coloffset<window_size;coloffset++)
                {
                    int input_row=row*stride-pad_size+rowoffset;
                    int input_col=col*stride-pad_size+coloffset;
                    int inaddress=(depth_idx*inheight+input_row)*inwidth+input_col;
                    if(input_row>=0 && input_row < inheight && input_col >=0 && input_col < inwidth)
                    {
                        temp=input[inaddress]>temp?input[inaddress]:temp;
                    }
                }
                int outaddr= (depth_idx*outheight+row)*outwidth+col;
                output[outaddr]=temp;

            }
        }
    }
}


template<class T>
void avg_pooling_soft(
    T* input,
    T* output,
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int depth,
    int pad_size,
    int window_size,
    int stride)
{

    std::cout<<"running maxpool soft layer"<<std::endl;
    for(int depth_idx=0;depth_idx<depth;depth_idx++)
    {
        for(int row=0;row<outheight;row++)
        {
            for(int col=0;col<outwidth;col++)
            {
                T temp= 0;
                int bank_idx=0;
                for(int rowoffset=0;rowoffset<window_size;rowoffset++)
                for(int coloffset=0;coloffset<window_size;coloffset++)
                {
                    int input_row=row*stride-pad_size+rowoffset;
                    int input_col=col*stride-pad_size+coloffset;
                    int inaddress=(depth_idx*inheight+input_row)*inwidth+input_col;
                    if(input_row>=0 && input_row < inheight && input_col >=0 && input_col < inwidth)
                    {
                        temp+=input[inaddress];
                    }
                }
                temp= ((float) temp)/(window_size*window_size);
                int outaddr= (depth_idx*outheight+row)*outwidth+col;
                output[outaddr]=temp;
            }
        }
    }
}

template<int dummy>
int hw_address(
    int h,
    int w,
    int d,
    int width_align8,
    int depth_align8
 )
 {
    return (h*width_align8*depth_align8+d/8*width_align8*8+w*8+d%8)*2;
 }

template<class T>
void max_pooling_hw(
    T* input,
    T* output,
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int depth,
    int pad_size,
    int window_size,
    int stride)
{
    int inwidth_align8=ALIGN(inwidth,8);
    int depth_align8=ALIGN(depth,8);

    int outwidth_align8=ALIGN(outwidth,8);

    std::cout<<"running maxpool soft layer"<<std::endl;
    for(int depth_idx=0;depth_idx<depth;depth_idx++)
    {
        for(int row=0;row<outheight;row++)
        {
            for(int col=0;col<outwidth;col++)
            {
                T temp0= 0;
                T temp1= 0;
                for(int rowoffset=0;rowoffset<window_size;rowoffset++)
                for(int coloffset=0;coloffset<window_size;coloffset++)
                {
                    int input_row=row*stride-pad_size+rowoffset;
                    int input_col=col*stride-pad_size+coloffset;
                    int inaddress=hw_address<0>(input_row,input_col,depth_idx,inwidth_align8,depth_align8);
                    if(input_row>=0 && input_row < inheight && input_col >=0 && input_col < inwidth)
                    {
                        temp0=input[inaddress]>temp0?input[inaddress]:temp0;
                        temp1=input[inaddress+1]>temp1?input[inaddress+1]:temp1;
                    }
                }
                
                int outaddr=hw_address<0>(row,col,depth_idx,outwidth_align8,depth_align8);
                output[outaddr]=temp0;
                output[outaddr+1]=temp1;
            }
        }
    }
}





#define AVGBACK_QUANT_BIT 18
template<class T>
void avg_pooling_int(
    T* input,
    T* output,
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int depth,
    int pad_size,
    int window_size,
    int stride)
{
    int avg_weight_int = ((float)1/(window_size*window_size))*(1<<AVGBACK_QUANT_BIT);

    std::cout<<"running avgpool int layer"<<std::endl;
    for(int depth_idx=0;depth_idx<depth;depth_idx++)
    {
        for(int row=0;row<outheight;row++)
        {
            for(int col=0;col<outwidth;col++)
            {
                int temp= 0;
                int bank_idx=0;
                for(int rowoffset=0;rowoffset<window_size;rowoffset++)
                for(int coloffset=0;coloffset<window_size;coloffset++)
                {
                    int input_row=row*stride-pad_size+rowoffset;
                    int input_col=col*stride-pad_size+coloffset;
                    int inaddress=(depth_idx*inheight+input_row)*inwidth+input_col;
                    if(input_row>=0 && input_row < inheight && input_col >=0 && input_col < inwidth)
                    {
                        temp+=input[inaddress];
                    }
                }
                int rst=(temp*avg_weight_int)>>AVGBACK_QUANT_BIT;
                int outaddr= (depth_idx*outheight+row)*outwidth+col;

                output[outaddr]=rst;
            }
        }
    }
}


template<class T>
void elt_hw(
    T* input1,
    T* input2,
    T* output,
    int outheight,
    int outwidth,
    int outdepth,
    int scale_oback_int,
    bool relu_flag)
{

    int length= outheight*ALIGN(outwidth,8)*ALIGN(outdepth,8)*2;
    std::cout<<"running residual soft layer"<<std::endl;
    for(int addr=0;addr<length;addr++)
    {
        T temp= 0;

        output[addr]=((int)(input1[addr]+input2[addr])*scale_oback_int)>>OBACK_QUANT_BIT;

        if(relu_flag && output[addr]<0)
            output[addr]=0;
    }
}


template<class T>
void elt_soft(
    T* input1,
    T* input2,
    T* output,
    int outheight,
    int outwidth,
    int outdepth,
    int scale_oback_int,
    bool relu_flag)
{

    std::cout<<"running residual soft layer"<<std::endl;
    for(int depth_idx=0;depth_idx<outdepth;depth_idx++)
    {
        for(int row=0;row<outheight;row++)
        {
            for(int col=0;col<outwidth;col++)
            {
                T temp= 0;
                int bank_idx=0;
                int outaddr= (depth_idx*outheight+row)*outwidth+col;
                output[outaddr]=((int)(input1[outaddr]+input2[outaddr])*scale_oback_int)>>OBACK_QUANT_BIT;

                if(relu_flag && output[outaddr]<0)
                    output[outaddr]=0;
            }
        }
    }
}




template<class T>
void conv_soft(
    T* input,
    T* output,
    T* weight,
    T* bias,
    int indepth,
    int inheight,
    int inwidth,
    int outdepth,
    int outheight,
    int outwidth,
    int pad_size,
    int window_size,
    int stride,
    bool adding_bias,
    bool relu_flag
    )
{
    std::cout<<"running conv soft layer"<<std::endl;
    int outidx=0;
    int weightidx=0;
    for(int od=0;od<outdepth;od++)
    {   
        T b=bias[od];
        for(int oh=0;oh<outheight;oh++)
        for(int ow=0;ow<outwidth;ow++)
        {
            if(adding_bias)
                output[outidx]=b;
            else  
                output[outidx]=0;
            outidx++;
        }
    }

    for(int od=0;od<outdepth;od++)
    for(int id=0;id<indepth;id++)
    {
        for(int kh=0;kh<window_size;kh++)
        for(int kw=0;kw<window_size;kw++)
        {
            T weightval=weight[weightidx];
            for(int oh=0,ih_off=-pad_size+kh;oh<outheight;oh++,ih_off+=stride)
            for(int ow=0,iw_off=-pad_size+kw;ow<outwidth;ow++,iw_off+=stride)
            {
                T inval;
                if(ih_off>=0 && ih_off < inheight && iw_off >=0 && iw_off <inwidth)
                {
                    inval= input[ (id*inheight+ih_off)*inwidth+iw_off];
                }
                else
                {
                    inval=0;
                }
                output[ (od*outheight+oh)*outwidth+ow]+=inval*weightval;
            }
            weightidx++;
        }
        
    }

    outidx=0;
    for(int od=0;od<outdepth;od++)
    {
        for(int oh=0;oh<outheight;oh++)
        for(int ow=0;ow<outwidth;ow++)
        {
            if(relu_flag && output[outidx]<0)
                output[outidx]=0;
            outidx++;
        }
    }
}


template<class T>
void fc_soft(
    T* input,
    T* output,
    T* weight,
    T* bias,
    int indepth,
    int outdepth,
    int scale_oback,
    bool relu_flag)
{    
    std::cout<<"running fc soft layer"<<indepth<<" "<<outdepth<<std::endl;
    int widx=0;
    for(int od=0; od<outdepth;od++)
    {
        // output[od]=bias[od];

        for(int id=0;id<indepth;id++)
        {
            output[od]+=input[id]*weight[widx];
            widx++;
        }
        int rst=output[od];
        output[od]=(rst*scale_oback)>>OBACK_QUANT_BIT;
        output[od]+=bias[od];
        if(relu_flag && output[od]<0) output[od]=0;
    }
}

template<class T>
void flatten_soft(
    T* input,
    T* output,
    int indepth,
    int inheight,
    int inwidth)
{    
    std::cout<<"running flatten soft layer"<<std::endl;
    int outidx=0;
    for(int d=0;d<indepth;d++)
    {
        for(int h=0;h<inheight;h++)
        for(int w=0;w<inwidth;w++)
        {
            output[outidx]=input[outidx];
            outidx++;
        }
    }
}


template<int dummy>
void fc_soft_int(
    char* input,
    char* output,
    char* weight,
    short* bias,
    int indepth,
    int outdepth,
    int scale_oback,
    bool relu_flag)
{    
    int widx=0;
    for(int od=0; od<outdepth;od++)
    {
        int temp=0;

        for(int id=0;id<indepth;id++)
        {
            temp+=input[id]*weight[widx];
            widx++;
        }
        int rst=temp;
        output[od]=(rst*scale_oback)>>OBACK_QUANT_BIT;
        output[od]+=bias[od];
        if(relu_flag && output[od]<0) output[od]=0;
    }
}

#endif