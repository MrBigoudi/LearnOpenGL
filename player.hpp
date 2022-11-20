#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "constants.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>

class Player{

    private:
        int mScale = 64;
    
    public:
        float mHeight = PLAYER_HEIGHT;
        float mWidth = PLAYER_WIDTH;
        float mX = WINDOW_WIDTH/2.0f;
        float mY = WINDOW_HEIGHT/2.0f;
        // std::vector<float> mVao;
        // std::vector<float> mColor = {0.0f, 1.0f, 1.0f};
        // std::vector<float> mAngleColor = {0.0f, 1.0f, 0.0f};
        // std::vector<float> mAngleVao;

        float mCursorX = mX;
        float mCursorY = mY;
        std::vector<float> mCursorVao;
        std::vector<float> mCursorColor = {0.0f, 0.0f, 0.0f};
        float mCursorWidth = 4;
        
        float mFov = PI / 3;
        int mNbRays = 512;
        int mMaxDepth = 64;
        float mAngle = 0;

    public:
        Player(){
            // std::vector<float> vao = {
            //     // first triangle
            //     convertX(this->mX), convertY(this->mY),
            //     convertX(this->mX + this->mWidth), convertY(this->mY),
            //     convertX(this->mX), convertY(this->mY + this->mHeight),

            //     // second triangle
            //     convertX(this->mX + this->mWidth), convertY(this->mY),
            //     convertX(this->mX), convertY(this->mY + this->mHeight),
            //     convertX(this->mX + this->mWidth), convertY(this->mY + this->mHeight)
            //     };
            // mVao = vao;

            // std::vector<float> angleVao = {
            //     convertX(mX+mWidth/2), 
            //     convertY(mY+mHeight/2), 
                
            //     convertX(mX+mWidth/2+mScale*cos(mAngle)), 
            //     convertY(mY+mHeight/2+mScale*sin(mAngle))
            // };
            // mAngleVao = angleVao;

            std::vector<float> cursorVao = {
                // first line
                convertX(mCursorX - CURSOR_SIZE/2.0f),
                convertY(mCursorY),
                convertX(mCursorX + CURSOR_SIZE/2.0f),
                convertY(mCursorY),

                // second line
                convertX(mCursorX),
                convertY(mCursorY - CURSOR_SIZE/2.0f),
                convertX(mCursorX),
                convertY(mCursorY + CURSOR_SIZE/2.0f),
            };
            mCursorVao = cursorVao;
        }

        void draw();
        void move(std::vector<std::vector<int>> map);
        void turnLeft();
        void turnRight();
        void update(float dt, std::vector<int>keys, std::vector<std::vector<int>> map, GLFWwindow* window);
        void setCursorPos(float x, float y){mCursorX = x; mCursorY = y;}
};

#endif