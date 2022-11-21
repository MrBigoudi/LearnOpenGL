#include "utils.hpp"
#include "constants.hpp"

#include <stdio.h>

int memIntVect(std::vector<int> v, int e1, int e2){
    for(int i=0; i<(int)v.size()-1; i+=2){
        if(e1 == v[i] && e2==v[i+1]){
            return i;
        }
    }
    return -1;
}

float convertX(float x){
    float convX = x / (WINDOW_WIDTH / 2.0f) - 1.0f;
    // printf("x: %f,convX: %f\n", x, convX);
    return convX;
}

float convertY(float y){
    float convY = -(y / (WINDOW_HEIGHT / 2.0f) - 1.0f);
    // printf("y: %f,convY: %f\n", y, convY);
    return convY;
}

float floatModulo(float a, float mod){
    float div = a / mod;
    int q = (int)div;
    float r = a - q*mod;
    r = r<0 ? mod+r : r;
    return r;
}