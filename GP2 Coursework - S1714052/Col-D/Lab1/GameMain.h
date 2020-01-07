#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "GameDisplay.h" 
#include "GameShader.h"
#include "MeshModel.h"
#include "GameTexture.h"
#include "transform.h"
#include "GameAudio.h"
#include <chrono>
#include <ctime> 

enum class GameState{PLAY, EXIT};

using namespace std;

class GameMain
{
public:
	GameMain();
	~GameMain();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);

	GameDisplay _gameDisplay;
	GameState _gameState;
	MeshModel player;
	MeshModel rock1;
	MeshModel rock2;
	MeshModel rock3;
	MeshModel water;
	Camera myCamera;
	GameAudio audioDevice;

	float counter;
	float counter2;
	float counter3;
	float fullTime;

	bool start;
	bool lose;
	bool left;
	bool right;

	glm::vec2 bulletPos;

	unsigned int collisionSound;
	unsigned int backGroundMusic;
};

