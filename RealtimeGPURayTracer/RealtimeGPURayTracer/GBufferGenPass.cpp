#include "GBufferGenPass.h"
#include "GBufferGenPassShader.h"
#include "Camera.h"

#include "error.h"
#include "utility.h"

#include <iostream>

CGBufferGenPass::CGBufferGenPass(void)
{
}

CGBufferGenPass::~CGBufferGenPass(void)
{
	SAFE_DELETE(pShader);
	SAFE_DELETE(pRenderTarget);
}

int CGBufferGenPass::Init()
{
	int r = 0;
	bool bNeedMipMaps = false;
	pRenderTarget = new CRenderTargetManager(4, 1, 1);
	// init shader
	CGBufferGenPassShader *pGShader = new CGBufferGenPassShader;
	IF_FAILED(r, pGShader->Init(), init_err);
	pShader = (CShaderProgram*)pGShader;
	// color attachment
	pRenderTarget->AddBuffer(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, bNeedMipMaps, 0);
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1, bNeedMipMaps, 1);
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2, bNeedMipMaps, 2);
	// depth attachment
	pRenderTarget->AddBuffer(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT, bNeedMipMaps, 3);
	// generate frame buffer
	IF_FAILED(r, pRenderTarget->GenFrameBuffers(CShaderProgram::ScreenWidth, CShaderProgram::ScreenHeight), init_err);

	return 0;
init_err:
	return r;
}

void CGBufferGenPass::RenderBegin(void *pArgs)
{
	pRenderTarget->BindForWrite();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CGBufferGenPassShader *pCurrentShader = GetShaderHandle<CGBufferGenPassShader>();
	CCamera *pCamera = static_cast<CCamera*>(pArgs);
	glm::mat4 V, P, T;

	V = pCamera->GetViewMatrix();
	P = pCamera->GetProjMatrix();
	T = P * V;

	pCurrentShader->BindShader();
	pCurrentShader->SetEyePos(pCamera->GetEyePos());
	pCurrentShader->SetVPMat(T);
}

void CGBufferGenPass::RenderEnd()
{
	pRenderTarget->Unbind();
}

GLuint CGBufferGenPass::GetPositionBuffer()
{
	return pRenderTarget->GetFrameBufferTex(1);
}

GLuint CGBufferGenPass::GetNormalBuffer()
{
	return pRenderTarget->GetFrameBufferTex(2);
}

GLuint CGBufferGenPass::GetColorBuffer()
{
	return pRenderTarget->GetFrameBufferTex(0);
}

GLuint CGBufferGenPass::GetZBuffer()
{
	return pRenderTarget->GetFrameBufferTex(3);
}
