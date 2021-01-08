#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <cassert>

#include "../src/wino_macro.h"
#include "../src/wino_struct.h"
#include "../testbench/debug.hpp"
#include "param.h"
#include "buffer.h"
#include "matrix_utility.hpp"
#include <map>

#include <cstdint>

void feature_to_ddr(
    char* feature_map1,
    char* feature_map2,
    ConvDesc_t &conv_desc,
    char* &input_DDR,
    char* &output_DDR
)
{
    int depth_ceildiv8 = conv_desc.indepth_align8/8;
    int group_depth_by8 = conv_desc.group_indepth/8;
    int group_depth_offset_by8 = conv_desc.group_indepth_offset/8;
    int width = conv_desc.inwidth;
    int width_align8 = conv_desc.inwidth_align8;
    int height = conv_desc.inheight;

    if(group_depth_offset_by8 == 0)
    {
        assert( height*width_align8*group_depth_by8*16 < FEATURE_PORT_DEPTH*16);
        input_DDR = (char*) MALLOC(FEATURE_PORT_DEPTH*16); //new char[height*width_align8*group_depth_by8*8*16];
        memset(input_DDR,0xCD,height*width_align8*group_depth_by8*16);
    }

    if(conv_desc.group_outdepth_offset ==0)
    {
        assert( conv_desc.outheight*conv_desc.outwidth_align8*conv_desc.group_outdepth*2*sizeof(ap_int<ODDR_WIDTH>) < FEATURE_PORT_DEPTH*16);
        output_DDR = (char*) MALLOC(FEATURE_PORT_DEPTH*16*sizeof(ap_int<ODDR_WIDTH>));//new char[conv_desc.outheight*conv_desc.outwidth_align8*conv_desc.group_outdepth*2*sizeof(ap_int<ODDR_WIDTH>)];
        memset(output_DDR,0xEF,conv_desc.outheight*conv_desc.outwidth_align8*conv_desc.group_outdepth*2*sizeof(ap_int<ODDR_WIDTH>));
    }

    int ddr_address = 0;
    for(int row=0; row<height;row++)
    {
        for(int  group_depth_idx_by8 =0; group_depth_idx_by8 < group_depth_by8 ; group_depth_idx_by8++)
        {
            for(int col=0; col<width_align8; col++)
            {
                for(int idx_tile8=0;idx_tile8<8;idx_tile8++)
                {
                   
                    int depth_idx_by8 = group_depth_idx_by8-group_depth_offset_by8;
                    int depth_idx = depth_idx_by8*8+ idx_tile8;

                    if(group_depth_idx_by8<group_depth_offset_by8 || depth_idx_by8>=depth_ceildiv8)
                    {
                        ddr_address+=2;
                        continue;
                    }

                    if(depth_idx < conv_desc.indepth && col < width )
                    {
                        int feature_map_address= depth_idx*height*width +row*width +col;
                        char data1= feature_map1[feature_map_address];
                        char data2= feature_map2[feature_map_address];
                        // printf("here %d %d %d\n",ddr_address,data1,data2);
                        input_DDR[ddr_address++]=data1;
                        input_DDR[ddr_address++]=data2;
                    }
                    else
                    {
                        ddr_address+=2;
                    }
                }
            }
        }
    }
    printf("%p \n",input_DDR );
    print_feature_map_ddr<0>(input_DDR,(char*) "inputddrcontent.txt",conv_desc.input_ddr_bytes);
}


void ddr_to_featuremap(
    char* feature_map1,
    char* feature_map2,
    char* DDR,
    ConvDesc_t &conv_desc
)
{

    ap_int<ODDR_WIDTH>* DDR_outwidth=(ap_int<ODDR_WIDTH>*) DDR;
    ap_int<ODDR_WIDTH>* DDR_feature_map1=(ap_int<ODDR_WIDTH>*) feature_map1;
    ap_int<ODDR_WIDTH>* DDR_feature_map2=(ap_int<ODDR_WIDTH>*) feature_map2;


    for(int  depth_idx =0; depth_idx < conv_desc.outdepth; depth_idx++)
    {
        for(int row=0; row<conv_desc.outheight/conv_desc.stride;row++)
        for(int col=0; col<conv_desc.outwidth/conv_desc.stride; col++)
        {
            int depth_tile8_idx = depth_idx/8;
            int idx_tile8 = depth_idx%8;

            int ddr_address1=   row*conv_desc.outdepth_align8*conv_desc.outwidth_align8/conv_desc.stride*2 
                                + depth_tile8_idx*conv_desc.outwidth_align8/conv_desc.stride*8*2 
                                + col*8*2+idx_tile8*2;
            int ddr_address2=   row*conv_desc.outdepth_align8*conv_desc.outwidth_align8/conv_desc.stride*2 
                                + depth_tile8_idx*conv_desc.outwidth_align8/conv_desc.stride*8*2 
                                + col*8*2+idx_tile8*2+1;
            ap_int<ODDR_WIDTH> data1= DDR_outwidth[ddr_address1];
            ap_int<ODDR_WIDTH> data2= DDR_outwidth[ddr_address2];
            
            int feature_map_address= depth_idx*conv_desc.outheight/conv_desc.stride*conv_desc.outwidth/conv_desc.stride +row*conv_desc.outwidth/conv_desc.stride +col;
            DDR_feature_map1[feature_map_address]=data1;
            DDR_feature_map2[feature_map_address]=data2;
        }
    }
    // FREE(DDR);
}



void feature_map_init(
    FeatureMapBuffer_t & featuremap,
    std::string style
)
{
    int depth = featuremap.blob_info->dim[0];
    int height = featuremap.blob_info->dim[1];
    int width = featuremap.blob_info->dim[2];
    std::cout<<"initializing feature map with mode "<<style <<std::endl;
    fflush(stdout);
    int address=0;
    for(int d=0;d<depth;d++)
    for(int h=0;h<height;h++)
    for(int w=0;w<width;w++)
    {

        if(style=="random")
        {
            featuremap.buffers_int[0][address]=random()%32-16;
            featuremap.buffers_int[1][address]=random()%32-16;
        }
        else if(style=="order")
        {
            featuremap.buffers_int[0][address]=h;
            featuremap.buffers_int[1][address]=w;
        }
        else if(style=="one")
        {
            featuremap.buffers_int[0][address]=1;
            featuremap.buffers_int[1][address]=1;
        }
        else
        {
            assert(0);
        }
        address++;
    }
    std::cout<<"initializing feature map done"<<std::endl;
    fflush(stdout);
}

