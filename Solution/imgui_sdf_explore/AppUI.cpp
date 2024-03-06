#include "AppUI.h"
#include <GL/gl3w.h>
#include <cmath>


static float circleSignedDistance(float sampleX, float sampleY, float centerX, float centerY, float radius)
{
	float distanceSquared = ((sampleX - centerX) * (sampleX - centerX) + (sampleY - centerY) * (sampleY - centerY));
	return std::sqrt(distanceSquared) - radius;
}


static float transpose(float a, float a0, float a1, float b0, float b1)
{
	float lenA = a1 - a0;
	float lenB = b1 - b0;

	return b0 + ((a - a0) / lenA) * lenB;
}

static float clamp(float a, float a0, float a1)
{
	if (a0 > a1)
	{
		if (a > a0) return a0;
		if (a < a1) return a1;
		return a;
	}

	if (a < a0) return a0;
	if (a > a1) return a1;
	return a;
}


static void DrawCircleInTexture(TextureHandler textureHandler, float centerX, float centerY, float radius)
{

	float width = textureHandler.width;
	float height = textureHandler.height;

	char* tex = new char[4 * width * height];

	float inverseWidth = 1.0f / width;
	float inverseHeight = 1.0f / height;

	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
		{
			size_t i = (y * width + x) * 4;

			float dist = circleSignedDistance(
				(float(x) + 0.5f) * inverseWidth,
				(float(y) + 0.5f) * inverseHeight,
				centerX, centerY, radius)
				* max(textureHandler.width, textureHandler.height);

			float intensity = transpose(dist, -1, 1, 0, 255);
			char intensity_clamped = clamp(intensity, 0, 255);

			tex[i + 0] = intensity_clamped;
			tex[i + 1] = intensity_clamped;
			tex[i + 2] = intensity_clamped;
			tex[i + 3] = 255;
		}

	glBindTexture(GL_TEXTURE_2D, (GLuint)textureHandler.textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
}


static void DrawWithOpenGLInTexture(TextureHandler textureHandler)
{
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, (GLuint)textureHandler.textureId, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDeleteProgram(shaderProgram);
}

static TextureHandler GenerateEmptyTexture(const int width, const int height)
{
	GLuint imageTexture;
	glGenTextures(1, &imageTexture);
	glBindTexture(GL_TEXTURE_2D, imageTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	return { (ImTextureID)imageTexture, width, height };
}

void AppUI::Setup()
{

	this->smallTextureId = GenerateEmptyTexture(32, 32);
	DrawCircleInTexture(this->smallTextureId, 0.4, 0.5, 0.33);

	this->bigTextureId = GenerateEmptyTexture(512, 512);
	//DrawWithOpenGLInTexture(this->bigTextureId);
}

void AppUI::DrawWindow()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Texture");

	ImGui::Image(this->smallTextureId.textureId, ImVec2(this->smallTextureId.width, this->smallTextureId.height));
	ImGui::Image(this->bigTextureId.textureId, ImVec2(this->bigTextureId.width, this->bigTextureId.height));

	ImGui::End();
}
