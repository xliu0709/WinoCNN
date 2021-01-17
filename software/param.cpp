#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "../src/wino_struct.h"
#include "../src/wino_macro.h"
#include "../src/misc.h"
#include "param.h"


void find_factor(int number,std::vector<int> & factor_list)
{
    for(int i=1;i<number+1;i++)
    {
        if(number % i ==0)
            factor_list.push_back(i);
    }
}


int find_max_factor_product(std::vector<int> & factor_list1, std::vector<int> & factor_list2, int capacity, int & factor1, int & factor2)
{
    if(factor_list2.size()==0 || factor_list2.size()==0 ) return -1;

    if(factor_list2[0]*factor_list2[0]>capacity ) return -1;

    int index1=0;
    int index2=factor_list2.size()-1;

    int product_candidate=0;
    int factor1_candidate_index;
    int factor2_candidate_index;

 

    while(1)
    {
        if(index1 == factor_list1.size() ) break;
        int product = factor_list1[index1]*factor_list2[index2];
        if(product > capacity)
        {
            index2--;
            continue;
        }

        if(product > product_candidate)
        {
            product_candidate=product;
            factor1_candidate_index=index1;
            factor2_candidate_index=index2;
        }

        index1++;        
    }
    factor1 = factor_list1[factor1_candidate_index];
    factor2 = factor_list2[factor2_candidate_index];

    return product_candidate;

}


int log2ceil(int value)
{
    int bit=0;
    while(value >(1<<bit) )
    {
        printf("%d %d\n", value,bit);
        bit++;
    }
    return bit;

}

