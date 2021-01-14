
#ifndef _PARAM_H_
#define _PARAM_H_
#include <string>
#include <vector>
#include "../src/wino_struct.h"

class LayerInfo_t{
    public:
        std::string layer_name;
        std::string layer_type;
        int indim[3];
        int outdim[3];
        int kernel_size;
        int stride;
        int pad_size;
        int group_indepth;
        int group_indepth_offset;
        int group_outdepth;
        int group_outdepth_offset;
        std::vector<std::string > bottoms;
        std::vector<std::string> tops;
        std::string weightfile;
        std::string biasfile;
        int relu_flag;
        ConvDesc_t conv_desc;

        float Scale_w;
        float Scale_wino;
        float Scale_o; //for validation
        float Scale_i; // for validation
        float Scale_b;
        float Scale_oback;
        int Scale_oback_int;
        int use_kernel_size;
        

        std::vector<float *>  weightbuffers_sw;
        std::vector<int> weightbuffers_sw_size;
        std::vector<float *>  biasbuffers_sw;

        std::vector<float *>  winobuffers_float;
        std::vector<int> winobuffers_float_size;

        std::vector<float *> weightbuffers_float_scaled;
        std::vector<float *> bias_float_scaled;
        std::vector<int> weightbuffers_float_scaled_size;


        std::vector<char*> weightbuffer_quant; 
        std::vector<int> weightbuffer_quant_size;
        std::vector<short*> biasbuffer_quant;

    
        
        std::vector<char *>  weightbuffers_hw;
        std::vector<int> weightbuffers_hw_size;
        std::vector<int> port_segment_size;
        std::vector<char *>  conv_desc_buffer;
        std::vector<char *>  biasbuffers_hw;
        
        // std::vector<char *> wino_weightbuffers;
        // std::vector<char *> wino_weightbuffers_hw;
        void Print(void);

};


class BlobInfo_t{
    public:
    std::string blob_name;
    std::string mem_type;
    int dim[3];
    std::vector< std::string > goldfilename;
    void Print(void);
};

void process_element6x6_soft(
    int input_height,
    int input_width,
    int input_depth,
    int output_height,
    int output_width,
    int output_depth,
    int kernel_size,
    int stride_size,
    int pad_size,
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
    ConvDesc_t &conv_desc
);

void load_layer_info(
    std::ifstream &ifs,
    std::vector<LayerInfo_t > & layerinfo_vect
);


void load_blob_info(
    std::ifstream &ifs,
    std::vector<BlobInfo_t > & blobinfo_vect
);

void prepare_conv_descriptor(
    std::vector<LayerInfo_t > & layerinfo_vect
);
void process_maxpool_desc(
    int inheight,
    int inwidth,
    int outheight,
    int outwidth,
    int outdepth,
    int kernel_size,
    int pad_size,
    int stride,
    MiscDesc_t & misc_desc);
    
#endif