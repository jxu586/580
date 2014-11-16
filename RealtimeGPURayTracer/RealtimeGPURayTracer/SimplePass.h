#ifndef _SIMPLE_PASS_H_
#define _SIMPLE_PASS_H_

#include "RenderPass.h"

class CSimplePass :
	public CRenderPass
{
public:
	CSimplePass(void);
	virtual ~CSimplePass(void);

	virtual int Init();
	virtual void RenderBegin(void *pArgs);
	virtual void RenderEnd();
};

#endif

