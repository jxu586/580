#include "RoomObject.h"

#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

CRoomObject::CRoomObject(void)
{
	float L = 10.0f;
	int idx = 0;
	VERTEX pVertices[36];
	// back face
	pVertices[idx].Position = glm::vec3(-L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(0, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 0, 1);
	pVertices[idx].Texcoord = glm::vec3(1, 0, 1);
	idx++;

	// left face
	pVertices[idx].Position = glm::vec3(-L, -L, L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, L, L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, L, -L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, -L, L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, L, -L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, -L, -L);
	pVertices[idx].Normal = glm::vec3(1, 0, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 0, 1);
	idx++;

	// bottom face
	pVertices[idx].Position = glm::vec3(-L, -L, L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(-L, -L, L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(0, 0, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, -L, -L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 1, 1);
	idx++;

	pVertices[idx].Position = glm::vec3(L, -L, L);
	pVertices[idx].Normal = glm::vec3(0, 1, 0);
	pVertices[idx].Texcoord = glm::vec3(1, 0, 1);
	idx++;

	glm::mat4 T = glm::rotate(glm::pi<float>(), glm::vec3(0, 1, 0));
	for (int i = 0; i < 12; i++) {
		pVertices[idx + i].Position = glm::vec3(T * glm::vec4(pVertices[i].Position, 1.0));
		pVertices[idx + i].Normal = glm::vec3(T * glm::vec4(pVertices[i].Normal, 0.0));
		pVertices[idx + i].Texcoord = pVertices[i].Texcoord;
	}
	idx += 12;
	// generate six faces
	T = glm::rotate(glm::pi<float>(), glm::vec3(1, 0, 0));
	for (int i = 0; i < 6; i++) {
		pVertices[idx + i].Position = glm::vec3(T * glm::vec4(pVertices[i + 12].Position, 1.0));
		pVertices[idx + i].Position.y = 8.01f;
		pVertices[idx + i].Normal = glm::vec3(T * glm::vec4(pVertices[i + 12].Normal, 0.0));
		pVertices[idx + i].Texcoord = pVertices[i].Texcoord;
	}
	// generate vertex buffer
	pVertexBuffer = new GLuint[1];
	glGenBuffers(1, pVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(VERTEX), pVertices, GL_STATIC_DRAW);
	// generate vertex array
	pVertexArray = new GLuint[1];
	glGenVertexArrays(1, pVertexArray);
	glBindVertexArray(pVertexArray[0]);

	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)24);

	glBindVertexArray(0);

	mat.ambient[0] = 0.1f;
	mat.ambient[1] = 0.1f;
	mat.ambient[2] = 0.1f;

	mat.diffuse[0] = 0.1f;
	mat.diffuse[1] = 0.9f;
	mat.diffuse[2] = 0.1f;

	mat.specular[0] = 0.05f;
	mat.specular[1] = 0.05f;
	mat.specular[2] = 0.05f;

	mat.shininess = 1.0f;
}

CRoomObject::~CRoomObject(void)
{
}

void CRoomObject::Render()
{
	glBindVertexArray(pVertexArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