char* wino6x6_weight_to_ddr(
    char* weight,
    ConvDesc_t conv_desc
)
{
    int weight_ddr;
    int port_segment_size=conv_desc.weightDDR_port_burst_length * conv_desc.weightDDR_burst_number;
    // int buffer_segment_size = conv_desc.weightDDR_buffer_burst_length * conv_desc.weightDDR_burst_number;
    assert(port_segment_size*4*sizeof(ap_uint<128>)<WEIGHT_PORT_DEPTH*16);

    char* weight_wino=(char*) MALLOC(WEIGHT_PORT_DEPTH*16*2);
    //= new char[port_segment_size*4*sizeof(ap_uint<128>)];
    printf("port_segment_size %d, %d \n",port_segment_size,conv_desc.weightDDR_burst_number);
    fflush(stdout);
    ap_uint<128>* weight_wino_128bit = (ap_uint<128>*) weight_wino;
    // int buffer_128_address[OUTDEPTH_MINITILE_SIZE]={0};
    
    printf("Odepth minitile size %d\n",OUTDEPTH_MINITILE_SIZE);

    int weight_load_idx=0;
    for(int outdepth_buffertile_baseidx=0;outdepth_buffertile_baseidx<conv_desc.outdepth;outdepth_buffertile_baseidx+=conv_desc.weightbuffer_load_outdepth_step)
    {
        for(int indepth_buffertile_baseidx=0;indepth_buffertile_baseidx<conv_desc.indepth;indepth_buffertile_baseidx+=conv_desc.weightbuffer_load_indepth_step)
        {

            int buffer_128_address=0;
            for(int indepth_minitile_baseidx=0;indepth_minitile_baseidx<conv_desc.weightbuffer_load_indepth_step; indepth_minitile_baseidx += INDEPTH_MINITILE_SIZE)
            {
                for(int outdepth_minitile_baseidx=0;outdepth_minitile_baseidx<conv_desc.weightbuffer_load_outdepth_step; outdepth_minitile_baseidx += OUTDEPTH_MINITILE_SIZE)
                {
                    for(int outdepth_minitile_idx=0;outdepth_minitile_idx<OUTDEPTH_MINITILE_SIZE;outdepth_minitile_idx++)
                    {
                        
                        int buffer_segment_offset=outdepth_minitile_idx/WEIGHT_FEED_NUMBER_PER_PORT*port_segment_size
                                                    +weight_load_idx*conv_desc.weightDDR_port_burst_length
                                                    +outdepth_minitile_idx%WEIGHT_FEED_NUMBER_PER_PORT*conv_desc.weightDDR_buffer_burst_length;



                        char indepth_minitile_data[WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP*sizeof(ap_uint<128>)];
                        char indepth_minitile_data_tranposed[WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP*sizeof(ap_uint<128>)];
                        
                        memset(indepth_minitile_data_tranposed,0,WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP*sizeof(ap_uint<128>));
                        memset(indepth_minitile_data,0,WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP*sizeof(ap_uint<128>));
           
                        for(int indepth_minitile_idx=0;indepth_minitile_idx<INDEPTH_MINITILE_SIZE;indepth_minitile_idx++)
                        {
                            int indepth_idx=indepth_buffertile_baseidx+indepth_minitile_baseidx+indepth_minitile_idx;
                            int outdepth_idx=outdepth_buffertile_baseidx+outdepth_minitile_baseidx+outdepth_minitile_idx;
                            int start_weight_offset = (indepth_idx*conv_desc.outdepth+outdepth_idx)*WINO_DOMAIN_SIZE_SQUARE;
                            if(indepth_idx<conv_desc.indepth && outdepth_idx < conv_desc.outdepth)
                            {
                                memcpy(indepth_minitile_data+indepth_minitile_idx*WINO_DOMAIN_SIZE_SQUARE,weight+start_weight_offset,WINO_DOMAIN_SIZE_SQUARE);
                            }
                            else
                            {
                                memset(indepth_minitile_data+indepth_minitile_idx*WINO_DOMAIN_SIZE_SQUARE,0,WINO_DOMAIN_SIZE_SQUARE);
                            }
                        }
                        //transpose WINO_DOMAIN_SIZE_SQUIRE x INDEPHT_MINITILE_SIZE to INDEPHT_MINITILE_SIZE x WINO_DOMAIN_SIZE_SQUIRE
                        for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                        for(int j=0;j<INDEPTH_MINITILE_SIZE;j++)
                        {
                            indepth_minitile_data_tranposed[i*INDEPTH_MINITILE_SIZE+j]=indepth_minitile_data[j*WINO_DOMAIN_SIZE_SQUARE+i];
                        }

                        
                        int write_address_128bit = buffer_128_address+buffer_segment_offset;
                        
                        memcpy(weight_wino_128bit+write_address_128bit,indepth_minitile_data,WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP*sizeof(ap_uint<128>));
                        // buffer_128_address[outdepth_minitile_idx]+=WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP;
                    }
                    buffer_128_address+=WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP;
                }
            }
            weight_load_idx++;
        }
    }
    return weight_wino;
}



void weight_to_ddr(
    char* weight,
    char* weight_wino,
    ConvDesc_t conv_desc
)
{
    // int weight_ddr;
    int port_segment_size=conv_desc.weightDDR_port_burst_length * conv_desc.weightDDR_burst_number;
    // int buffer_segment_size = conv_desc.weightDDR_buffer_burst_length * conv_desc.weightDDR_burst_number;
    assert(port_segment_size*4*sizeof(ap_uint<128>)<WEIGHT_PORT_DEPTH*16);

    // char* weight_wino=(char*) MALLOC(WEIGHT_PORT_DEPTH*16*2);
    //= new char[port_segment_size*4*sizeof(ap_uint<128>)];
    printf("port_segment_size %d, %d \n",port_segment_size,conv_desc.weightDDR_burst_number);
    fflush(stdout);
    ap_uint<128>* weight_wino_128bit = (ap_uint<128>*) weight_wino;
    // int buffer_128_address[OUTDEPTH_MINITILE_SIZE]={0};
    
    printf("Odepth minitile size %d\n",OUTDEPTH_MINITILE_SIZE);

    int weight_load_idx=0;


    
    // storing weight
    // storing
    for(int outdepth_buffertile_baseidx=0;outdepth_buffertile_baseidx<conv_desc.outdepth;outdepth_buffertile_baseidx+=conv_desc.weightbuffer_load_outdepth_step)
    {
        for(int indepth_buffertile_baseidx=0;indepth_buffertile_baseidx<conv_desc.indepth;indepth_buffertile_baseidx+=conv_desc.weightbuffer_load_indepth_step)
        {

            int buffer_128_address=0;
            for(int indepth_minitile_baseidx=0;indepth_minitile_baseidx<conv_desc.weightbuffer_load_indepth_step; indepth_minitile_baseidx += INDEPTH_MINITILE_SIZE)
            {
                for(int outdepth_minitile_baseidx=0;outdepth_minitile_baseidx<conv_desc.weightbuffer_load_outdepth_step; outdepth_minitile_baseidx += OUTDEPTH_MINITILE_SIZE)
                {
                    for(int outdepth_minitile_idx=0;outdepth_minitile_idx<OUTDEPTH_MINITILE_SIZE;outdepth_minitile_idx++)
                    {
                        
                        int buffer_segment_offset=outdepth_minitile_idx/WEIGHT_FEED_NUMBER_PER_PORT*port_segment_size
                                                    +weight_load_idx*conv_desc.weightDDR_port_burst_length
                                                    +outdepth_minitile_idx%WEIGHT_FEED_NUMBER_PER_PORT*conv_desc.weightDDR_buffer_burst_length;



                        ap_uint<128> indepth_minitile_data[INDEPTH_MINITILE_SIZE/2];
                        ap_uint<128> indepth_minitile_data_tranposed[INDEPTH_MINITILE_SIZE/2*sizeof(ap_uint<128>)];
                        
                        memset(indepth_minitile_data_tranposed,0,INDEPTH_MINITILE_SIZE/2*sizeof(ap_uint<128>));
                        memset(indepth_minitile_data,0,INDEPTH_MINITILE_SIZE/2*sizeof(ap_uint<128>));
           
                        for(int indepth_minitile_idx=0;indepth_minitile_idx<INDEPTH_MINITILE_SIZE;indepth_minitile_idx++)
                        {
                            int indepth_idx=indepth_buffertile_baseidx+indepth_minitile_baseidx+indepth_minitile_idx;
                            int outdepth_idx=outdepth_buffertile_baseidx+outdepth_minitile_baseidx+outdepth_minitile_idx;
                            int start_weight_offset = (outdepth_idx*conv_desc.indepth+indepth_idx)*9;
                            int offset_bit=indepth_minitile_idx%2*9*G_WIDTH;


                            if(indepth_idx<conv_desc.indepth && outdepth_idx < conv_desc.outdepth)
                            {
                                // memcpy(indepth_minitile_data+indepth_minitile_idx*WINO_DOMAIN_SIZE_SQUARE,weight+start_weight_offset,WINO_DOMAIN_SIZE_SQUARE);
                                
                                for(int k=0;k<9;k++)
                                {
                                    indepth_minitile_data[indepth_minitile_idx/2].range(offset_bit+k*G_WIDTH+G_WIDTH-1,offset_bit+k*G_WIDTH)=weight[start_weight_offset+k];
                                }
                            }
                            else
                            {
                                for(int k=0;k<9;k++)
                                {
                                    indepth_minitile_data[indepth_minitile_idx/2].range(offset_bit+k*G_WIDTH+G_WIDTH-1,offset_bit+k*G_WIDTH)=0;
                                }
                            }
                        }

                        
                        //transpose WINO_DOMAIN_SIZE_SQUIRE x INDEPHT_MINITILE_SIZE to INDEPHT_MINITILE_SIZE x WINO_DOMAIN_SIZE_SQUIRE
                        // for(int i=0;i<WINO_DOMAIN_SIZE_SQUARE;i++)
                        // for(int j=0;j<INDEPTH_MINITILE_SIZE;j++)
                        // {
                        //     indepth_minitile_data_tranposed[i*INDEPTH_MINITILE_SIZE+j]=indepth_minitile_data[j*WINO_DOMAIN_SIZE_SQUARE+i];
                        // }

                        
                        int write_address_128bit = buffer_128_address+buffer_segment_offset;
                        
                        memcpy(weight_wino_128bit+write_address_128bit,indepth_minitile_data,INDEPTH_MINITILE_SIZE/2*sizeof(ap_uint<128>));
                        // buffer_128_address[outdepth_minitile_idx]+=WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP;
                    }
                    buffer_128_address+=INDEPTH_MINITILE_SIZE/2;
                }
            }
            weight_load_idx++;
        }
    }

    printf("weight_to_ddr done");
    fflush(stdout);
}





