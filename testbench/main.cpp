#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <fstream>

#include <string>
#include "../src_gold/wino_gold.h"
#include "../software/buffer.h"

#include "debug.hpp"
#include "../src/wino.h"
#include "../src/wino_struct.h"
#include "../src/wino_macro.h"
#include "../src/misc.h"
#include "../software/param.h"
#include "../software/softlayer.hpp"
#include "../software/exec.h"
#include <ap_int.h>






int main(int argc, char** argv)
{
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);
    std::ifstream layer_file_stream(argv[1],std::ifstream::in);
    std::ifstream blob_file_stream(argv[2],std::ifstream::in);

    std::string filepath(argv[3]);

    std::vector<LayerInfo_t > layerinfo_vect;
    std::vector<BlobInfo_t > blobinfo_vect;

    load_layer_info(layer_file_stream, layerinfo_vect);
    
    load_blob_info(blob_file_stream, blobinfo_vect);  

    std::map< std::string, FeatureMapBuffer_t >  feature_map_dict; 



    initiate_featuremap_dict(feature_map_dict, blobinfo_vect); 
    alloc_float_featuremap_mem(feature_map_dict, blobinfo_vect); 


    load_float_golden_featuremap(feature_map_dict);

    load_img_featuremap(feature_map_dict);

    alloc_float_weight_buffer(layerinfo_vect,"random");   
    //TODO: implement  wino_weight_transform function
    weight_pretransform_float(layerinfo_vect);

    compute_scale_factors(layerinfo_vect,feature_map_dict);



    scale_weight_bias_data_float(layerinfo_vect);
    weight_preprocess_quant(layerinfo_vect);
    alloc_int_featuremap_mem(feature_map_dict, blobinfo_vect); 
    prepare_conv_descriptor(layerinfo_vect);
    alloc_hw_featuremap_mem(feature_map_dict,layerinfo_vect);
    //alloc_hw_conv_desc_mem()
    alloc_convdesc_and_bias_buffer(layerinfo_vect);
    alloc_hw_weight_buffer(layerinfo_vect); // this function shall initialize all the weight data as -1;
    process_hw_weight_buffer(layerinfo_vect);
    apply_scale_factor(feature_map_dict["img"]);
    
    exec_plot(layerinfo_vect,feature_map_dict);
    std::cout<<"exec_plot done"<<std::endl;

    del_featuremap_mem(feature_map_dict);
    del_weight_buffer_pointer(layerinfo_vect);
}