

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/System/Vector2.hpp>
#include <random>
#include <ctime>

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 800;

const float pixelsPerUnit = WINDOW_WIDTH;

const float PI = 3.1415926535f;

const float EPSILON = 1e-9f;

// needs to match the value in water frag
const int MAX_POINTS_FOR_SHADER = 1024;

inline float lengthSq(sf::Vector2f& v){
    return (v.x * v.x) + (v.y * v.y);
}

float dot(sf::Vector2f& u, sf::Vector2f& v);

float randF(float min, float max);

float randN(float sigma);

float clamp(float v, float min, float max);

void print(char* varName, float v);

void print(char* varName, const sf::Vector2f& v);

void warnIfAbnormal(char* varName, sf::Vector2f& v);

enum RoundedLineCollision : unsigned int {
    START = 0,
    END = 1,
    BOTH = 2,
    NONE = 3
};

#endif