template<class Tin,class Tout>
void weight_seperation(
    Tin* weight,
    Tout* weight_sep,
    int kernel_size,
    int merge_kernel_size,
    int indepth,
    int outdepth
)
{
    std::cout<<" kernel_size"<<kernel_size;
    int addr=0;
    for(int merge_kernel_row_offset=0;merge_kernel_row_offset<kernel_size;merge_kernel_row_offset+=merge_kernel_size)
    for(int merge_kernel_col_offset=0;merge_kernel_col_offset<kernel_size;merge_kernel_col_offset+=merge_kernel_size)
    {
        for(int od=0;od<outdepth;od++)
        for(int id=0;id<indepth;id++)
        {
            for(int merge_kernel_row_idx=0;merge_kernel_row_idx<merge_kernel_size;merge_kernel_row_idx++)
            for(int merge_kernel_col_idx=0;merge_kernel_col_idx<merge_kernel_size;merge_kernel_col_idx++)
            {
                int kernel_row_idx=merge_kernel_row_offset+merge_kernel_row_idx;
                int kernel_col_idx=merge_kernel_col_offset+merge_kernel_col_idx;
                int iaddr= ((od*indepth+id)*kernel_size+kernel_row_idx)*kernel_size+kernel_col_idx;


                if(kernel_row_idx<kernel_size && kernel_col_idx<kernel_size)
                {
                    weight_sep[addr]=weight[iaddr];
                }
                else
                {
                    weight_sep[addr]=0;
                }
                addr++;
            }
        }    
    }
}

void weight_int_to_merged_DDR(
    char* weight,
    char* weight_hw,
    int kernel_size, 
    int merge_kernel_size,
    ConvDesc_t conv_desc
)
{
    int indepth=conv_desc.indepth;
    int outdepth=conv_desc.outdepth;
    char* weight_sep=new char[ALIGN(kernel_size,merge_kernel_size)* ALIGN(kernel_size,merge_kernel_size)*indepth*outdepth];
    
    weight_seperation<char,char>( weight, weight_sep, kernel_size, merge_kernel_size, indepth,outdepth);

    // an single 3x3 weight segment length
    int weight_hw_step=16*conv_desc.weightDDR_port_burst_length * conv_desc.weightDDR_burst_number*4;
    int weight_sep_step= merge_kernel_size*merge_kernel_size*indepth*outdepth;
    int weight_hw_offset=0;
    int weight_sep_offset=0;

    for(int merge_kernel_row_offset=0;merge_kernel_row_offset<kernel_size;merge_kernel_row_offset+=merge_kernel_size)
    for(int merge_kernel_col_offset=0;merge_kernel_col_offset<kernel_size;merge_kernel_col_offset+=merge_kernel_size)
    {
        weight_to_ddr(
            weight_sep+weight_sep_offset,
            weight_hw+weight_hw_offset,
            conv_desc);
        weight_hw_offset+=weight_hw_step;
        weight_sep_offset+=weight_sep_step;
    }

    delete [] weight_sep;
}



void init_bias_int(
    short* bias,
    int length,
    std::string mode)
{
        for(int i=0;i<length;i++)
        {
            if(mode=="order")
            {
                bias[i]=0;
            }
            else if (mode=="zero")
            {
                bias[i]=0;
            }
            else
            {
                bias[i]=random()%256-128;
            }
        }
}

void init_weight_float(
    float* weight,
    int indepth,
    int outdepth,
    int kernelsize,
   std::string mode 
)
{
    for(int id=0;id<indepth;id++)
    for(int od=0;od<outdepth;od++)
    for(int ks=0;ks<kernelsize*kernelsize;ks++)
    {
        float value;
        if(mode=="depth_order")
        {
            if(ks%2==0) value=id;
            else value = od;
        }
        else if(mode=="kernel_order")
        {
            value=ks;
        }
        else if(mode == "random")
        {
            value=random()%256/256.0-0.5;
        }
        
        weight[ (od*indepth+id)*kernelsize*kernelsize+ks]=value;
    }
}



char* fc_weight_to_ddr(
    char* weight,
    short* bias,
    int indepth,
    int outdepth
)
{
    int indepth_align8 = ALIGN(indepth,8);
    int outdepth_align8 = ALIGN(outdepth,8);

    char* weight_ddr=(char*) MALLOC(WEIGHT_PORT_DEPTH*16*2);
    memset(weight_ddr,0,WEIGHT_PORT_DEPTH*16*2);
    
    char* weight_ddr0=weight_ddr+0*indepth_align8*outdepth_align8/4;
    char* weight_ddr1=weight_ddr+1*indepth_align8*outdepth_align8/4;
    char* weight_ddr2=weight_ddr+2*indepth_align8*outdepth_align8/4;
    char* weight_ddr3=weight_ddr+3*indepth_align8*outdepth_align8/4;

    int int128_offset=0;
    for(int j=0;j<indepth_align8;j+=8)
    {
        for(int i=0;i<outdepth_align8;i+=8)
        {

            char weight_mat[8][8];
            for(int ii=0;ii<8;ii++)
            for(int jj=0;jj<8;jj++)
            {
                if(i+ii<outdepth && j+jj< indepth)
                {
                    weight_mat[ii][jj]=weight[ (j+jj)*outdepth+i+ii];
                }
                else
                {
                    weight_mat[ii][jj]=0;
                }
            }
            

            for(int ii=0;ii<8;ii++){
                #pragma HLS unroll
                weight_ddr0[int128_offset*16+ii]=weight_mat[0][ii]; 
                weight_ddr0[int128_offset*16+8+ii]=weight_mat[1][ii];  
                weight_ddr1[int128_offset*16+ii]=weight_mat[2][ii]; 
                weight_ddr1[int128_offset*16+8+ii]=weight_mat[3][ii];  
                weight_ddr2[int128_offset*16+ii]=weight_mat[4][ii]; 
                weight_ddr2[int128_offset*16+8+ii]=weight_mat[5][ii];  
                weight_ddr3[int128_offset*16+ii]=weight_mat[6][ii]; 
                weight_ddr3[int128_offset*16+8+ii]=weight_mat[7][ii];  
            }
            int128_offset++;
        }
    }
    memcpy(weight_ddr+indepth_align8*outdepth_align8,bias,outdepth*2);

    return weight_ddr;
}


void initiate_featuremap_dict(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
)
{
    for(int i=0;i< blobinfo_vect.size();i++)
    {
        FeatureMapBuffer_t bufferstruct;
        bufferstruct.blob_info = &(blobinfo_vect[i]);
        feature_map_dict[blobinfo_vect[i].blob_name]=bufferstruct;
    }
}



