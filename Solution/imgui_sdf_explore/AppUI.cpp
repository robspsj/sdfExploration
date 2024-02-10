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


static char* CreateTexture(const int width, const int height)
{
	char* tex = new char[4 * width * height];

	float centerX = 4.6f;
	float centerY = 3.2f;
	float radius = 2.0f;

	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
		{
			size_t i = (y * width + x) * 4;

			float dist = circleSignedDistance(float(x), float(y), centerX, centerY, radius);
			float intensity = transpose(dist, -1, 1, 0, 255);
			char intensity_clamped = clamp(intensity, 0, 255);

			tex[i + 0] = intensity_clamped;
			tex[i + 1] = intensity_clamped;
			tex[i + 2] = intensity_clamped;
			tex[i + 3] = 255;
		}

	return tex;
}

static ImTextureID GenerateTexture(const char* tex, const int width, const int height)
{
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

	return (ImTextureID)image_texture;
}

void AppUI::Setup()
{
	int width = 8;
	int height = 8;

	this->textureId = (ImTextureID)GenerateTexture(
		CreateTexture(width, height), width, height);
}

void AppUI::DrawWindow()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Texture");

	ImGui::Image(this->textureId, ImVec2(512, 512));
	ImGui::Image(this->textureId, ImVec2(8, 8));

	ImGui::End();
}
