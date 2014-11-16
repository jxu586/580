#ifndef _LIGHT_VIEW_PASS_H_
#define _LIGHT_VIEW_PASS_H_

#include "RenderPass.h"

class CLightViewPass :
	public CRenderPass
{
protected:
	int RSMLength;
public:
	CLightViewPass(int textureSize);
	~CLightViewPass(void);

	virtual int Init();
	virtual void RenderBegin(void *pArgs);
	virtual void RenderEnd();
};

#endif


