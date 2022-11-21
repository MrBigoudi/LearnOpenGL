#include "mainGame.hpp"
#include "constants.hpp"
#include "callbacks.hpp"
#include "map.hpp"
#include "utils.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <stdio.h>

void MainGame::run(){
    this->init();
    this->gameLoop();
    this->quit();
}

void MainGame::init(){

    glfwSetErrorCallback(error_callback);

    if(!glfwInit()){
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayCasting", NULL, NULL);
    if(!this->mWindow){
        this->quit();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowPos(this->mWindow, 1128-WINDOW_WIDTH/2, 752 - WINDOW_HEIGHT/2);

    glfwSetFramebufferSizeCallback(this->mWindow, framebuffer_size_callback);
    glfwMakeContextCurrent(this->mWindow);
    glfwSetKeyCallback(this->mWindow, MainGame::key_callback);
    glfwSwapInterval(1);

    // printMap(this->mMap);
    // printf("px: %f, py: %f\n", this->mPlayer->mX, this->mPlayer->mY);

}

void MainGame::quit(){
    glfwDestroyWindow(this->mWindow);
    glfwTerminate();
    MainGame::sKeys.clear();
}

void MainGame::inputHandler(){
    glfwPollEvents();
}

void MainGame::update(double dt){
    this->mPlayer->update(dt, MainGame::sKeys, this->mMap, this->mWindow);
    // printf("px: %f, py: %f\n", this->mPlayer->mX, this->mPlayer->mY);
}

void MainGame::drawPlayer(){
    // draw player
    Player* p = this->mPlayer;
    p->draw();

    // // draw angle
    // glColor3f(p->mAngleColor[0], p->mAngleColor[1], p->mAngleColor[2]);
    // glLineWidth(4);
    // glBegin(GL_LINES);
    // glVertex2f(p->mAngleVao[0], p->mAngleVao[1]);
    // glVertex2f(p->mAngleVao[2], p->mAngleVao[3]);
    // // printf("x1: %f, y1: %f, x2: %f, y2: %f\n", p->mAngleVao[0], p->mAngleVao[1], p->mAngleVao[2], p->mAngleVao[3]);
    // glEnd();

    // // printf("color: %d\n", (int)this->mPlayer.mColor.size());
    // glColor3f(p->mColor[0],p->mColor[1],p->mColor[2]);
    // glBegin(GL_TRIANGLES);
    // // printf("vao: %d\n", (int)this->mPlayer.mVao.size());
    // // first triangle
    // glVertex2f(p->mVao[0],p->mVao[1]);
    // glVertex2f(p->mVao[2],p->mVao[3]);
    // glVertex2f(p->mVao[4],p->mVao[5]);
    // // second triangle
    // glVertex2f(p->mVao[6],p->mVao[7]);
    // glVertex2f(p->mVao[8],p->mVao[9]);
    // glVertex2f(p->mVao[10],p->mVao[11]);
    // glEnd();

    // printf("px: %f, py: %f\n", this->mPlayer->mX, this->mPlayer->mY);
    glLineWidth(p->mCursorWidth);
    glColor3f(p->mCursorColor[0], p->mCursorColor[1], p->mCursorColor[2]);
    glBegin(GL_LINES);
    // first line
    for(int i=0; i< (int)p->mCursorVao.size(); i+=2){
        glVertex2f(p->mCursorVao[i], p->mCursorVao[i+1]);
    }
    glEnd();

}


void MainGame::drawMap(){
    std::vector<std::vector<int>> map = this->mMap;
    int raws = map.size();
    int cols = map[0].size();

    for(int i=0; i<raws; i++){
        for(int j=0; j<cols; j++){
            if(map[i][j]==0) continue;
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            // printf("i: %d, j: %d\n", i, j);
            glVertex2f(convertX(j*TILE_SIZE+1), convertY(i*TILE_SIZE+1));
            glVertex2f(convertX((j+1)*TILE_SIZE-1), convertY(i*TILE_SIZE+1));
            glVertex2f(convertX((j+1)*TILE_SIZE-1), convertY((i+1)*TILE_SIZE-1));
            glVertex2f(convertX(j*TILE_SIZE+1), convertY((i+1)*TILE_SIZE-1));
            glEnd();
        }
    }
    // printf("px: %f, py: %f\n", this->mPlayer->mX, this->mPlayer->mY);

}


void MainGame::castRays(){
    float deltaAngle = this->mPlayer->mFov / (this->mPlayer->mNbRays-1);
    float initRay = floatModulo(this->mPlayer->mAngle + (this->mPlayer->mFov/2), 2*PI);
    //printf("mangle: %f, mFov: %f\n", this->mPlayer->mAngle, this->mPlayer->mFov);

    float px = this->mPlayer->mX + (this->mPlayer->mWidth/2);
    float py = this->mPlayer->mY + (this->mPlayer->mHeight/2);

    int delta = (int)(WINDOW_WIDTH / this->mPlayer->mNbRays);

    // printf("px: %f, py: %f\n", px, py);
    for(int i=0; i<this->mPlayer->mNbRays; i++){
        float curRay = floatModulo(initRay - i*deltaAngle, 2*PI);
        // printf("curRay[%d]: %f\n", i, curRay);
        float x, y, deltaX, deltaY;
        int mx, my; 
        float vx = 1e9, vy = 1e9, hx = 1e9, hy = 1e9;
        float disV, disH;
        bool foundV = false;
        bool foundH = false;
        int up = 0, left = 0;
        int depth = 0;
        float Tan = tan(curRay);
        if(Tan==0.0f) Tan = 1e-6;
        // printf("curRay[%d]: %f, tan: %f\n", i, curRay, Tan);
        int wallColorH = 1;
        int wallColorV = 1;



        //---Vertical---

        if((0 <= curRay && curRay < PI/2) or (3*PI/2 < curRay && curRay < 5*PI/2)){ //looking right
            x = ((int)(px/TILE_SIZE))*TILE_SIZE+TILE_SIZE; 
            y = py - Tan*(x-px);
            deltaX = TILE_SIZE;
            deltaY = -deltaX*Tan;
            // printf("right\n");
        }
        
        else if((PI/2 < curRay && curRay < 3*PI/2)){ //looking left
            x = ((int)(px/TILE_SIZE))*TILE_SIZE;    
            y = py - Tan*(x-px);
            left = 1;
            // printf("left\n");
            deltaX = -TILE_SIZE;
            deltaY = -deltaX*Tan;
        }

        else{ //looking up or down. no hit
            // printf("no hit\n");
            x=px; 
            y=py; 
            depth=this->mPlayer->mMaxDepth;
        }                                                    

        while(depth<this->mPlayer->mMaxDepth){ 
            mx=(int)(x/TILE_SIZE); 
            my=(int)(y/TILE_SIZE);                 
            if(mx-left >= 0 && my >= 0 && mx-left < MAP_WIDTH && my < MAP_HEIGHT && this->mMap[my][mx-left]!=0){ // hit
                depth=this->mPlayer->mMaxDepth; 
                foundV = true;
                disV=cos(curRay)*(x-px)-sin(curRay)*(y-py);
                wallColorV = this->mMap[my][mx-left];
            }//hit         
            else{ 
                x+=deltaX; 
                y+=deltaY; 
                depth++;//check next horizontal
            }
        }

        if(foundV){
            vx=x; 
            vy=y;
        }

        //---Horizontal---
        depth = 0;
        Tan = 1/Tan;
        
        // printf("ray: %f\n", curRay);
        if(0 < curRay && curRay < PI){ //looking up
            y = ((int)(py/TILE_SIZE))*TILE_SIZE; 
            x = px + (py-y)*Tan;
            up = 1;
            deltaY = -TILE_SIZE;
            deltaX = -deltaY*Tan;
            // printf("up\n");
        }
        
        else if(PI < curRay && curRay < 2*PI){ //looking down
            y = (int)(py/TILE_SIZE)*TILE_SIZE+TILE_SIZE;    
            x = px + (py-y)*Tan;
            // printf("down\n");
            deltaY = TILE_SIZE;
            deltaX = -deltaY*Tan;
            // printf("x: %f, y: %f, px: %f, py: %f\n", x, y, px, py);
        }

        else{ //looking left or right. no hit
            x=px; 
            y=py; 
            depth=this->mPlayer->mMaxDepth;
        }      

        // printf("x: %f, y: %f\n", x, y);                                              

        while(depth<this->mPlayer->mMaxDepth){ 
            mx=(int)(x/TILE_SIZE); 
            my=(int)(y/TILE_SIZE); 
            // printf("x: %f, mx: %d, y: %f, my: %d\n", x, mx, y, my);
            if(mx >= 0 && my-up >= 0 && mx < (MAP_WIDTH) && my-up < (MAP_HEIGHT) && this->mMap[my-up][mx]!=0){ // hit
                depth=this->mPlayer->mMaxDepth; 
                foundH = true;
                disH=cos(curRay)*(x-px)-sin(curRay)*(y-py);
                wallColorH = this->mMap[my-up][mx];
            }//hit         
            else{ 
                x+=deltaX; 
                y+=deltaY; 
                depth++;//check next vertical
                // printf("depth: %d\n", depth);
            }
        } 

        if(foundH){
            hx=x; 
            hy=y;
        }

        // printf("distH: %f, distV: %f\n", disH, disV);

        x = vx;
        y = vy;
        float dis = disV;
        int wallColor = wallColorV;
        if(disH<disV){
            x = hx;
            y = hy;
            dis = disH;
            wallColor = wallColorH;
        }

        if(!foundH){
            x = vx;
            y = vy;
            dis = disV;
        }
        if(!foundV){
            x = hx;
            y = hy;
            dis = disH;
        }

        // dis = dis*cos(this->mPlayer->mAngle);
        float lineH = (WALL_CUBE*MAX_HEIGHT)/dis;
        // lineH = lineH>MAX_HEIGHT ? MAX_HEIGHT : lineH;
        float lineOff = (WINDOW_HEIGHT / 2.0f) + (lineH / 2.0f); 

        float r, g, b;
        switch(wallColor){
            case 1:
                r = 1.0f;
                g = 0.0f;
                b = 0.0f;
                break;
            case 2:
                r = 0.0f;
                g = 1.0f;
                b = 0.0f;
                break;
            case 3:
                r = 0.0f;
                g = 0.0f;
                b = 1.0f;
                break;
            case 4:
                r = 1.0f;
                g = 1.0f;
                b = 0.0f;
                break;
            case 5:
                r = 1.0f;
                g = 214/255.0f;
                b = 241/255.0f;
            default:
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
                break;
        }


        glColor3f(r,g,b);
        glLineWidth(delta); 
        glBegin(GL_LINES);
        glVertex2f(convertX(i*delta),convertY(lineOff)); 
        glVertex2f(convertX(i*delta),convertY(lineOff - lineH)); 
        glEnd();//draw 2D ray
        
    }

}

void MainGame::drawSky(){
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(convertX(0.0f), convertY(0.0f));
    glVertex2f(convertX(0.0f), convertY(WINDOW_HEIGHT/2.0f));
    glVertex2f(convertX(WINDOW_WIDTH), convertY(WINDOW_HEIGHT/2.0f));
    glVertex2f(convertX(WINDOW_WIDTH), convertY(0.0f));
    glEnd();
}

void MainGame::drawGround(){
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(convertX(0.0f), convertY(WINDOW_HEIGHT/2.0f));
    glVertex2f(convertX(0.0f), convertY(WINDOW_HEIGHT));
    glVertex2f(convertX(WINDOW_WIDTH), convertY(WINDOW_HEIGHT));
    glVertex2f(convertX(WINDOW_WIDTH), convertY(WINDOW_HEIGHT/2.0f));
    glEnd();
}


void MainGame::draw(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // clear background
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // printf("draw player\n");
    // this->drawMap();
    this->drawSky();
    this->drawGround();
    this->castRays();
    this->drawPlayer();

    glfwSwapBuffers(this->mWindow);
}

void MainGame::gameLoop(){
    while (!glfwWindowShouldClose(this->mWindow)){
        double dt = glfwGetTime() - this->mTime;
        this->mTime += dt;
        this->inputHandler();
        this->update(dt);
        this->draw();
    }
}

void MainGame::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else{
        switch(action){
            case GLFW_PRESS:
                MainGame::pressKey(key, scancode);
                break;
            case GLFW_RELEASE:
                MainGame::releaseKey(key, scancode);
                break;
            default:
                break;
        }

    }
}

void MainGame::pressKey(int key, int scancode){
    // printf("pressed:\n%d\n", (int)MainGame::sKeys.size());
    int index = memIntVect(MainGame::sKeys, key, scancode);
    // printf("index: %d\n", index);
    if (index == -1){
        MainGame::sKeys.push_back(key);
        MainGame::sKeys.push_back(scancode);
    }
    // printf("%d\n", (int)MainGame::sKeys.size());
}

void MainGame::releaseKey(int key, int scancode){
    // printf("relased:\n%d\n", (int)MainGame::sKeys.size());
    int index = memIntVect(MainGame::sKeys, key, scancode);
    // printf("index: %d\n", index);
    if (index != -1){
            int len = MainGame::sKeys.size();
            int last = MainGame::sKeys[len-1];
            int beforeLast = MainGame::sKeys[len-2];

            MainGame::sKeys[index] = beforeLast;
            MainGame::sKeys[index+1] = last;

            MainGame::sKeys.pop_back();
            MainGame::sKeys.pop_back(); 
    }
    // printf("%d\n", (int)MainGame::sKeys.size());
}

std::vector<int> MainGame::sKeys;