void alloc_float_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
)
{

    for(int i=0;i< blobinfo_vect.size();i++)
    {
        FeatureMapBuffer_t & bufferstruct=feature_map_dict[blobinfo_vect[i].blob_name];
        
        int length_sw= blobinfo_vect[i].dim[0]*blobinfo_vect[i].dim[1]*blobinfo_vect[i].dim[2];
        bufferstruct.byte_size_sw=length_sw*sizeof(float);
        bufferstruct.buffer_size_sw=length_sw;
        float *featuremap1 = new float[length_sw];
        float *featuremap2 = new float[length_sw];
        float *featuremap1_scale = new float[length_sw];
        float *featuremap2_scale = new float[length_sw];
        float *featuremap_gold = new float[length_sw];
        bufferstruct.buffers_sw.push_back(featuremap1);
        bufferstruct.buffers_sw.push_back(featuremap2);
        bufferstruct.buffers_scale.push_back(featuremap1_scale);
        bufferstruct.buffers_scale.push_back(featuremap2_scale);
        bufferstruct.buffers_gold.push_back(featuremap_gold);
    }
    std::map< std::string, FeatureMapBuffer_t >::iterator itr;
    for(itr =feature_map_dict.begin(); itr!= feature_map_dict.end();++itr)
    {
        std::cout<<"Assigned blob ["<<itr->first<<"]-- software bytes: "<<itr->second.byte_size_sw<<std::endl;
    }

}

void load_float_golden_featuremap(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
)
{
    std::map< std::string, FeatureMapBuffer_t >::iterator it;
    for ( it = feature_map_dict.begin(); it != feature_map_dict.end(); it++ )
    {
        if(it->second.blob_info->goldfilename.size() >0 && it->second.blob_info->goldfilename[0]!="None")
        {
            std::cout<<it->second.blob_info->goldfilename[0]<<std::endl;
            std::cout<<it->second.blob_info->blob_name <<std::endl;
            fflush(stdout);
            // getchar();
            FILE* wptr=fopen(it->second.blob_info->goldfilename[0].c_str(),"rb");
            std::cout<<it->second.blob_info->blob_name<<std::endl;
            std::cout<<it->second.blob_info->goldfilename[0]<<std::endl;
            fflush(stdout);
            fread(it->second.buffers_gold[0],1,it->second.byte_size_sw,wptr);
            fseek(wptr,0,SEEK_END);
            int filelength=ftell(wptr);
            std::cout<<filelength<<","<< it->second.byte_size_sw<<std::endl;
            assert(filelength==it->second.byte_size_sw);
            fclose(wptr);
        }
    }
}

void load_img_featuremap(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
)
{
    if(feature_map_dict["img"].buffers_sw.size() == 0)
    {
        FeatureMapBuffer_t & bufferstruct=feature_map_dict["img"];
        int length_sw= bufferstruct.blob_info->dim[0]*bufferstruct.blob_info->dim[1]*bufferstruct.blob_info->dim[2];
        bufferstruct.byte_size_sw=length_sw*sizeof(float);
        bufferstruct.buffer_size_sw=length_sw;
        float *featuremap1 = new float[length_sw];
        float *featuremap2 = new float[length_sw];
        float *featuremap_gold = new float[length_sw];
        bufferstruct.buffers_sw.push_back(featuremap1);
        bufferstruct.buffers_sw.push_back(featuremap2);
    }

    for(int i=0;i<feature_map_dict["img"].blob_info->goldfilename.size();i++)
    {
        FILE* wptr=fopen(feature_map_dict["img"].blob_info->goldfilename[i].c_str(),"rb");
        fread(feature_map_dict["img"].buffers_sw[i],1,feature_map_dict["img"].byte_size_sw,wptr);
        fseek(wptr,0,SEEK_END);
        int filelength=ftell(wptr);
        assert(filelength==feature_map_dict["img"].byte_size_sw);
        fclose(wptr);
    }
}

void alloc_float_weight_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect,
    std::string mode
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type == "conv")
        {
            std::cout<<"load weight for conv layer "<< layerinfo_vect[i].layer_name <<std::endl;
            int length = layerinfo_vect[i].indim[0] * layerinfo_vect[i].outdim[0] * layerinfo_vect[i].kernel_size * layerinfo_vect[i].kernel_size;
            float* weight_float = new float[length];
            float* bias_float = new float[layerinfo_vect[i].outdim[0]];
            
            layerinfo_vect[i].weightbuffers_sw.push_back(weight_float);
            layerinfo_vect[i].weightbuffers_sw_size.push_back(length);

            if(layerinfo_vect[i].weightfile!="None")
            {
                FILE* wptr=fopen(layerinfo_vect[i].weightfile.c_str(),"rb");
                fread(layerinfo_vect[i].weightbuffers_sw[0],sizeof(float),length,wptr);
                fseek(wptr,0,SEEK_END);
                int filelength=ftell(wptr);
        
                std::cout<<layerinfo_vect[i].layer_name<<std::endl;
                std::cout<<layerinfo_vect[i].weightfile<<std::endl;
                std::cout<<filelength<<std::endl;
                fflush(stdout);
                assert(filelength==sizeof(float)*length);
                fclose(wptr);
            }
            else
            {
                // memset(layerinfo_vect[i].weightbuffers_sw[0],1,sizeof(float)*length);
                init_weight_float(
                layerinfo_vect[i].weightbuffers_sw[0],
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].kernel_size,
                mode);                
            }
            

            layerinfo_vect[i].biasbuffers_sw.push_back(bias_float);

            if(layerinfo_vect[i].biasfile!="None")
            {
                std::cout<<layerinfo_vect[i].biasfile<<std::endl;
                std::cout<<layerinfo_vect[i].layer_name<<std::endl;
                fflush(stdout);
                FILE* wptr=fopen(layerinfo_vect[i].biasfile.c_str(),"rb");
                fread(layerinfo_vect[i].biasbuffers_sw[0],sizeof(float),layerinfo_vect[i].outdim[0],wptr);
                fseek(wptr,0,SEEK_END);
                int filelength=ftell(wptr);
                assert(filelength==sizeof(float)*layerinfo_vect[i].outdim[0]);
                fclose(wptr);
                std::cout<<"done"<<std::endl;
            }
            else
            {
                memset(layerinfo_vect[i].biasbuffers_sw[0],0,sizeof(float)*layerinfo_vect[i].outdim[0]);
            }
        }
        else if( layerinfo_vect[i].layer_type == "linear")
        {
            std::cout<<"load weight for linear layer "<< layerinfo_vect[i].layer_name <<std::endl;
            int length = layerinfo_vect[i].indim[0] * layerinfo_vect[i].outdim[0];
            float* weight_float = new float[length];
            float* bias_float = new float[layerinfo_vect[i].outdim[0]];

            layerinfo_vect[i].weightbuffers_sw.push_back(weight_float);
            layerinfo_vect[i].weightbuffers_sw_size.push_back(length);

            if(layerinfo_vect[i].weightfile!="None")
            {
                FILE* wptr=fopen(layerinfo_vect[i].weightfile.c_str(),"rb");
                fread(layerinfo_vect[i].weightbuffers_sw[0],sizeof(float),length,wptr);
                fseek(wptr,0,SEEK_END);
                int filelength=ftell(wptr);
        
                std::cout<<layerinfo_vect[i].layer_name<<std::endl;
                std::cout<<layerinfo_vect[i].weightfile<<std::endl;
                std::cout<<filelength<<std::endl;
                fflush(stdout);
                assert(filelength==sizeof(float)*length);
                fclose(wptr);
            }
            else
            {
                memset(layerinfo_vect[i].weightbuffers_sw[0],0,sizeof(float)*length);
            }
            layerinfo_vect[i].biasbuffers_sw.push_back(bias_float);

            if(layerinfo_vect[i].biasfile!="None")
            {
                FILE* wptr=fopen(layerinfo_vect[i].biasfile.c_str(),"rb");
                fread(layerinfo_vect[i].biasbuffers_sw[0],sizeof(float),layerinfo_vect[i].outdim[0],wptr);
                fseek(wptr,0,SEEK_END);
                int filelength=ftell(wptr);
                assert(filelength==sizeof(float)*layerinfo_vect[i].outdim[0]);
                fclose(wptr);
            }
            else
            {
                memset(layerinfo_vect[i].weightbuffers_sw[0],0,sizeof(float)*layerinfo_vect[i].outdim[0]);
            }
        }
    }
}