void process_element6x6_soft(
    int input_height,
    int input_width,
    int input_depth,
    int output_height,
    int output_width,
    int output_depth,
    int kernel_size_h,
    int kernel_size_w,
    int stride_size,
    int pad_size_h,
    int pad_size_w,
    int group_indepth_offset,
    int group_indepth,
    int group_outdepth_offset,
    int group_outdepth,
    int relu_flag,
    int scale_oback,
    int wino_domain_size,
    int weightddr_indepth_minitile_128bit_step,
    int input_buffer_depth_bitwidth,
    int outdepth_minitile_size,
    int indepth_minitile_size,
    int weight_buffer_depth,
    int output_buffer_depth,
    int input_buffer_depth,
    int inbuffer_height,
    int inbuffer_width,
    int wino_width,
    int use_kernel_size,
    ConvDesc_t &conv_desc
)
{
   

    // if(kernel_size==1)
    //     use_kernel_size=1;
    // else
    //     use_kernel_size=3;

    memset(&conv_desc,0xAA,sizeof(conv_desc));
    // oringal parameters
    conv_desc.inwidth = input_width;
    conv_desc.inheight = input_height;
    conv_desc.indepth = input_depth;
    conv_desc.outwidth = output_width*stride_size;
    conv_desc.outheight = output_height*stride_size;
    conv_desc.outdepth = output_depth;
    conv_desc.kernel_size_h = kernel_size_h;
    conv_desc.kernel_size_w = kernel_size_w;
    conv_desc.stride= stride_size;
    conv_desc.pad_size_h = pad_size_h;
    conv_desc.pad_size_w = pad_size_w;
    // conv_desc.use_kernel_size=use_kernel_size;

    // wino related parameters
    if(wino_domain_size==6)
    {
        // if( kernel_size == 3)
        // {
        //     conv_desc.wino3x3_flag = 1;
        //     conv_desc.wino_output_tile_size = 4;
        //     conv_desc.merge_kernel_flag=0;
        //     conv_desc.merge_kernel_size=ALIGN(3,3);
        //     conv_desc.merge_kernel_step=3;
        // }
        // else if( kernel_size == 5)
        // {
        //     conv_desc.wino3x3_flag = 0;
        //     conv_desc.wino_output_tile_size =2;
        //     conv_desc.merge_kernel_flag=0;
        //     conv_desc.merge_kernel_size=3;
        //     conv_desc.merge_kernel_step=3;
        // }
        // else
        // {
        //     conv_desc.wino3x3_flag = 1;
        //     conv_desc.wino_output_tile_size = 4;
        //     conv_desc.merge_kernel_flag=1;
        //     conv_desc.merge_kernel_size=ALIGN(kernel_size,3);
        //     conv_desc.merge_kernel_step=3;
        // }
    }
    else
    {
        if( use_kernel_size==3 )
        {
            conv_desc.wino3x3_flag = 1;
            conv_desc.wino_output_tile_size = 2;
            conv_desc.merge_kernel_size_h=ALIGN(kernel_size_h,3);
            conv_desc.merge_kernel_size_w=ALIGN(kernel_size_w,3);
            conv_desc.merge_kernel_step=3;

            if(kernel_size_h<=3)
            {
                conv_desc.merge_kernel_flag=0;
            }
            else
            {
                conv_desc.merge_kernel_flag=1;
            }
        }
        else
        {
            conv_desc.wino3x3_flag = 0;
            conv_desc.wino_output_tile_size = 4;
            conv_desc.merge_kernel_size_h=kernel_size_h;
            conv_desc.merge_kernel_size_w=kernel_size_w;
            conv_desc.merge_kernel_step=1;

            if(kernel_size_h==1)
            {
                conv_desc.merge_kernel_flag=0;
            }
            else
            {
                conv_desc.merge_kernel_flag=1;
            }
        }
    }
    //input buffer related

    conv_desc.indepth_align8 = ALIGN(input_depth,8);
    conv_desc.indepth_align_minitile_size = ALIGN(input_depth,indepth_minitile_size);
    conv_desc.inwidth_ceildiv_inbufferwidth = CEIL_DIV(input_width,inbuffer_width);
    conv_desc.inwidth_align8 = ALIGN(input_width,8);
    conv_desc.group_indepth = group_indepth;
    conv_desc.group_indepth_offset = group_indepth_offset;
    
    conv_desc.outwidth_align8 = ALIGN(output_width,8)*stride_size;

    conv_desc.wino_tile_number_in_outwidth = CEIL_DIV(conv_desc.outwidth, conv_desc.wino_output_tile_size * wino_width);

    // check the minimum address segment that can be contained in the input buffer
    int row_occupy_address = conv_desc.inwidth_ceildiv_inbufferwidth*conv_desc.indepth_align8;
    int row_occupy_address_bits = log2ceil(row_occupy_address);
    std::cout<<"conv_desc.inwidth_ceildiv_inbufferwidth "<<conv_desc.inwidth_ceildiv_inbufferwidth<<std::endl;
    std::cout<<"conv_desc.indepth_align8 "<<conv_desc.indepth_align8<<std::endl;
    printf("row_occupy_address/bit %d %d\n ",row_occupy_address,row_occupy_address_bits );

    int max_row_bit = input_buffer_depth_bitwidth - row_occupy_address_bits;

    int row_bit;

    printf("INFO:depth=[%d x %d], real depth [%d],max row bit %d \n", conv_desc.inwidth_ceildiv_inbufferwidth, conv_desc.indepth_align8, input_buffer_depth, max_row_bit);
       
    if(max_row_bit <1)
    {
        printf("ERROR: in sufficient IN BUFFER DEPTH, expected depth=[%d x %d], reald depth [%d]\n", conv_desc.inwidth_ceildiv_inbufferwidth, conv_desc.indepth_align8, input_buffer_depth);
        exit(-1);
    }
    else if( max_row_bit >= 2)
    {
        row_bit=2;
        conv_desc.row_address_bitnumber_flag=0;
        printf("row address set to 0\n");
    }
    else
    {
        row_bit=1;
        conv_desc.row_address_bitnumber_flag=1;
        printf("row address set to 1\n");
    }

    int input_buffer_row_capacity = inbuffer_height *(1<<row_bit);

    conv_desc.input_ddr_bytes = conv_desc.group_indepth * conv_desc.inwidth_align8 * conv_desc.inheight * 2;
    conv_desc.input_ddr_128bits = conv_desc.group_indepth * conv_desc.inwidth_align8 * conv_desc.inheight / 8;
    conv_desc.group_indepth_x_inwidth_align8_by8 = conv_desc.group_indepth * conv_desc.inwidth_align8 /8;
    conv_desc.group_indepth_offset_x_inwidth_align8_by8 = conv_desc.group_indepth_offset * conv_desc.inwidth_align8 /8;

    conv_desc.input_load_burst_length = conv_desc.indepth_align8 * conv_desc.inwidth_align8/8;
    conv_desc.buffer_address_mid_increment_step = conv_desc.inwidth_ceildiv_inbufferwidth*(8-indepth_minitile_size)/indepth_minitile_size +1 ;
    
    





    //out buffer related
    conv_desc.outdepth_align8 = ALIGN(output_depth,8);
    conv_desc.outheight_align4 = ALIGN(output_height*stride_size,4);
    conv_desc.outdepth_align_minitile_size = ALIGN(output_depth, outdepth_minitile_size);
    conv_desc.group_outdepth=group_outdepth;
    conv_desc.group_outdepth_offset=group_outdepth_offset;
    conv_desc.output_ddr_bytes = conv_desc.group_outdepth * conv_desc.outwidth_align8 * conv_desc.outheight*2;
    conv_desc.output_ddr_128bits = conv_desc.group_outdepth * conv_desc.outwidth_align8 * conv_desc.outheight/8;

 
   

    // weight DDR related parameter
    int total_outdepth_minitile_number = CEIL_DIV(conv_desc.outdepth_align8,outdepth_minitile_size);
    int total_indepth_minitile_number = CEIL_DIV(conv_desc.indepth_align8,indepth_minitile_size);

    

    int weight_capacity = weight_buffer_depth/2; 
    std::vector<int> indepth_factor_list;
    std::vector<int> outdepth_factor_list;

    find_factor(total_outdepth_minitile_number,outdepth_factor_list);
    find_factor(total_indepth_minitile_number,indepth_factor_list);

    int indepth_factor,outdepth_factor;
    find_max_factor_product(indepth_factor_list,outdepth_factor_list,weight_capacity,indepth_factor,outdepth_factor);
    
    std::cout<<"indepth_factor "<<indepth_factor<<std::endl;
    std::cout<<"outdepth_factor "<<outdepth_factor<<std::endl;

    conv_desc.weightbuffer_indepth_minitile_number = indepth_factor;
    conv_desc.weightbuffer_load_indepth_step = indepth_factor * indepth_minitile_size;
    conv_desc.weightbuffer_load_indepth_number = conv_desc.indepth_align8 / conv_desc.weightbuffer_load_indepth_step;
    
    conv_desc.weightbuffer_outdepth_minitile_number = outdepth_factor;
    conv_desc.weightbuffer_load_outdepth_step = outdepth_factor*outdepth_minitile_size;
    conv_desc.weightbuffer_load_outdepth_number = conv_desc.outdepth_align8 / conv_desc.weightbuffer_load_outdepth_step;

    int weightbuffer_load_size=indepth_factor*outdepth_factor;
    if(kernel_size_h==5 && kernel_size_w && wino_domain_size==6)
        conv_desc.weightDDR_buffer_burst_length = weightddr_indepth_minitile_128bit_step*weightbuffer_load_size;
    else
    {
        conv_desc.weightDDR_buffer_burst_length = (indepth_minitile_size/2)*weightbuffer_load_size;
    }





    conv_desc.weightDDR_port_burst_length = (outdepth_minitile_size/WEIGHT_PORT_NUM)*conv_desc.weightDDR_buffer_burst_length;
    conv_desc.weightDDR_burst_number = conv_desc.weightbuffer_load_indepth_number * conv_desc.weightbuffer_load_outdepth_number;


    // row_tile parameter.

    int expected_row_step =CEIL_DIV(weightddr_indepth_minitile_128bit_step * outdepth_minitile_size/WEIGHT_PORT_NUM*5, conv_desc.wino_tile_number_in_outwidth*4)*conv_desc.wino_output_tile_size;
    printf("load cycle %d\n", weightddr_indepth_minitile_128bit_step * (outdepth_minitile_size/4));
    printf("Repeat tile in row_tile %d\n", conv_desc.wino_tile_number_in_outwidth);

    expected_row_step=ALIGN(expected_row_step,conv_desc.wino_output_tile_size);
    
    int maximum_row_step_input =(input_buffer_row_capacity+1-conv_desc.merge_kernel_step)/2;
    
    int outdepth_minitile_number = conv_desc.weightbuffer_outdepth_minitile_number*conv_desc.weightbuffer_load_outdepth_number;
    int maximum_row_step_output = output_buffer_depth / (outdepth_minitile_number * conv_desc.wino_tile_number_in_outwidth) * conv_desc.wino_output_tile_size;                               
    printf("maximum_row_step_input %d\n",maximum_row_step_input);
    printf("maximum_row_step_output %d\n",maximum_row_step_output);
    printf("expected_row_step %d\n",expected_row_step);
    
    
    
    

    int maximum_row_step = maximum_row_step_output < maximum_row_step_input ? maximum_row_step_output : maximum_row_step_input;
    assert( maximum_row_step >= conv_desc.wino_output_tile_size);
    int row_step;
    if(expected_row_step <= maximum_row_step )
    {
        row_step=expected_row_step<output_height*stride_size?expected_row_step:output_height*stride_size;
    }
    else if(input_buffer_row_capacity >= input_height )
    {
        row_step=ALIGN(output_height*stride_size,4);
    }
    else
    {
        row_step=maximum_row_step/conv_desc.wino_output_tile_size*conv_desc.wino_output_tile_size;
    }
    printf("row_step %d\n",row_step );

    conv_desc.out_rowstep=row_step;
    conv_desc.wino_tile_number_in_out_rowstep=row_step/conv_desc.wino_output_tile_size;



    //input buffer feeding related
    conv_desc.loop_wino_tile_col_reset_cycle =  indepth_minitile_size
                                                *conv_desc.wino_tile_number_in_outwidth;
    conv_desc.loop_wino_tile_row_reset_cycle =  conv_desc.loop_wino_tile_col_reset_cycle 
                                                *conv_desc.wino_tile_number_in_out_rowstep;
    conv_desc.input_buffer_feeding_loop_bound = conv_desc.weightbuffer_load_indepth_number*conv_desc.weightbuffer_indepth_minitile_number 
                                                *conv_desc.loop_wino_tile_row_reset_cycle;
    
    conv_desc.buffer_address_mid_minitile_depth_step = conv_desc.inwidth_ceildiv_inbufferwidth;
    conv_desc.wino_out_size_by_wino_width=conv_desc.wino_output_tile_size*wino_width;
    
    conv_desc.input_transform_feeding_loop_bound = conv_desc.input_buffer_feeding_loop_bound*
                                                    conv_desc.weightbuffer_load_outdepth_number;


    //wino kernel related
    conv_desc.loop_omini_base_reset_cycle = conv_desc.weightbuffer_outdepth_minitile_number>indepth_minitile_size? conv_desc.weightbuffer_outdepth_minitile_number:indepth_minitile_size;
	conv_desc.total_input_stream_tile=  conv_desc.weightbuffer_load_outdepth_number*
                                        conv_desc.weightbuffer_load_indepth_number*
                                        conv_desc.weightbuffer_indepth_minitile_number*
                                        conv_desc.wino_tile_number_in_outwidth*
                                        conv_desc.wino_tile_number_in_out_rowstep;
    conv_desc.loop_wino_tile_rowcol_self_reset_cycle_min1 = conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth-1;

    conv_desc.loop_iload_reset_cycle =  conv_desc.weightbuffer_load_indepth_number*
										conv_desc.weightbuffer_indepth_minitile_number*
										conv_desc.wino_tile_number_in_out_rowstep*
										conv_desc.wino_tile_number_in_outwidth*
										conv_desc.loop_omini_base_reset_cycle;

    conv_desc.loop_wino_cell_bound =    conv_desc.weightbuffer_load_outdepth_number *
                                        conv_desc.weightbuffer_load_indepth_number*
                                        conv_desc.weightbuffer_indepth_minitile_number*
                                        conv_desc.wino_tile_number_in_out_rowstep*
                                        conv_desc.wino_tile_number_in_outwidth*
                                        conv_desc.loop_omini_base_reset_cycle;
    
    conv_desc.outbuffer_oload_increment_step =  conv_desc.wino_tile_number_in_out_rowstep*
                                                conv_desc.wino_tile_number_in_outwidth*
                                                conv_desc.weightbuffer_outdepth_minitile_number;
    
    conv_desc.outbuffer_omini_increment_step =  conv_desc.wino_tile_number_in_out_rowstep*
                                                conv_desc.wino_tile_number_in_outwidth;
                                


    //weight buffer feeding related
    conv_desc.loop_outdepth_minitile_baseidx_reset_cycle_minus1=conv_desc.weightbuffer_outdepth_minitile_number-1;
    conv_desc.loop_start_output_baserowcol_reset_cycle= conv_desc.weightbuffer_outdepth_minitile_number * 
                                                        conv_desc.wino_tile_number_in_outwidth *
                                                        conv_desc.wino_tile_number_in_out_rowstep;
    conv_desc.loop_weight_feed_bound =  conv_desc.weightbuffer_indepth_minitile_number * 
                                        conv_desc.loop_start_output_baserowcol_reset_cycle;

    //out buffer related
    conv_desc.outdepth_ceildiv8 = CEIL_DIV(conv_desc.outdepth, 8);
    conv_desc.output_burst_length = conv_desc.outwidth_align8*CEIL_DIV(conv_desc.outdepth, OUT_PORT_BATCH_NUM)/stride_size;
    printf("conv_desc.output_burst_length %d\n",conv_desc.output_burst_length);
    
    fflush(stdout);

    //output write back related

    conv_desc.wino_col_pix_upper_bound= conv_desc.wino_output_tile_size-stride_size;
    conv_desc.wino_tile_number_rowcol=conv_desc.wino_tile_number_in_out_rowstep*conv_desc.wino_tile_number_in_outwidth;
	conv_desc.out_ddr_increment_step=conv_desc.output_burst_length*conv_desc.wino_output_tile_size/conv_desc.stride;


    conv_desc.scale_oback_int=scale_oback;
    std::cout<<"scale_oback "<< scale_oback<<std::endl;

    conv_desc.merge_weight_row_step=conv_desc.merge_kernel_size_w/conv_desc.merge_kernel_step*conv_desc.weightDDR_port_burst_length*conv_desc.weightDDR_burst_number*4;
    conv_desc.merge_weight_col_step=conv_desc.weightDDR_port_burst_length*conv_desc.weightDDR_burst_number*4;

    conv_desc.relu_flag=relu_flag;
    
}


