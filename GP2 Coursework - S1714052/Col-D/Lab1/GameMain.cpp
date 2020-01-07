#include "GameMain.h"
#include "GameCamera.h"
#include <Windows.h>
#include <iostream>
#include <string>


Transform transform;

GameMain::GameMain()
{
	_gameState = GameState::PLAY;
	GameDisplay* _gameDisplay = new GameDisplay(); //new display
	MeshModel* player();
	MeshModel* rock1();
	MeshModel* rock2();
	MeshModel* rock3();
	MeshModel* water();
	GameAudio* audioDevice();
}

GameMain::~GameMain()
{
}

void GameMain::run()
{
	initSystems(); 
	gameLoop();
}

void GameMain::initSystems()
{
	//Initialising everything used.
	_gameDisplay.initDisplay(); 
	start = true;
	collisionSound = audioDevice.loadSound("..\\res\\Collision.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	player.loadModel("..\\res\\Canoe.obj");
	rock1.loadModel("..\\res\\rock.obj");
	rock2.loadModel("..\\res\\rock.obj");
	rock3.loadModel("..\\res\\rock2.obj");
	
	//Initialises the camera pos and projection
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
}

void GameMain::gameLoop()
{
	//Used to keep track of time played
	auto frame1 = chrono::high_resolution_clock::now();
	auto frame2 = chrono::high_resolution_clock::now();

	//Loops whilst the game is in play
	while (_gameState != GameState::EXIT)
	{
		//Calculates elapsed time
		frame2 = chrono::high_resolution_clock::now();
		chrono::duration<float> elapsed = frame2 - frame1;
		frame1 = frame2;
		float fElapsed = elapsed.count();
		fullTime = fullTime + fElapsed;

		processInput();
		drawGame();
		collision(player.getSpherePos(), player.getSphereRadius(), rock1.getSpherePos(), rock1.getSphereRadius());
		collision(player.getSpherePos(), player.getSphereRadius(), rock2.getSpherePos(), rock2.getSphereRadius());
		collision(player.getSpherePos(), player.getSphereRadius(), rock3.getSpherePos(), rock3.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
 	}
}

void GameMain::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		//Closes the game if the gameState changes to EXIT
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}

		//Checks for Keyboard input
		switch (evnt.type)
		{
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_a:
			{
				left = true;
				break;
			}
			case SDLK_d:
			{
				right = true;
				break;
			}
			case SDLK_LEFT:
			{
				left = true;
				break;
			}
			case SDLK_RIGHT:
			{
				right = true;
				break;
			}
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			}
		}
	}
}


//Checks for collisions between the player and the rocks
bool GameMain::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) *(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		playAudio(collisionSound, m1Pos);
		lose = true;
		return true;
	}
	else
	{
		return false;
	}
}

void GameMain::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(Source, pos);
	}
}

void GameMain::drawGame()
{
		//Clears the display
		_gameDisplay.clearDisplay(0.0f, 119.0f, 190.0f, 1.0f);

		GameShader shader("..\\res\\shader"); //new shader
		GameTexture texture("..\\res\\wood.jpg"); //load texture

		//Checks if the player is to move left and if so will offset the player to make it move to the left
		if (left)
		{
			if (player.getSpherePos().x >= 2)
			{
				left = false;
			}
			else
			{
				transform.SetPos(glm::vec3(player.getSpherePos().x + 0.1, -1, 2));
				transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
				transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

				shader.Bind();
				shader.Update(transform, myCamera);
				texture.Bind(0);
				player.draw();
				player.updateSphereData(*transform.GetPos(), 0.05f);
				left = false;
			}
		}

		//Checks if the player is to move right and if so will offset the player to make it move to the right
		if (right)
		{
			if (player.getSpherePos().x <= -2)
			{
				right = false;
			}
			else
			{
				transform.SetPos(glm::vec3(player.getSpherePos().x - 0.1, -1, 2));
				transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
				transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

				shader.Bind();
				shader.Update(transform, myCamera);
				texture.Bind(0);
				player.draw();
				player.updateSphereData(*transform.GetPos(), 0.05f);
				right = false;
			}
		}

		//Checks if the player is stationary and if so will continue to render the player in the same spot.
		if (!left && !right)
		{
			transform.SetPos(glm::vec3(player.getSpherePos().x, -1, 2));
			transform.SetRot(glm::vec3(0.0, 4.5, 1.5));
			transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(0);
			player.draw();
			player.updateSphereData(*transform.GetPos(), 0.05f);
		}

		GameTexture texture1("..\\res\\rock.jpg"); //load texture

		if (start)
		{
			transform.SetPos(glm::vec3(0, 5, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock1.draw();
			rock1.updateSphereData(*transform.GetPos(), 0.5f);

			transform.SetPos(glm::vec3(-2, 8, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock2.draw();
			rock2.updateSphereData(*transform.GetPos(), 0.5f);

			transform.SetPos(glm::vec3(2, 11, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(0.001, 0.001, 0.001));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(2);
			rock3.draw();
			rock3.updateSphereData(*transform.GetPos(), 0.5f);

			start = false;
		}

		//Checks to see if rock has passed the player and if so then will reset the position to the top of the screen with a random x value.
		if (rock1.getSpherePos().y > -4)
		{
			transform.SetPos(glm::vec3(rock1.getSpherePos().x, rock1.getSpherePos().y - 0.3, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock1.draw();
			rock1.updateSphereData(*transform.GetPos(), 0.5f);
		}
		else
		{
			float xVal = rand() % 2;

			transform.SetPos(glm::vec3(xVal, 5, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock1.draw();
			rock1.updateSphereData(*transform.GetPos(), 0.5f);
		}

		if (rock2.getSpherePos().y > -4)
		{
			transform.SetPos(glm::vec3(rock2.getSpherePos().x, rock2.getSpherePos().y - 0.3, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock2.draw();
			rock2.updateSphereData(*transform.GetPos(), 0.5f);
		}
		else
		{
			float xVal = rand() % 2;

			transform.SetPos(glm::vec3(xVal, 5, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(1);
			rock2.draw();
			rock2.updateSphereData(*transform.GetPos(), 0.5f);
		}

		if (rock3.getSpherePos().y > -4)
		{
			transform.SetPos(glm::vec3(rock3.getSpherePos().x, rock3.getSpherePos().y - 0.3, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(0.001, 0.001, 0.001));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(2);
			rock3.draw();
			rock3.updateSphereData(*transform.GetPos(), 0.5f);
		}
		else
		{
			float xVal = rand() % 2;

			transform.SetPos(glm::vec3(xVal, 5, 2));
			transform.SetRot(glm::vec3(0.0, 0, 0));
			transform.SetScale(glm::vec3(0.001, 0.001, 0.001));

			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(2);
			rock3.draw();
			rock3.updateSphereData(*transform.GetPos(), 0.5f);
		}

		glEnableClientState(GL_COLOR_ARRAY);
		glEnd();

		_gameDisplay.swapBuffer();

		if (lose)
		{
			cout << "Score: " << fullTime;
			_gameState = GameState::EXIT;
		}
} 
