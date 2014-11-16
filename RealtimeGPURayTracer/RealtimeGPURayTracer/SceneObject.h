#ifndef _SCENE_OBJECT_
#define _SCENE_OBJECT_

#include "OpenGLHeader.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct VERTEX
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Texcoord;
};

struct SPOT_LIGHT
{
	glm::vec4 lightColor;
	glm::vec3 lightPosition;
	glm::vec3 lightDir;
	float fov;
	float lightAttenu;
};

class CSceneObject
{
protected:
	GLuint *pVertexBuffer;
	GLuint *pIndexBuffer;
	GLuint *pVertexArray;
	int numArrays;
	int id;
public:
	CSceneObject(void);
	~CSceneObject(void);

	virtual void Render() = 0;
};

#endif

