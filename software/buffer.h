#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "param.h"
#include "../src/wino_struct.h"
#include <vector>
#include <map>
#include <cmath>
#include <assert.h>
void feature_to_ddr(
    char* feature_map1,
    char* feature_map2,
    ConvDesc_t &conv_desc,
    char* &input_DDR,
    char* &output_DDR
);
void ddr_to_featuremap(char* feature_map1, char* feature_map2, char* DDR, ConvDesc_t &conv_desc);
char* wino6x6_weight_to_ddr(char* weight,ConvDesc_t conv_desc);
char* fc_weight_to_ddr(char* weight,short* bias,int indepth,int outdepth);
void weight_to_ddr(
    char* weight,
    char* weight_wino,
    ConvDesc_t conv_desc
);

class FeatureMapBuffer_t{
    public:
        BlobInfo_t* blob_info;
        int byte_size_sw;
        int buffer_size_sw;
        int byte_size_int;
        int buffer_size_int;
        int byte_size_hw;
        int group_depth;
        std::vector<int> group_depth_offset;
        float Scale_blob;
        std::vector< float* > buffers_gold;
        std::vector< float* > buffers_sw;
        std::vector< float* > buffers_scale;
        std::vector< char* > buffers_int;
        std::vector< char* > buffers_hw;
};


template<class T1, class T2>
void scale_on(T1* buffer_in, T2* buffer_out, int dim[3], float scale_factor)
{

        std::cout<<"Applying Scale Factor ["<<scale_factor <<"]"<<std::endl;
        for(int i=0;i<dim[0]*dim[1]*dim[2];i++)
        {
            buffer_out[i]=round(buffer_in[i]*scale_factor);
        }
}



template<class T1,class T2>
void scale_off(T1* buffer_in, T2* buffer_out, int dim[3], float scale_factor)
{

        std::cout<<"Removing Scale Factor ["<<scale_factor <<"]"<<std::endl;

        for(int i=0;i<dim[0]*dim[1]*dim[2];i++)
        {
            buffer_out[i]=buffer_in[i]/scale_factor;
        }
}


template<class Tin, class Tout>
void scale_data(Tin* data_buffer, Tout* data, float scale_factor, int size, int bit)
{
    for(int i=0;i<size;i++)
    {
        int result =round(data_buffer[i]*scale_factor);
        assert( result<=((1<<bit)-1) && result>( -1<<bit) );
        data[i]=result;
    }
}

template<class intbuffer_t>
void feature_map_float_to_int(
    float* float_buffer,
    intbuffer_t* char_buffer,
    int size,
    int qbit,
    float Scale_blob
)
{
    std::cout<<"Scale Blob "<<Scale_blob<< std::endl;
    std::cout<<"Quant bit"<<qbit<< std::endl;
    fflush(stdout);
    for(int i=0;i<size;i++)
    {
        intbuffer_t result=round( ((float) (float_buffer[i]))*Scale_blob);
        char_buffer[i]=result;
    }
}

void feature_map_init(
    FeatureMapBuffer_t & featuremap,
    std::string style
);

void initiate_featuremap_dict(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
);
void alloc_float_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
);

void del_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
);

void load_float_golden_featuremap(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
);
void load_img_featuremap(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
);

void alloc_float_weight_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect,
    std::string mode
);

void weight_pretransform_float(
    std::vector< LayerInfo_t > & layerinfo_vect
);

void scale_weight_bias_data_float(
    std::vector< LayerInfo_t > & layerinfo_vect
);

void weight_preprocess_quant(
    std::vector< LayerInfo_t > & layerinfo_vect
);

void alloc_int_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< BlobInfo_t > & blobinfo_vect
);
void alloc_hw_featuremap_mem_single_blob(
    FeatureMapBuffer_t & featuremap
);
void alloc_hw_featuremap_mem(
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict,
    std::vector< LayerInfo_t > & layerinfo_vect
);
void apply_scale_factor(FeatureMapBuffer_t &fmap);
void featuremap_hw_to_int_pointers(
    char* sourceDDR,
    char* target0,
    char* target1,
    int height,
    int width,
    int depth,
    int group_depth_offset,
    int group_depth
);
void featuremap_int_to_hw_pointers(
    char* source0,
    char* source1,
    char* targetDDR,
    int height,
    int width,
    int depth,
    int group_depth_offset,
    int group_depth
);
void featuremap_int_to_hw(
    FeatureMapBuffer_t & featuremap,
    int group_depth_offset,
    int group_depth
);

void featuremap_int_to_hw_from_pointer(
    char* resource0,
    char* resource1,
    FeatureMapBuffer_t & featuremap,
    int group_depth_offset,
    int group_depth
);

void alloc_convdesc_and_bias_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect
);

void alloc_hw_weight_buffer(
    std::vector< LayerInfo_t > & layerinfo_vect
);

void process_hw_weight_buffer_single_layer(
    LayerInfo_t & layerinfo
);

void process_hw_weight_buffer(
    std::vector<LayerInfo_t> & layerinfo_vect
);

void init_weight(
    char* weight,
    int indepth,
    int outdepth,
    int kernelsize,
   std::string mode 
);
void init_bias_int(
    short* bias,
    int length,
    std::string mode);
void init_weight_float(
    float* weight,
    int indepth,
    int outdepth,
    int kernelsize,
   std::string mode 
);

void del_weight_buffer_pointer(
    std::vector< LayerInfo_t > & layerinfo_vect
);
void compute_scale_factors(
    std::vector< LayerInfo_t > & layerinfo_vect,
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict
);

template<class T>
void feature_map_buffer_init(
    T* buffer,
    int depth,
    int height,
    int width,
    std::string style
)
{
    int address=0;
    for(int d=0;d<depth;d++)
    for(int h=0;h<height;h++)
    for(int w=0;w<width;w++)
    {

        if(style=="random")
        {
            buffer[address]=random()%256;
        }
        else if(style=="order")
        {
            buffer[address]=h*width+w;
            // featuremap.buffers_int[0][address]=1;
            // featuremap.buffers_int[1][address]=1;
        }
        else
        {
            assert(0);
        }
        address++;
    }
}

template<class T>
void init_weight(
    T* weight,
    int indepth,
    int outdepth,
    int kernelsize_h,
    int kernelsize_w,
   std::string mode 
)
{
    std::cout<<"init weight"<<std::endl;
    fflush(stdout);
    for(int id=0;id<indepth;id++)
    for(int od=0;od<outdepth;od++)
    for(int ks=0;ks<kernelsize_h*kernelsize_w;ks++)
    {
        int value;
        if(mode=="depth_order")
        {
            value=id*outdepth+od;
            // value=od;
        }
        else if(mode=="idepth_order")
        {
            value=id;
            // value=od;
        }
        else if(mode=="odepth_order")
        {
            value=od%64;
            // value=od;
        }
        else if(mode=="kernel_order")
        {
            // value=ks;
            value=1;
            // if(ks==kernelsize*kernelsize/2 && od%16<8 && id==0) value=1;
            // else value=0;
        }
        else if( mode == "center")
        {
            if(ks==kernelsize_w*kernelsize_h/2 && id==0 && od%16<8) value=1;
            else value=0;
        }
        else if(mode=="random")
        {
            value=random()%128-64;
        }
        else if(mode=="zero")
        {
            value=0;
        }
        else
        {
            assert(0);
        }
        
        weight[ (od*indepth+id)*kernelsize_h*kernelsize_w+ks]=value;
    }
}

#endif