void LayerInfo_t::Print()
{
    std::cout<<"layer_name:"<<layer_name<<std::endl;
    std::cout<<"layer_type:"<<layer_type<<std::endl;
    std::cout<<"weightfile:"<<weightfile<<std::endl;
    std::cout<<"biasfile:"<<biasfile<<std::endl;
    std::cout<<std::endl;
}

void BlobInfo_t::Print()
{
    std::cout<<"layer_name:"<<blob_name<<std::endl;
    std::cout<<"layer_type:"<<mem_type<<std::endl;
    std::cout<<std::endl;
}

void load_layer_info(
    std::ifstream &ifs,
    std::vector<LayerInfo_t > & layerinfo_vect
)
{
    std::cout<<"*** loading layer info**"<<std::endl;
    fflush(stdout);
    std::string tmp_string;
    
    while(tmp_string != "relu_flag")
    {
        ifs>>tmp_string;
        std::cout<<tmp_string<<"\t";
    }
    fflush(stdout);

    int cnt=0;
    while(!ifs.eof())
    {
        LayerInfo_t layer_info;
        ifs>>layer_info.layer_name;
        
        if(layer_info.layer_name.length()==0)
            break;
        ifs>>layer_info.layer_type;

        ifs>>layer_info.indim[0];
        ifs>>layer_info.indim[1];
        ifs>>layer_info.indim[2];

        ifs>>layer_info.outdim[0];
        ifs>>layer_info.outdim[1];
        ifs>>layer_info.outdim[2];

        ifs>>layer_info.kernel_size_h;
        ifs>>layer_info.kernel_size_w;
        ifs>>layer_info.stride;
        ifs>>layer_info.pad_size_h;
        ifs>>layer_info.pad_size_w;

        ifs>>layer_info.group_indepth;
        ifs>>layer_info.group_indepth_offset;
        
        ifs>>layer_info.group_outdepth;
        ifs>>layer_info.group_outdepth_offset;

        printf(" layer_info.group_indepth_offset %d\n",layer_info.group_indepth_offset);
        printf(" layer_info.group_outdepth_offset %d\n",layer_info.group_outdepth_offset);
        
        // if(layer_info.layer_type == "conv")
        // {
        //     process_element6x6(
        //         layer_info.indim[0],
        //         layer_info.indim[1],
        //         layer_info.indim[2],
        //         layer_info.outdim[0],
        //         layer_info.outdim[1],
        //         layer_info.outdim[2],
        //         layer_info.kernel_size,
        //         layer_info.stride,
        //         layer_info.pad_size,
        //         layer_info.group_indepth_offset,
        //         layer_info.group_indepth,
        //         layer_info.group_outdepth_offset,
        //         layer_info.group_outdepth,
        //         layer_info.conv_desc
        //     );
        // }
        

        std::size_t start,end;

        ifs>>tmp_string;
        start=0;


        while(start<tmp_string.length())
        {
            std::string blobname;
            end=tmp_string.find(",",start+1);
            blobname.assign(tmp_string,start,end-start);
            layer_info.bottoms.push_back(blobname);
            start=end+1;
        }


        ifs>>tmp_string;
        start=0;


        while(start<tmp_string.length())
        {
            std::string blobname;
            end=tmp_string.find(",",start+1);
            blobname.assign(tmp_string,start,end-start);
            layer_info.tops.push_back(blobname);
            start=end+1;
        }
  

        ifs>>layer_info.weightfile;
        ifs>>layer_info.biasfile;
        ifs>>layer_info.relu_flag;
        layerinfo_vect.push_back(layer_info);
        std::cout<<"layer name:" <<layer_info.layer_name<<" relu "<<layer_info.relu_flag<<std::endl;
        cnt++;
    }
    std::cout<<cnt<<std::endl;
    std::cout<<"*** loading layer info end**"<<std::endl;
    fflush(stdout);
}


