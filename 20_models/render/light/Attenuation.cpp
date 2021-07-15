#include <functional>
#include "Attenuation.h"

using namespace render::light;

Attenuation Attenuation::FromDistance(Attenuation::Distance distance) {
    return s_attenuationTable[distance];
}

std::map<Attenuation::Distance, Attenuation> Attenuation::s_attenuationTable = []() {
    std::map<Attenuation::Distance, Attenuation> table;
    
    table[Attenuation::Distance::seven] = { 1.0f, 0.7, 1.8f };
    
    return table;
}();
