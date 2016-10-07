#include "threshold.h"

bool threshold::read_default_config_flag = false;

double threshold::LSDthrehold = 0.30;

threshold::threshold(/*QString config_file_path*/)
{
    if(!read_default_config_flag){
        //QString default_config_file_path = "path-to-configFolder/default.yml";
        //QString dynamic_config_file_path = "path-to-configFolder/dynamic.yml";

        // TODO : Read default config

        // TODO : dynamic << default
    }
}