void transform_weight_5x5(
    float* weight,
    float* weight_wino,
    // float* G,
    // float* Gt,
    int indepth,
    int outdepth,
    int kernel_size
)
{
    float G[36]={   1/4.0,  0,          0,          0,          0,
                    -1/6.0, -1/6.0,     -1/6.0,     -1/6.0,     -1/6.0, 
                    -1/6.0, 1/6.0,      -1/6.0,     1/6.0,     -1/6.0, 
                    1/24.0, 1/12.0,     1/6.0,      1/3.0,      2/3.0,
                    1/24.0, -1/12.0,    1/6.0,      -1/3.0,     2/3.0,
                    0,      0,          0,          0,          1    };
    
    float Gt[36]={  1/4.0,	-1/6.0,	    -1/6.0,	    1/24.0,	    1/24.0,	    0,
                    0,      -1/6.0,	    1/6.0,	    1/12.0,	    -1/12.0,	0,
                    0,      -1/6.0,	    -1/6.0,	    1/6.0,	    1/6.0,	    0,
                    0,      -1/6.0,	    1/6.0,	    1/3.0,	    -1/3.0,	    0,
                    0,      -1/6.0,	    -1/6.0,	    2/3.0,	    2/3.0,	    1};

    int normal_offset=0;
    int wino_offset=0;

    for(int j=0;j<outdepth*indepth;j++)
    {
            float tempmat[36];
            matrix_mult<float,float,float> (G, WINO_DOMAIN_SIZE, kernel_size, weight+normal_offset, kernel_size,kernel_size, tempmat, WINO_DOMAIN_SIZE, kernel_size );
            matrix_mult<float,float,float> (tempmat, WINO_DOMAIN_SIZE, kernel_size, Gt, kernel_size,WINO_DOMAIN_SIZE, weight_wino+wino_offset, WINO_DOMAIN_SIZE, WINO_DOMAIN_SIZE);
            normal_offset+=kernel_size*kernel_size;
            wino_offset+=WINO_DOMAIN_SIZE_SQUARE;
    }

}



void weight_pretransform_float(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    std::cout<<"weight_pretransform_float START"<<std::endl; fflush(stdout);
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type == "conv" && layerinfo_vect[i].kernel_size==5)
        {
            int indepth= layerinfo_vect[i].indim[0];
            int outdepth= layerinfo_vect[i].outdim[0];
            float* wino_weight_buffer= new float[indepth*outdepth*36];
            transform_weight_5x5(
                layerinfo_vect[i].weightbuffers_sw[0],
                wino_weight_buffer,
                indepth,
                outdepth,
                5
            );
            layerinfo_vect[i].winobuffers_float.push_back(wino_weight_buffer);
            layerinfo_vect[i].winobuffers_float_size.push_back(indepth*outdepth*36);
        }
    }
    std::cout<<"weight_pretransform_float END"<<std::endl; fflush(stdout);
}

float find_scale_factor(
    float* data,
    int data_number,
    int quant_bit,
    int* absmax
)
{
    float min,max;
    min=data[0];
    max=data[0];

    for(int i=0;i<data_number;i++)
    {
        if(min>data[i]) min=data[i];
        if(max<data[i]) max=data[i];
    }
    if(min<0) min=-min;
    if(max<0) max=-max;

    if(min > max) max=min;

    if(absmax!=NULL)
        *absmax=max;

    int scaled_max = ( (1<<(quant_bit-1))-1);
    printf("scale max: %f %d\n", max, scaled_max);
    float scale_factor = scaled_max/max;
    return scale_factor;
}


void compute_scale_factors(
    std::vector< LayerInfo_t > & layerinfo_vect,
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
)
{

    std::map< std::string, FeatureMapBuffer_t >::iterator itr;

    for(itr =feature_map_dict.begin(); itr!= feature_map_dict.end();++itr)
    {
        std::cout<<itr->second.blob_info->blob_name<<std::endl;
        itr->second.Scale_blob=find_scale_factor(itr->second.buffers_gold[0],itr->second.buffer_size_sw,IN_WIDTH,NULL);
    }
    // TODO: Unify the Scale_blob factor for merging blobs the minimum scale factor, used in group/elementwise add
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="residual")
        {
            std::cout<<"Unifying scale factor for residual layer "<<layerinfo_vect[i].layer_name<<std::endl;

            float scale_factor=1000000000;
            for(int j=0;j<layerinfo_vect[i].bottoms.size();j++)
            {
                std::cout<<"Original Blob ["<<layerinfo_vect[i].bottoms[j]<<"] "<<feature_map_dict[layerinfo_vect[i].bottoms[j]].Scale_blob<<std::endl;
                if(scale_factor>feature_map_dict[layerinfo_vect[i].bottoms[j]].Scale_blob)
                {
                    scale_factor=feature_map_dict[layerinfo_vect[i].bottoms[j]].Scale_blob;
                }
            }



            std::cout<<"Min Blob: "<<scale_factor<<std::endl;

            for(int j=0;j<layerinfo_vect[i].bottoms.size();j++)
            {
                feature_map_dict[layerinfo_vect[i].bottoms[j]].Scale_blob=scale_factor;
                
            }

            // getchar();

        }
    }

    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_name=="avgpool" || layerinfo_vect[i].layer_type=="flatten")
        {
            feature_map_dict[layerinfo_vect[i].tops[0]].Scale_blob=feature_map_dict[layerinfo_vect[i].bottoms[0]].Scale_blob;
        }
    }

    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type!="conv" && layerinfo_vect[i].layer_type!="linear")
            continue;
        int weight_bit;
        if(layerinfo_vect[i].layer_type=="conv")
        {
            weight_bit=layerinfo_vect[i].kernel_size!=5?G_WIDTH:8;
        }
        else
        {
            weight_bit=8;
        }
        
        layerinfo_vect[i].Scale_w=find_scale_factor(layerinfo_vect[i].weightbuffers_sw[0],layerinfo_vect[i].weightbuffers_sw_size[0],weight_bit,NULL);

        if(layerinfo_vect[i].layer_type=="linear")
        {
            layerinfo_vect[i].Scale_wino=1;
            continue;
        }

        float Scale_winograd_tranform;
        if( layerinfo_vect[i].kernel_size!=5)
        {
            Scale_winograd_tranform= ((float) G_TRANSFACTOR*G_TRANSFACTOR)/ (1<< (RG_WIDTH-W_WIDTH) );
        }
        else
        {
            Scale_winograd_tranform=1;
        }
       
        layerinfo_vect[i].Scale_wino=Scale_winograd_tranform;
        
    }
    
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        std::string & inname=layerinfo_vect[i].bottoms[0];
        std::string & outname=layerinfo_vect[i].tops[0];

        layerinfo_vect[i].Scale_i=feature_map_dict[inname].Scale_blob;
        layerinfo_vect[i].Scale_o=feature_map_dict[outname].Scale_blob;

        if(layerinfo_vect[i].layer_type=="conv" or layerinfo_vect[i].layer_type=="linear")
        {

            int total_quant_bit= layerinfo_vect[i].layer_type=="conv"? DB_QUANT_BIT+BTB_QUANT_BIT+UV_QUANT_BIT+UVA_QUANT_BIT+ATA_QUANT_BIT+OUT_BUFFER_QUANT_BIT:0;
            layerinfo_vect[i].Scale_oback=
                                layerinfo_vect[i].Scale_o*
                                (1<<total_quant_bit)/
                                (layerinfo_vect[i].Scale_i*
                                layerinfo_vect[i].Scale_wino*
                                layerinfo_vect[i].Scale_w);
            layerinfo_vect[i].Scale_oback_int = round(layerinfo_vect[i].Scale_oback*(1<<OBACK_QUANT_BIT));


            std::cout<<layerinfo_vect[i].layer_name<<" Scale out "<<layerinfo_vect[i].Scale_o<<std::endl;
            std::cout<<layerinfo_vect[i].layer_name<<" Scale in "<<layerinfo_vect[i].Scale_i<<std::endl;
            std::cout<<layerinfo_vect[i].layer_name<<" Scale w "<<layerinfo_vect[i].Scale_w<<std::endl;
            std::cout<<layerinfo_vect[i].layer_name<<" Scale wino "<<layerinfo_vect[i].Scale_wino<<std::endl;
            std::cout<<layerinfo_vect[i].layer_name<<" Scale Oback "<<layerinfo_vect[i].Scale_oback<<std::endl;
            layerinfo_vect[i].Scale_b=
                                    layerinfo_vect[i].Scale_i*
                                    layerinfo_vect[i].Scale_w*
                                    layerinfo_vect[i].Scale_wino*
                                    layerinfo_vect[i].Scale_oback/
                                    (1<<total_quant_bit);
            std::cout<<layerinfo_vect[i].layer_name<<" Scale_b "<<layerinfo_vect[i].Scale_b<<std::endl;
            float total_scale=
                            layerinfo_vect[i].Scale_i*           
                            layerinfo_vect[i].Scale_w*           
                            layerinfo_vect[i].Scale_wino*
                            layerinfo_vect[i].Scale_oback/
                            ( (1<<total_quant_bit)* layerinfo_vect[i].Scale_o);
            
            if(total_scale <0.5 || total_scale >2)
            {
                printf("WARNING: TOTAL SCALE NOT ACCURATE! %f", total_scale);
            }
            std::cout<<layerinfo_vect[i].layer_name <<" "<<layerinfo_vect[i].Scale_oback_int<<std::endl;
            // getchar();         
        }
        else if( layerinfo_vect[i].layer_type=="residual" )
        {
            layerinfo_vect[i].Scale_oback=layerinfo_vect[i].Scale_o/layerinfo_vect[i].Scale_i;
            layerinfo_vect[i].Scale_oback_int = round(layerinfo_vect[i].Scale_oback*(1<<OBACK_QUANT_BIT));
        }
    }
}





