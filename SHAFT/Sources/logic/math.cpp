#include "logic/math.h"
#include <glm/glm.hpp>

glm::vec2 vec2toGLM(vec2 input)
{
    return glm::vec2(input.x, input.y);
}

glm::vec2 vec2itoGLM(vec2i input)
{
    return glm::vec2(input.x, input.y);
}

vec2 vec2itoVec2(vec2i input)
{
    return vec2{(float)input.x, (float)input.y};
}

vec2 glmToVec2(glm::vec2 input)
{
    return vec2{input.x, input.y};
}

glm::vec3 vec3toGLM(vec3 input)
{
    return glm::vec3(input.x, input.y, input.z);
}

vec3 glmToVec3(glm::vec3 input)
{
    return vec3{input.x, input.y, input.z};
}