#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "ShaderProgram.h"
#include "RenderTargetManager.h"

class CRenderPass
{
protected:
	CShaderProgram *pShader;
	CRenderTargetManager *pRenderTarget;
public:
	CRenderPass() : pShader(0), pRenderTarget(0) {}
	~CRenderPass() {}

	virtual int Init() = 0;
	virtual void RenderBegin(void *pArgs = nullptr) = 0;
	virtual void RenderEnd() = 0;

	template<class T>
	T *GetShaderHandle() { return dynamic_cast<T*>(pShader); }
	CRenderTargetManager *GetFBHandle() { return pRenderTarget; }
};

#endif