void scale_weight_bias_data_float(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{       
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="conv" or layerinfo_vect[i].layer_type=="linear")
        {
            float* weight;
            float* weight_scaled;
            float* bias_scaled;
            int weight_size;
            int quant_bit;
            

            if(layerinfo_vect[i].kernel_size!=5 && layerinfo_vect[i].layer_type=="conv")
                quant_bit=G_WIDTH;
            else
                quant_bit=8;

            if(layerinfo_vect[i].kernel_size==5 && layerinfo_vect[i].layer_type=="conv")
            {
                weight = layerinfo_vect[i].winobuffers_float[0];
                weight_size=layerinfo_vect[i].winobuffers_float_size[0];
            }
            else
            {
                weight = layerinfo_vect[i].weightbuffers_sw[0];
                weight_size=layerinfo_vect[i].weightbuffers_sw_size[0];
            }
            weight_scaled = new float[weight_size];
            layerinfo_vect[i].weightbuffers_float_scaled.push_back(weight_scaled);
            layerinfo_vect[i].weightbuffers_float_scaled_size.push_back(weight_size);
            


            scale_data<float,float>(
                weight,
                weight_scaled,
                layerinfo_vect[i].Scale_w,
                weight_size,
                quant_bit);


          

            bias_scaled = new float[layerinfo_vect[i].outdim[0]];


            scale_data<float,float>(
                layerinfo_vect[i].biasbuffers_sw[0],
                bias_scaled,
                layerinfo_vect[i].Scale_b,
                layerinfo_vect[i].outdim[0],
                16);

            layerinfo_vect[i].bias_float_scaled.push_back(bias_scaled);
        }
    }
}



void reorder_weight_quant(
    float* weight,
    char* weight_quant,
    int indepth,
    int outdepth,
    int K
)
{
    // int addr=0;
    // for(int i=0;i<outdepth;i++)
    // for(int j=0;j<indepth;j++)
    // {
    //     for(int win=0;win<K*K;win++)
    //     {
    //         // std::cout<<addr<<std::endl;
    //         // fflush(stdout);
    //         weight_quant[j*outdepth*K*K+i*K*K+win]
    //         =weight[addr];
    //         addr++;
    //     }
    // }

    for(int i=0;i<outdepth*indepth*K*K;i++){
            weight_quant[i]
            =weight[i];
    }
}


void weight_preprocess_quant(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        
        if(layerinfo_vect[i].layer_type=="conv" )
        {
            int weight_size = layerinfo_vect[i].weightbuffers_float_scaled_size[0];
            int kernel_size = layerinfo_vect[i].kernel_size;
            if(kernel_size ==5) kernel_size=6;

            char* weight_quant = new char[weight_size];
            fflush(stdout);
            reorder_weight_quant(
                layerinfo_vect[i].weightbuffers_float_scaled[0],
                weight_quant,
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].outdim[0],
                kernel_size
            );
            layerinfo_vect[i].weightbuffer_quant.push_back(weight_quant);
            layerinfo_vect[i].weightbuffer_quant_size.push_back(weight_size);

            short* bias_quant=new short[layerinfo_vect[i].outdim[0]];
            for(int k=0;k<layerinfo_vect[i].outdim[0];k++)
            {
                bias_quant[k]=layerinfo_vect[i].bias_float_scaled[0][k];
            }
            layerinfo_vect[i].biasbuffer_quant.push_back(bias_quant);
        }
        else if(layerinfo_vect[i].layer_type=="linear")
        {
            int weight_size = layerinfo_vect[i].weightbuffers_float_scaled_size[0];
            char* weight_quant = new char[weight_size];

            for(int k=0;k<weight_size;k++)
            {
                weight_quant[k]=layerinfo_vect[i].weightbuffers_float_scaled[0][k];
            }
            layerinfo_vect[i].weightbuffer_quant.push_back(weight_quant);
            layerinfo_vect[i].weightbuffer_quant_size.push_back(weight_size);

            short* bias_quant=new short[layerinfo_vect[i].outdim[0]];
            for(int k=0;k<layerinfo_vect[i].outdim[0];k++)
            {
                bias_quant[k]=layerinfo_vect[i].bias_float_scaled[0][k];
            }
            layerinfo_vect[i].biasbuffer_quant.push_back(bias_quant);
        }
    }
}


void alloc_int_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
)
{
    for(int i=0;i< blobinfo_vect.size();i++)
    {
        FeatureMapBuffer_t & bufferstruct=feature_map_dict[blobinfo_vect[i].blob_name];
        
        int length_int= blobinfo_vect[i].dim[0]*blobinfo_vect[i].dim[1]*blobinfo_vect[i].dim[2];
        bufferstruct.byte_size_int=length_int*sizeof(char);
        bufferstruct.buffer_size_int=length_int;
        char *featuremap1 = new char[length_int];
        char *featuremap2 = new char[length_int];
        bufferstruct.buffers_int.push_back(featuremap1);
        bufferstruct.buffers_int.push_back(featuremap2);
    }
}


void alloc_hw_featuremap_mem_single_blob(
    FeatureMapBuffer_t & featuremap
)
{
    int depth_align8=ALIGN(featuremap.blob_info->dim[0],8);
    int width_align8=ALIGN(featuremap.blob_info->dim[2],8);
    int height_align8=ALIGN(featuremap.blob_info->dim[1],8);


    assert(depth_align8 %8 ==0);

    int total_bytes = depth_align8*width_align8*height_align8*2*sizeof(ap_int<ODDR_WIDTH>);
    printf("bottom bytes [%d %d %d]\n",depth_align8,width_align8, height_align8);
    assert( total_bytes < FEATURE_PORT_DEPTH*16);

    if(featuremap.buffers_hw.size()!=0)
    {
        assert(total_bytes==featuremap.byte_size_hw);
        return;
    }

    char* input_DDR = (char*) MALLOC(FEATURE_PORT_DEPTH*16); //new char[height*width_align8*group_depth_by8*8*16];
    
    memset(input_DDR,0xCD,total_bytes);
    featuremap.buffers_hw.push_back(input_DDR);
    featuremap.byte_size_hw=total_bytes;
    featuremap.group_depth=depth_align8;    
}