void load_blob_info(
    std::ifstream &ifs,
    std::vector<BlobInfo_t > & blobinfo_vect
)
{
    std::cout<<"*** loading blob info**"<<std::endl;
    fflush(stdout);
    std::string tmp_string;
    std::getline(ifs,tmp_string,'\n' );
    std::cout<<tmp_string<<std::endl;

    
    int cnt=0;
    while(!ifs.eof())
    {
        // getchar();
        BlobInfo_t blob_info;
        ifs>>blob_info.blob_name;
        if(blob_info.blob_name.length()==0)
            break;
        // ifs>>blob_info.mem_type;

        ifs>>blob_info.dim[0];
        ifs>>blob_info.dim[1];
        ifs>>blob_info.dim[2];

        std::size_t start,end;

        ifs>>tmp_string;
        start=0;

        while(start<tmp_string.length())
        {
            std::string blobname;
            end=tmp_string.find(",",start+1);
            blobname.assign(tmp_string,start,end-start);
            std::cout<<blobname<<std::endl;
            if(blobname != "None")
                blob_info.goldfilename.push_back(blobname);
            start=end+1;
        }
        blob_info.Print();
        blobinfo_vect.push_back(blob_info);
        cnt++;
    }
    std::cout<<cnt<<std::endl;
    std::cout<<"*** loading blob info end**"<<std::endl;
    fflush(stdout);
}


