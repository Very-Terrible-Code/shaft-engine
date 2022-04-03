#pragma once
#include <glm/glm.hpp>

typedef struct vec2
{
    float x;
    float y;
} vec2;

typedef struct rot
{
    vec2 rot;
    float angle;
} rot;


typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

typedef struct vec2i
{
    int x;
    int y;
} vec2i;

typedef struct rect
{
    float x;
    float y;
    float w;
    float h;
} rect;

glm::vec2 vec2toGLM(vec2 input);
glm::vec2 vec2itoGLM(vec2i input);
vec2 glmToVec2(glm::vec2 input);

glm::vec3 vec3toGLM(vec3 input);
vec3 glmToVec3(glm::vec3 input);