void alloc_hw_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    // for( int i=0;i<layerinfo_vect.size();i++)
    // {
    //     if(layerinfo_vect[i].layer_type=="conv")
    //     {
    //         std::cout<<"alloc hw layer feature map"<< layerinfo_vect[i].layer_name << std::endl;
    //         for( int j =0;j<layerinfo_vect[i].bottoms.size();j++)
    //         {
    //             std::string name = layerinfo_vect[i].bottoms[j];
    //             alloc_hw_featuremap_mem_single_blob(
    //                 feature_map_dict[name],
    //                 layerinfo_vect[i].conv_desc,
    //                 true
    //             );
    //         }


    //         for( int j =0;j<layerinfo_vect[i].bottoms.size();j++)
    //         {
    //             std::string name = layerinfo_vect[i].tops[j];
    //             alloc_hw_featuremap_mem_single_blob(
    //                 feature_map_dict[name],
    //                 layerinfo_vect[i].conv_desc,
    //                 false
    //             );
    //         }
    //     }
    // }
    std::map< std::string, FeatureMapBuffer_t >::iterator iter;
    for(iter=feature_map_dict.begin();iter!=feature_map_dict.end();iter++)
    {
        alloc_hw_featuremap_mem_single_blob(iter->second);
    }

}


void alloc_convdesc_and_bias_buffer_single_layer(
    LayerInfo_t & layerinfo
)
{
    ConvDesc_t & conv_desc= layerinfo.conv_desc;
    int bias_number = conv_desc.outdepth_align8;
    std::cout<<" Bias number "<<bias_number<<std::endl;
    char* convdesc_and_bias_buffer = (char*) MALLOC( 128*sizeof(int) + bias_number*sizeof(short) );
    layerinfo.conv_desc_buffer.push_back(convdesc_and_bias_buffer);

}

void alloc_convdesc_and_bias_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="conv")
        {
            alloc_convdesc_and_bias_buffer_single_layer(layerinfo_vect[i]);
        }
    }
}
void alloc_hw_weight_buffer_single_layer(
    LayerInfo_t & layerinfo
)
{
    ConvDesc_t & conv_desc = layerinfo.conv_desc;

    int segment_number;
    if(WINO_DOMAIN_SIZE==4)
        segment_number = CEIL_DIV(layerinfo.kernel_size,3)*CEIL_DIV(layerinfo.kernel_size,3);
    else
        segment_number = 1;


    int port_segment_size=conv_desc.weightDDR_port_burst_length * conv_desc.weightDDR_burst_number;

    std::cout<<"port_segment_size "<<port_segment_size<<std::endl;
    // int buffer_segment_size = conv_desc.weightDDR_buffer_burst_length * conv_desc.weightDDR_burst_number;

    assert(port_segment_size*WEIGHT_PORT_NUM*sizeof(ap_uint<128>)*segment_number<WEIGHT_PORT_DEPTH*16);
    int byte_size = port_segment_size*WEIGHT_PORT_NUM*sizeof(ap_uint<128>)*segment_number;

    char* weight_hw=(char*) MALLOC(WEIGHT_PORT_DEPTH*16*2);
    memset(weight_hw,0xff,WEIGHT_PORT_DEPTH*16*2);
    for(int i=0;i<WEIGHT_PORT_NUM;i++)
    {
        layerinfo.weightbuffers_hw.push_back(weight_hw+port_segment_size*i*sizeof(ap_uint<128>));
    }
    layerinfo.weightbuffers_hw_size.push_back(byte_size);
    layerinfo.port_segment_size.push_back(port_segment_size);
}


void alloc_hw_weight_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="conv")
        {
            alloc_hw_weight_buffer_single_layer(layerinfo_vect[i]);
        }
    }
}


void process_hw_weight_buffer_single_layer(
    LayerInfo_t & layerinfo
)
{
    ConvDesc_t & conv_desc = layerinfo.conv_desc;
    std::cout<<layerinfo.layer_name<<std::endl;
    fflush(stdout);
    // weight_to_ddr(layerinfo.weightbuffer_quant[0],layerinfo.weightbuffers_hw[0],conv_desc);
    weight_int_to_merged_DDR(layerinfo.weightbuffer_quant[0], layerinfo.weightbuffers_hw[0],layerinfo.kernel_size,3,conv_desc);
}


void process_hw_weight_buffer(
    std::vector<LayerInfo_t> & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="conv")
        {
            process_hw_weight_buffer_single_layer(layerinfo_vect[i]);           
        }
    }
}

// void dummy_hw_weight_buffer_min1(
//     std::vector< LayerInfo_t > & layerinfo_vect
// )
// {
//     for(int i=0;i<layerinfo_vect.size();i++)
//     {
//         if(layerinfo_vect[i].layer_type=="conv")
//         {
//             alloc_hw_weight_buffer_single_layer(layerinfo_vect[i]);
//         }
//     }
// }




void featuremap_hw_to_int_pointers(
    char* sourceDDR,
    char* target0,
    char* target1,
    int height,
    int width,
    int depth,
    int group_depth_offset,
    int group_depth
)
{
    int group_depth_by8=group_depth/8;
    int width_align8= ALIGN(width,8);
    int group_depth_offset_by8=group_depth_offset/8;
    int depth_ceildiv8 = CEIL_DIV(depth,8);

    char* feature_map1=target0;
    char* feature_map2=target1;
    char* featuremap_buffer_hw=sourceDDR;

    int ddr_address = 0;
    for(int row=0; row<height;row++)
    {
        for(int  group_depth_idx_by8 =0; group_depth_idx_by8 < group_depth_by8 ; group_depth_idx_by8++)
        {
            for(int col=0; col<width_align8; col++)
            {
                for(int idx_tile8=0;idx_tile8<8;idx_tile8++)
                {
                    int depth_idx_by8 = group_depth_idx_by8-group_depth_offset_by8;
                    int depth_idx = depth_idx_by8*8+ idx_tile8;
                    if(group_depth_idx_by8<group_depth_offset_by8 || depth_idx_by8>=depth_ceildiv8)
                    {
                        ddr_address+=2;
                        continue;
                    }
                    if(depth_idx < depth && col < width )
                    {
                        int feature_map_address= depth_idx*height*width +row*width +col;
                        char data1=featuremap_buffer_hw[ddr_address++] ;
                        char data2=featuremap_buffer_hw[ddr_address++];
                        feature_map1[feature_map_address]=data1;
                        feature_map2[feature_map_address]=data2;
                    }
                    else
                    {
                        ddr_address+=2;
                    }
                }
            }
        }
    }
}


void featuremap_int_to_hw_pointers(
    char* source0,
    char* source1,
    char* targetDDR,
    int height,
    int width,
    int depth,
    int group_depth_offset,
    int group_depth
)
{
    int group_depth_by8=group_depth/8;
    int width_align8= ALIGN(width,8);
    int group_depth_offset_by8=group_depth_offset/8;
    int depth_ceildiv8 = CEIL_DIV(depth,8);
    char* feature_map1=source0;
    char* feature_map2=source1;
    char* featuremap_buffer_hw=targetDDR;

    int ddr_address = 0;
    for(int row=0; row<height;row++)
    {
        for(int  group_depth_idx_by8 =0; group_depth_idx_by8 < group_depth_by8 ; group_depth_idx_by8++)
        {
            for(int col=0; col<width_align8; col++)
            {
                for(int idx_tile8=0;idx_tile8<8;idx_tile8++)
                {
                   
                    int depth_idx_by8 = group_depth_idx_by8-group_depth_offset_by8;
                    int depth_idx = depth_idx_by8*8+ idx_tile8;

                    if(group_depth_idx_by8<group_depth_offset_by8 || depth_idx_by8>=depth_ceildiv8)
                    {
                        ddr_address+=2;
                        continue;
                    }

                    if(depth_idx < depth && col < width )
                    {
                        int feature_map_address= depth_idx*height*width +row*width +col;
                        char data1= feature_map1[feature_map_address];
                        char data2= feature_map2[feature_map_address];
                        // printf("here %d %d %d\n",ddr_address,data1,data2);
                        featuremap_buffer_hw[ddr_address++]=data1;
                        featuremap_buffer_hw[ddr_address++]=data2;
                    }
                    else
                    {
                        ddr_address+=2;
                    }
                }
            }
        }
    }
}



