#include "param.h"
#include "buffer.h"

#ifndef _EXEC_H_
#define _EXEC_H_

void exec_plot(
    std::vector<LayerInfo_t > & layerinfo_vect,
    std::map< std::string, FeatureMapBuffer_t > & feature_map_dict);


#endif