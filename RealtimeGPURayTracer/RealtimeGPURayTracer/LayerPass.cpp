#include "LayerPass.h"
#include "LayerPassShader.h"
#include "GBufferGenPass.h"
#include "error.h"
#include "utility.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <cstdint>

using namespace std;

CLayerPass::CLayerPass(int w, int h,int f)
{
	LayerWidth = w;
	LayerHeight = h;
	flag = f;
}

CLayerPass::~CLayerPass(void)
{
	SAFE_DELETE(pShader);
	glDeleteBuffers(1, &pVertexBuffer);
	glDeleteVertexArrays(1, &pVertexArray);
	SAFE_DELETE(pRenderTarget);
}

int CLayerPass::Init()
{
	int r = 0;
	CLayerPassShader *pSTPShader = new CLayerPassShader;
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

	bool bNeedMipMaps = false;

	// init framebuffer
	pRenderTarget = new CRenderTargetManager(2, 0, 1);
	// color attachment
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, bNeedMipMaps, 0);
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1, bNeedMipMaps, 1);

	// generate frame buffer
	IF_FAILED(r, pRenderTarget->GenFrameBuffers(LayerHeight,LayerWidth ), init_err);

init_err:
	return r;
}

void CLayerPass::RenderBegin(void *pArgs)
{
	pRenderTarget->BindForWrite();
	glViewport(0, 0,LayerWidth ,LayerHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CLayerPassShader *pCurrentShader = GetShaderHandle<CLayerPassShader>();
	pCurrentShader->BindShader();
	if( flag != 1){
	    CLayerPass &pass = *static_cast<CLayerPass*>(pArgs);
	    GLuint pMin = pass.GetMinBuffer();
	    pCurrentShader->SetMin(pMin, GL_TEXTURE0 + 20);
	    GLuint pMax = pass.GetMaxBuffer();
	    pCurrentShader->SetMax(pMax, GL_TEXTURE0 + 30);
	   }
	else{
		uint64_t pMin = reinterpret_cast<uint64_t>(pArgs);
	    pCurrentShader->SetMin(pMin, GL_TEXTURE0 + 20);
	    pCurrentShader->SetMax(pMin, GL_TEXTURE0 + 30);

	}
	pCurrentShader->SetHeight(LayerHeight*2);
	pCurrentShader->SetWidth(LayerWidth*2);

//	glBindVertexArray(pVertexArray);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindVertexArray(0);
}



void CLayerPass::RenderEnd()
{
	pRenderTarget->Unbind();
	glViewport(0, 0, CShaderProgram::ScreenWidth, CShaderProgram::ScreenHeight);

}

GLuint CLayerPass::GetMinBuffer()
{
	return pRenderTarget->GetFrameBufferTex(0);
}

GLuint CLayerPass::GetMaxBuffer()
{
	return pRenderTarget->GetFrameBufferTex(1);
}



