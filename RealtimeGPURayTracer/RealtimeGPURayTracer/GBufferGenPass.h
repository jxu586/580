#ifndef _GBUFFER_GEN_PASS_H_
#define _GBUFFER_GEN_PASS_H_

#include "RenderPass.h"

class CGBufferGenPass :
	public CRenderPass
{
public:
	CGBufferGenPass(void);
	~CGBufferGenPass(void);

	virtual int Init();
	virtual void RenderBegin(void *pArgs);
	virtual void RenderEnd();

	GLuint GetPositionBuffer();
	GLuint GetNormalBuffer();
	GLuint GetColorBuffer();
	GLuint GetZBuffer();
};


#endif

