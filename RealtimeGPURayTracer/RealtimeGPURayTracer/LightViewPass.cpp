#include "LightViewPass.h"
#include "LightViewPassShader.h"

#include "SceneObject.h"
#include "utility.h"
#include "error.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

CLightViewPass::CLightViewPass(int textureSize)
{
	RSMLength = textureSize;
}

CLightViewPass::~CLightViewPass(void)
{
	SAFE_DELETE(pShader);
	SAFE_DELETE(pRenderTarget);
}

int CLightViewPass::Init()
{
	int r = 0;
	bool bNeedMipMaps = false;
	// init shader
	CLightViewPassShader *pLVShader = new CLightViewPassShader;
	IF_FAILED(r, pLVShader->Init(), init_err);
	pShader = (CShaderProgram*)pLVShader;

	// init framebuffer
	pRenderTarget = new CRenderTargetManager(4, 1, 1);
	// color attachment
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, bNeedMipMaps, 0);
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT1, bNeedMipMaps, 1);
	pRenderTarget->AddBuffer(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2, bNeedMipMaps, 2);
	// depth attachment
	pRenderTarget->AddBuffer(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT, bNeedMipMaps, 3);
	// generate frame buffer
	IF_FAILED(r, pRenderTarget->GenFrameBuffers(RSMLength, RSMLength), init_err);

	return 0;
init_err:
	return r;
}

void CLightViewPass::RenderBegin(void *pArgs)
{
	pRenderTarget->BindForWrite();
	glViewport(0, 0, RSMLength, RSMLength);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CLightViewPassShader *pCurrentShader = GetShaderHandle<CLightViewPassShader>();
	SPOT_LIGHT &light = *static_cast<SPOT_LIGHT*>(pArgs);
	glm::mat4 V, P, T;

	glm::vec3 center = light.lightPosition;
	glm::vec3 at = center - light.lightDir * 100.0f;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	V = glm::lookAt(center, at, up);
	P = glm::perspective(light.fov * 2.0f, 1.0f, 0.1f, 100.0f);

	T = P * V;

	pCurrentShader->BindShader();
	pCurrentShader->SetVPMat(T);
	pCurrentShader->SetSpotLight(light);
	pCurrentShader->SetReflectiveCoeff(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CLightViewPass::RenderEnd()
{
	pRenderTarget->Unbind();
	glViewport(0, 0, CShaderProgram::ScreenWidth, CShaderProgram::ScreenHeight);
}
