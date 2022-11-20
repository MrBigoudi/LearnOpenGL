#include "player.hpp"
#include "constants.hpp"

#include <GLFW/glfw3.h>
#include <stdio.h>

void Player::draw(){
    // Player
    // printf("tri: %d\n", (int)this->mVao.size());
    // first triangle
    // this->mVao[0] = convertX(this->mX);
    // this->mVao[1] = convertY(this->mY);
    // this->mVao[2] = convertX(this->mX + this->mWidth); 
    // this->mVao[3] = convertY(this->mY);
    // this->mVao[4] = convertX(this->mX); 
    // this->mVao[5] = convertY(this->mY + this->mHeight);

    // // second triangle
    // this->mVao[6] = convertX(this->mX + this->mWidth); 
    // this->mVao[7] = convertY(this->mY);
    // this->mVao[8] = convertX(this->mX); 
    // this->mVao[9] = convertY(this->mY + this->mHeight);
    // this->mVao[10] = convertX(this->mX + this->mWidth); 
    // this->mVao[11] = convertY(this->mY + this->mHeight);

    // // Player angle
    // // printf("angle: %d\n", (int)this->mAngleVao.size());
    // // printf("angle: %f, sin: %f\n", mAngle, mScale*sin(mAngle) );
    // this->mAngleVao[0] = convertX(mX+mWidth/2);
    // this->mAngleVao[1] = convertY(mY+mHeight/2);
    // this->mAngleVao[2] = convertX(mX+mWidth/2+mScale*cos(mAngle));
    // this->mAngleVao[3] = convertY(mY+mHeight/2-mScale*sin(mAngle));


    // Cursor
    this->mCursorVao[0] = convertX(mCursorX - CURSOR_SIZE/2.0f);
    this->mCursorVao[1] = convertY(mCursorY);
    this->mCursorVao[2] = convertX(mCursorX + CURSOR_SIZE/2.0f);
    this->mCursorVao[3] = convertY(mCursorY);

    this->mCursorVao[4] = convertX(mCursorX);
    this->mCursorVao[5] = convertY(mCursorY - CURSOR_SIZE/2.0f);
    this->mCursorVao[6] = convertX(mCursorX);
    this->mCursorVao[7] = convertY(mCursorY + CURSOR_SIZE/2.0f);
}

void Player::move(std::vector<std::vector<int>> map){
    float oldX = this->mX;
    float oldY = this->mY;
    this->mX += cos(this->mAngle)*PLAYER_SPEED;
    this->mY -= sin(this->mAngle)*PLAYER_SPEED;

    // check collisions
    int mX = (int)(this->mX/TILE_SIZE);
    int mY = (int)(this->mY/TILE_SIZE);
    int mX2 = (int)((this->mX+this->mWidth)/TILE_SIZE);
    int mY2 = (int)((this->mY+this->mHeight)/TILE_SIZE);
    // printf("mx: %d, my: %d\n", mX, mY);
    if(map[mY][mX]!=0 || map[mY2][mX]!=0 || map[mY][mX2]!=0 || map[mY2][mX2]!=0){
        this->mX = oldX;
        this->mY = oldY;
    }
}

void Player::turnLeft(){
    // printf("avant mod: %f\n", mAngle - PLAYER_ROTATION_SPEED);
    this->mAngle = floatModulo((this->mAngle + PLAYER_ROTATION_SPEED), 2*PI);
    // printf("apres mod: %f\n", mAngle);
}

void Player::turnRight(){
    this->mAngle = floatModulo((this->mAngle - PLAYER_ROTATION_SPEED), 2*PI);
}

void Player::update(float dt, std::vector<int> keys, std::vector<std::vector<int>> map, GLFWwindow* window){
    // printf("update\n");
    for (int i=0; i<(int)keys.size(); i+=2){
        switch(keys[i]){
            case GLFW_KEY_W:
                // printf("moved\n");
                this->move(map);
                break;
            case GLFW_KEY_UP:
                this->move(map);
                break;
            case GLFW_KEY_A:
                this->turnLeft();
                break;
            case GLFW_KEY_LEFT:
                this->turnLeft();
                break;
            case GLFW_KEY_D:
                this->turnRight();
                break;   
            case GLFW_KEY_RIGHT:
                this->turnRight();
                break;
        }
    }

    // update cursor
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    mCursorX = xPos;
    mCursorY = yPos;
}