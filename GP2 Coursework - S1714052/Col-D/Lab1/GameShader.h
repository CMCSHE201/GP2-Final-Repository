#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "GameCamera.h"

class GameShader
{
public:
	GameShader(const std::string& filename);

	void Bind(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Camera& camera);

	std::string GameShader::LoadShader(const std::string& fileName);
	void GameShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint GameShader::CreateShader(const std::string& text, unsigned int type);

    ~GameShader();


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables
};
