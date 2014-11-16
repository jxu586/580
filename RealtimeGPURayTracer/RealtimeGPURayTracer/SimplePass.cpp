#include "SimplePass.h"
#include "SimplePassShader.h"
#include "Camera.h"
#include "error.h"

#include <iostream>

CSimplePass::CSimplePass(void)
{
}


CSimplePass::~CSimplePass(void)
{
}

int CSimplePass::Init()
{
	int r = 0;
	CSimplePassShader *pSimple = new CSimplePassShader;

	IF_FAILED(r, pSimple->Init(), err_init);
	pShader = (CShaderProgram *)pSimple;

	return 0;

err_init:
	return r;
}

void CSimplePass::RenderBegin(void *pArgs)
{
	CSimplePassShader *pSimple = dynamic_cast<CSimplePassShader*>(pShader);
	CCamera *pCamera = static_cast<CCamera*>(pArgs);
	glm::mat4 V, P, T;

	V = pCamera->GetViewMatrix();
	P = pCamera->GetProjMatrix();
	T = P * V;

	pSimple->BindShader();
	pSimple->SetWVPMat(T);
}

void CSimplePass::RenderEnd()
{

}