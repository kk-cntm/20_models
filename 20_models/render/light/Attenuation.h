#pragma once

#include <map>

namespace render::light {

struct Attenuation {
    enum Distance { seven };
    
    float constant;
    float linear;
    float quadratic;
    
    static Attenuation FromDistance(Distance distance);
    
private:
    static std::map<Distance, Attenuation> s_attenuationTable;
};

}