void featuremap_int_to_hw(
    FeatureMapBuffer_t & featuremap,
    int group_depth_offset,
    int group_depth
)
{

    int height=featuremap.blob_info->dim[1];
    int width=featuremap.blob_info->dim[2];
    int depth=featuremap.blob_info->dim[0];

    int group_depth_by8=group_depth/8;
    int width_align8= ALIGN(width,8);
    int group_depth_offset_by8=group_depth_offset/8;
    int depth_ceildiv8 = CEIL_DIV(depth,8);

    char* feature_map1;
    char* feature_map2;
    
    if(featuremap.buffers_int.size()==2)
    {
        feature_map1=featuremap.buffers_int[0];
        feature_map2=featuremap.buffers_int[1];
    }
    else if(featuremap.buffers_int.size()==1)
    {
        feature_map1=featuremap.buffers_int[0];
        feature_map2=featuremap.buffers_int[0];    
    }
    else
    {
        std::cout<<"ERROR featuremap_int number: "<<featuremap.buffers_int.size()<<std::endl;
        assert(0);
    }
    
    // if(featuremap.buffers_hw.size()!=1)
    // {
    //     std::cout<<"ERROR featuremap_hw number: "<<featuremap.buffers_int.size()<<std::endl;
    //     assert(0);
    // }
    
    char* featuremap_buffer_hw= featuremap.buffers_hw[0];

    int ddr_address = 0;


    for(int row=0; row<height;row++)
    {
        for(int  group_depth_idx_by8 =0; group_depth_idx_by8 < group_depth_by8 ; group_depth_idx_by8++)
        {
            for(int col=0; col<width_align8; col++)
            {
  
                for(int idx_tile8=0;idx_tile8<8;idx_tile8++)
                {
                    int depth_idx_by8 = group_depth_idx_by8-group_depth_offset_by8;
                    int depth_idx = depth_idx_by8*8+ idx_tile8;

                    if(group_depth_idx_by8<group_depth_offset_by8 || depth_idx_by8>=depth_ceildiv8)
                    {
                        ddr_address+=2;
                        continue;
                    }

                    if(depth_idx < depth && col < width )
                    {
                        int feature_map_address= depth_idx*height*width +row*width +col;
                        char data1= feature_map1[feature_map_address];
                        char data2= feature_map2[feature_map_address];
                        // printf("here %d %d %d\n",ddr_address,data1,data2);
                        featuremap_buffer_hw[ddr_address++]=data1;
                        featuremap_buffer_hw[ddr_address++]=data2;
                    }
                    else
                    {
                        ddr_address+=2;
                    }
                }
            }
        }
    }
}

void apply_scale_factor(FeatureMapBuffer_t &fmap)
{
    for(int i=0;i<fmap.buffers_gold.size();i++)
    {
        std::cout<<"Scale_blob "<<fmap.Scale_blob<<std::endl;
        scale_on<float,float>(
            fmap.buffers_gold[i],
            fmap.buffers_scale[i],
            fmap.blob_info->dim,
            fmap.Scale_blob
        );
        scale_on<float,char>(
            fmap.buffers_gold[i],
            fmap.buffers_int[i],
            fmap.blob_info->dim,
            fmap.Scale_blob
        );

        //TODO: modify and fill in featuremap_int_to_hw

        featuremap_int_to_hw(fmap,0,fmap.group_depth);
    }
}

void del_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
)
{
    std::map< std::string, FeatureMapBuffer_t >::iterator itr;
    for(itr =feature_map_dict.begin(); itr!= feature_map_dict.end();++itr)
    {

        for(int i=0;i< itr->second.buffers_sw.size();i++)
        {
            delete [] itr->second.buffers_sw[i];
        }

        for(int i=0;i< itr->second.buffers_hw.size();i++)
        {
            std::cout<<"feature map name "<< itr->first<<std::endl;
            fflush(stdout);
            FREE( itr->second.buffers_hw[i]);
        }
    
        for(int i=0;i< itr->second.buffers_gold.size();i++)
        {
            delete [] itr->second.buffers_gold[i];
        }

        for(int i=0;i< itr->second.buffers_scale.size();i++)
        {
            delete [] itr->second.buffers_scale[i];
        }

        for(int i=0;i< itr->second.buffers_int.size();i++)
        {
            delete [] itr->second.buffers_int[i];
        }
    }
}
















template<class T>
void free_buffer(std::vector<T*> &vect)
{
    for(int i=0;i<vect.size();i++)
    {
        delete [] vect[i];
    }
}

template<class T>
void free_buffer_hw(std::vector<T*> &vect)
{
    for(int i=0;i<vect.size();i++)
    {
        FREE(vect[i]);
    }
}

void del_weight_buffer_pointer(
    std::vector< LayerInfo_t > & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type == "conv")
        {

            free_buffer<float>(layerinfo_vect[i].weightbuffers_sw);
            free_buffer<float>(layerinfo_vect[i].biasbuffers_sw);
            free_buffer<float>(layerinfo_vect[i].winobuffers_float);
            free_buffer<float>(layerinfo_vect[i].weightbuffers_float_scaled);
            free_buffer<float>(layerinfo_vect[i].bias_float_scaled);
            free_buffer<char>(layerinfo_vect[i].weightbuffer_quant);
            free_buffer<short>(layerinfo_vect[i].biasbuffer_quant);
            FREE(layerinfo_vect[i].weightbuffers_hw[0]);
            free_buffer_hw<char>(layerinfo_vect[i].biasbuffers_hw);
            free_buffer_hw<char>(layerinfo_vect[i].conv_desc_buffer);
            // delete[] layerinfo_vect[i].weightbuffers_noreorder[0];
            // delete[] layerinfo_vect[i].biasbuffers_noreorder[0];

            // FREE(layerinfo_vect[i].wino_weightbuffers_hw[0]);
        }
        else if( layerinfo_vect[i].layer_type == "linear")
        {
            free_buffer<float>(layerinfo_vect[i].weightbuffers_sw);
            free_buffer<float>(layerinfo_vect[i].biasbuffers_sw);

            free_buffer<float>(layerinfo_vect[i].winobuffers_float);

            free_buffer<float>(layerinfo_vect[i].weightbuffers_float_scaled);
            free_buffer<float>(layerinfo_vect[i].bias_float_scaled);
            
            free_buffer<char>(layerinfo_vect[i].weightbuffer_quant);
            free_buffer<short>(layerinfo_vect[i].biasbuffer_quant);
            
            free_buffer_hw<char>(layerinfo_vect[i].weightbuffers_hw);
            free_buffer_hw<char>(layerinfo_vect[i].biasbuffers_hw);
            free_buffer_hw<char>(layerinfo_vect[i].conv_desc_buffer);
            // delete[] layerinfo_vect[i].weightbuffers_noreorder[0];
            // delete[] layerinfo_vect[i].biasbuffers_noreorder[0];
        }
    }
}









// void load_weight_content(
//     std::vector< LayerInfo_t > & layerinfo_vect,
//     std::string filefolder
// )
// {
//     for(int i=0;i<layerinfo_vect.size();i++)
//     {
//         if(layerinfo_vect[i].layer_type == "conv" ||layerinfo_vect[i].layer_type == "linear"  )
//         {
//             int length = layerinfo_vect[i].indim[0] * layerinfo_vect[i].outdim[0] * layerinfo_vect[i].kernel_size * layerinfo_vect[i].kernel_size;
//             std::string filepath=filefolder+layerinfo_vect[i].weightfile;
//             FILE* fptr=fopen(filepath.c_str(),"r");
//             fread(layerinfo_vect[i].weightbuffers_sw[0],sizeof(float),length,fptr );
//             fclose(fptr);
//             filepath=filefolder+layerinfo_vect[i].biasfile;
//             fptr=fopen(filepath.c_str(),"r");
//             fread(layerinfo_vect[i].biasbuffers_sw[0],sizeof(float),layerinfo_vect[i].outdim[0],fptr );
//             fclose(fptr);
//         }
//     }
// }














