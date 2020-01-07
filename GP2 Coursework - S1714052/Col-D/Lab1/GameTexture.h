#pragma once
#include <string>
#include <GL\glew.h>

class GameTexture
{
public:
	GameTexture(const std::string& fileName);

	void Bind(unsigned int unit); // bind upto 32 textures

	~GameTexture();

protected:
private:

	GLuint textureHandler;
};

