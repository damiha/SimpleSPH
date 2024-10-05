
#include "../include/Globals.h"

float dot(sf::Vector2f &u, sf::Vector2f &v)
{
    return (u.x * v.x) + (u.y * v.y);
}

float randF(float min, float max)
{
    static std::mt19937 generator(std::time(0));
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

float randN(float sigma) {
    static std::mt19937 generator(std::time(0));
    std::normal_distribution<float> distribution(0, 1.0);
    return distribution(generator) * sigma;
}

float clamp(float v, float min, float max){
    return v > max ? max : (v < min ? min : v);
}

void print(char* varName, float v){
    printf("%s: %.3f\n", varName, v);
}

void print(char* varName, sf::Vector2f& v){
    printf("%s: (%.3f, %.3f)\n", varName, v.x, v.y);
}

void warnIfAbnormal(char* varName, sf::Vector2f& v){

    // x != x iff x is Nan
    if(v.x != v.x || v.y != v.y){
        printf("WARN: %s is NaN\n", varName);
        exit(1);
    }
}
