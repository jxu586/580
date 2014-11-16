#include "ShowTexturePass.h"
#include "ShowTexturePassShader.h"
#include "error.h"
#include "utility.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <cstdint>

using namespace std;

CShowTexturePass::CShowTexturePass(void)
{
}

CShowTexturePass::~CShowTexturePass(void)
{
	SAFE_DELETE(pShader);
	glDeleteBuffers(1, &pVertexBuffer);
	glDeleteVertexArrays(1, &pVertexArray);
}

int CShowTexturePass::Init()
{
	int r = 0;
	CShowTexturePassShader *pSTPShader = new CShowTexturePassShader;
	glm::vec3 Position[3];

	IF_FAILED(r, pSTPShader->Init(), init_err);
	pShader = (CShaderProgram*)pSTPShader;

	Position[0] = glm::vec3(3, 1, 0);
	Position[1] = glm::vec3(-1, -3, 0);
	Position[2] = glm::vec3(-1, 1, 0);

	glGenBuffers(1, &pVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Position), Position, GL_STATIC_DRAW);

	glGenVertexArrays(1, &pVertexArray);
	glBindVertexArray(pVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
init_err:
	return r;
}

void CShowTexturePass::RenderBegin(void *pArgs)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CShowTexturePassShader *pCurrentShader = GetShaderHandle<CShowTexturePassShader>();
	pCurrentShader->BindShader();

	uint64_t pTex = reinterpret_cast<uint64_t>(pArgs);
	pCurrentShader->SetTexture((GLuint)pTex, GL_TEXTURE0 + 10);
	pCurrentShader->SetTexture(pTex, GL_TEXTURE0 + 10);

	glBindVertexArray(pVertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void CShowTexturePass::RenderEnd()
{
}