void 
prepare_conv_descriptor(
    std::vector<LayerInfo_t > & layerinfo_vect
)
{
    for(int i=0;i<layerinfo_vect.size();i++)
    {
        if(layerinfo_vect[i].layer_type=="conv")
        {
            process_element6x6_soft(
                layerinfo_vect[i].indim[1],
                layerinfo_vect[i].indim[2],
                layerinfo_vect[i].indim[0],
                layerinfo_vect[i].outdim[1],
                layerinfo_vect[i].outdim[2],
                layerinfo_vect[i].outdim[0],
                layerinfo_vect[i].kernel_size_h,
                layerinfo_vect[i].kernel_size_w,
                layerinfo_vect[i].stride,
                layerinfo_vect[i].pad_size_h,
                layerinfo_vect[i].pad_size_w,
                layerinfo_vect[i].group_indepth_offset,
                layerinfo_vect[i].group_indepth,
                layerinfo_vect[i].group_outdepth_offset,
                layerinfo_vect[i].group_outdepth,
                layerinfo_vect[i].relu_flag,
                layerinfo_vect[i].Scale_oback_int,
                WINO_DOMAIN_SIZE,
                WEIGHTDDR_INDEPTH_MINITILE_128BIT_STEP,
                INPUT_BUFFER_DEPTH_BITWIDTH,
                OUTDEPTH_MINITILE_SIZE,
                INDEPTH_MINITILE_SIZE,
                WEIGHT_BUFFER_DEPTH,
                OUTPUT_BUFFER_DEPTH,
                INPUT_BUFFER_DEPTH,
                INBUFFER_HEIGHT,
                INBUFFER_WIDTH,
                WINO_WIDTH,
                layerinfo_vect[i].use_kernel_size,
                layerinfo_vect[i].conv_desc
            );
            std::cout<<"conv_desc.outdepth_align8 "<<layerinfo_vect[i].conv_desc.outdepth_align8<<std::endl; 
            fflush(stdout);
        }
    }
}


