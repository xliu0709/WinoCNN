#ifndef _WINO_STRUCT_H_
#define _WINO_STRUCT_H_


typedef struct{
    //Original signal
    int inheight;
    int inwidth;
    int indepth;
    int outheight;
    int outwidth;
    int outdepth;
    int kernel_size;
    int pad_size;
    int stride;

    //wino related
    int wino3x3_flag;
    // 1: 3x3, 0:5x5
    int wino_output_tile_size;

    //input buffer related
    int indepth_align_minitile_size;
    int indepth_align8;
    int indepth_ceildiv8;
    int inwidth_ceildiv_inbufferwidth;
    int inwidth_align8;
    int group_indepth_offset;
    int group_indepth;
    int input_ddr_bytes;
    int input_ddr_128bits;
    int group_indepth_x_inwidth_align8_by8;
    int group_indepth_offset_x_inwidth_align8_by8;
    int input_load_burst_length;
    int buffer_address_mid_increment_step;
    int row_address_bitnumber_flag;
     


    // ouput_buffer_related
    int outwidth_align8;
    int outdepth_align8;
    int outheight_align4;
    int outdepth_align_minitile_size;
    int group_outdepth_offset;
    int group_outdepth;

    int output_ddr_bytes;
    int output_ddr_128bits;


    // Weight_related
    int weightbuffer_load_indepth_number;
    int weightbuffer_load_indepth_step;
    int weightbuffer_load_outdepth_number;
    int weightbuffer_load_outdepth_step;
    int weightbuffer_indepth_minitile_number;
    int weightbuffer_outdepth_minitile_number;
    int weightbuffer_total_load_number;

    //weight_load hardware
    int weightDDR_buffer_burst_length;
    int weightDDR_port_burst_length;
    int weightDDR_burst_number;
    int loop_outdepth_minitile_baseidx_reset_cycle_minus1;
    int loop_start_output_baserowcol_reset_cycle;
    int loop_weight_feed_bound;
   
    // input buffer feeding related
    int wino_out_size_by_wino_width;
    int wino_tile_number_in_outwidth;

    int loop_outdepth_minitile_baseidx_reset_cycle;
    int loop_wino_tile_col_reset_cycle;
    int loop_wino_tile_row_reset_cycle;
    int buffer_address_mid_minitile_depth_step;
    int input_buffer_feeding_loop_bound;  
    int input_transform_feeding_loop_bound;

    // row_tile calculation , these parameter have to be solved after weight parameters are decided.
    int out_rowstep;
    int wino_tile_number_in_out_rowstep;


    // wino computation
    int total_input_stream_tile;
    int loop_omini_base_reset_cycle;
    int loop_wino_cell_bound;
    int loop_wino_tile_rowcol_self_reset_cycle_min1;
    int loop_iload_reset_cycle;
    int outbuffer_oload_increment_step;
    int outbuffer_omini_increment_step;
    
    //output write back 
    int outdepth_ceildiv8;
    int output_burst_length;
    int write_back_flag;
    int wino_col_pix_upper_bound;
    int wino_tile_number_rowcol;
    int out_ddr_increment_step;


    //NEW



    //Merge Kernel
    int merge_kernel_size;
    int merge_kernel_step;
    int merge_kernel_flag;
    int scale_oback_int;
    int merge_weight_row_step;
    int merge_weight_col_step;

    //relu and stride
    int relu_flag;
    // int use_kernel_size;

}ConvDesc_t;

enum misc_enum_t{MAXPOOL=0,LINEAR,ELTWISE};
typedef struct{

    misc_enum_t layer_type;
    

    int iter_bound;


 
    int inheight;
    int inwidth;
    int indepth;
    int outheight;
    int outwidth;
    int outdepth;
    int kernel_size;
    int pad_size;
    int stride;
    
    int inwidth_align8;
    int outwidth_align8;
    
    //max_pool_info
    int depth_tile_size;
    int depth_addr_offset_step; 
    int first_required_rows;
    int in_row_stride_addr_offset_step;
    int in_row_first_required_addr_offset_step;
    int buffer_featuremap_offset_step;

    int in_row_addr_offset_step; // offset

    int depth_tile_cnt;
 
    int update_bound_lvl0;

   
    
    
    //input buffer related
}MiscDesc_t;








    

#endif