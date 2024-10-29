#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "raylib.h"

using namespace std;

//2D point struct for geometry computations
typedef struct point{
    double x,y;
    point(double xx, double yy){
        x = xx; y = yy;
    }
    point(){}
}point;

// Random point generator class (uses Mersenne Twister 19937 generator)
class RandomPointGenerator {
    mt19937 rng;
    public:
        void initRandomPointGenerator();
        vector<point> generateRandomPoints(int N, double max_coord);
};

void RandomPointGenerator::initRandomPointGenerator(){
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();// use chrono to get a random seed for the generator
    rng.seed(seed);
}

vector<point> RandomPointGenerator::generateRandomPoints(int N, double max_coord){
    vector<point> points;
    uniform_real_distribution<double> dist(0.0, max_coord);
    for(int i=0; i<N; i++){
        point p(dist(rng), dist(rng));
        points.push_back(p);
    }
    return points;
}

// Game map class
class GameMap{
    vector<point> controlPoints;
    public:
        void reset();
        void generateNewMap(const vector<point>& randomControlPoints);
        void displayMap();
};

void GameMap::reset(){
    controlPoints.clear();
}

void GameMap::generateNewMap(const vector<point>& randomControlPoints){
    controlPoints = randomControlPoints;
}

void GameMap::displayMap(){//let's show the map on the screen :)

}

// Main game loop
int main(){

    float screenWidth = 800;
    float screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "The Four Color Challenge");

    Font romulusFont = LoadFont("assets/romulus_font.png");

    bool inTitleScreen = true;
    bool generateNewMap = true;
    bool mapSolved = false;

    SetTargetFPS(60); //sets maximum FPS rate

    RandomPointGenerator randomPointGenerator;
    GameMap gameMap;

    while (!WindowShouldClose()){//close button OR esc key press
        BeginDrawing();
        ClearBackground(BLACK);

        if (inTitleScreen){
            Vector2 titlePosition = {screenWidth/2.0f - 140, screenHeight/3.0f};
            Vector2 messagePosition = {screenWidth/2.0f - 130, screenHeight/2.0f};
            DrawTextEx(romulusFont, "The Four Color Challenge", titlePosition, 20, 3, WHITE);
            DrawTextEx(romulusFont, "Press ENTER to start game", messagePosition, 16, 3, WHITE);
            if(IsKeyPressed(KEY_ENTER)){
                inTitleScreen = false;
            }
        }else{
            if(generateNewMap){//Let's sample random points and generate new map
                gameMap.reset();
                randomPointGenerator.initRandomPointGenerator();
                gameMap.generateNewMap(randomPointGenerator.generateRandomPoints(15, 300));
                generateNewMap = false;//we let player solve this map, then we'll generate a new one
            }

            gameMap.displayMap();

            if(mapSolved){// show congratulations message and ask for ENTER to move to next map
                if(IsKeyPressed(KEY_ENTER)){
                    generateNewMap = true;
                    mapSolved = false;
                }
            }
        }
        
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}