void process_maxpool_desc(
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int outdepth,
    int kernel_size,
    int pad_size,
    int stride,
    MiscDesc_t & misc_desc)
{
    misc_desc.layer_type=MAXPOOL;
    misc_desc.inwidth=inwidth;
    misc_desc.inheight=inheight;
    misc_desc.outwidth=outwidth;
    misc_desc.outheight=outheight;

    misc_desc.kernel_size=kernel_size;
    misc_desc.outdepth=outdepth;
    misc_desc.pad_size=pad_size;
    misc_desc.stride=stride;


    int inwidth_align8=ALIGN(inwidth,8);
    misc_desc.inwidth_align8=inwidth_align8;
    int outwidth_align8=ALIGN(outwidth,8);
    misc_desc.outwidth_align8=outwidth_align8;

    int max_row_requirement=kernel_size+stride>inheight?inheight: kernel_size+stride;

    int depth_tile_capacity = MISC_INBUFF_DEPTH/(inwidth_align8*max_row_requirement);

    int depth_tile_featuremap = CEIL_DIV(outdepth,16);
    
    int depth_tile_size = depth_tile_capacity > depth_tile_featuremap? depth_tile_featuremap:depth_tile_capacity;
    std::cout<<depth_tile_capacity<<std::endl;
    std::cout<<depth_tile_featuremap<<std::endl;
    std::cout<<depth_tile_size<<std::endl;

    int depth_tile_numebr = CEIL_DIV(outdepth,depth_tile_size*16);

    std::cout<<depth_tile_numebr<<std::endl;

    int depth_addr_offset_step =  depth_tile_size*inwidth_align8; //incrementing after each depth tile's computation
    
    int in_row_stride_addr_offset_step = inwidth_align8*CEIL_DIV(outdepth,8)*stride; // incrementing after each row's computation
    
   
    
    int in_row_addr_offset_step = inwidth_align8*CEIL_DIV(outdepth,8);

    int first_required_rows = kernel_size-pad_size>inheight?inheight:kernel_size-pad_size;
    
    int in_row_first_required_addr_offset_step = inwidth_align8*CEIL_DIV(outdepth,8)*first_required_rows;
    
    int update_depth_tile_bound = outheight-1; // when update_depth_tile counter reaches update_depth_tile_bound, 
    int iter_bound = outheight*depth_tile_numebr;

    int buffer_featuremap_offset_step=inheight*depth_tile_size*inwidth_align8;

    misc_desc.depth_tile_size=depth_tile_size;    
    misc_desc.iter_bound= iter_bound;
    misc_desc.update_bound_lvl0= update_depth_tile_bound;
    misc_desc.depth_addr_offset_step=depth_addr_offset_step;
    misc_desc.first_required_rows=first_required_rows;    
    misc_desc.in_row_addr_offset_step=in_row_addr_offset_step;
    misc_desc.in_row_stride_addr_offset_step=in_row_stride_addr_offset_step;
    misc_desc.in_row_first_required_addr_offset_step=in_row_first_required_addr_offset_step;
    misc_desc.buffer_featuremap_offset_step=buffer_featuremap_offset_step;


}