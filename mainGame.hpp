#ifndef __MAIN_GAME_HPP__
#define __MAIN_GAME_HPP__

#include "constants.hpp"
#include "player.hpp"
#include "map.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class MainGame{

    private:
        GLFWwindow* mWindow = nullptr;
        double mTime = 0.0f;
        static std::vector<int> sKeys;
        Player* mPlayer = new Player();
        std::vector<std::vector<int>> mMap = initMap("map.txt");
    

    public:
        void run();

    private:
        void init();
        void quit();
        void inputHandler();
        void gameLoop();
        void update(double dt);
        void draw();
        void drawMap();
        void drawPlayer();
        void drawSky();
        void drawGround();
        void castRays();

    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void pressKey(int key, int scancode);
        static void releaseKey(int key, int scancode);
};

#endif