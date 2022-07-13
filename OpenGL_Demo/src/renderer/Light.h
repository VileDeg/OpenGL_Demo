#pragma once
#include "math_headers.h"

//struct DirectionalLight
//{
//	alignas(16) glm::vec3 direction;
//	alignas(16) glm::vec3 ambient;
//	alignas(16) glm::vec3 diffuse;
//	alignas(16) glm::vec3 specular;
//};
//
//struct PointLight
//{
//	glm::vec3 position;
//	float constant;
//	glm::vec3 ambient;
//	float linear;
//	glm::vec3 diffuse;
//	float quadratic;
//	glm::vec3 specular;
//};
//	
//struct Spotlight
//{
//	glm::vec3 position;
//	float constant;
//	glm::vec3 direction;
//	float linear;
//	glm::vec3 ambient;
//	float quadratic;
//	glm::vec3 diffuse;
//	float cutOff;
//	glm::vec3 specular;
//	float outerCutOff;
//};

struct Light
{
	glm::vec3 position;
	float constant;
	glm::vec3 direction;
	float linear;
	glm::vec3 ambient;
	float quadratic;
	glm::vec3 diffuse;
	float cutOff;
	glm::vec3 specular;
	float outerCutOff;
	int type;
};