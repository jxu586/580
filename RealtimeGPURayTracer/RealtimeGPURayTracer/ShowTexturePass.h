#ifndef _SHOW_TEXTURE_PASS__H_
#define _SHOW_TEXTURE_PASS__H_

#include "RenderPass.h"

class CShowTexturePass :
	public CRenderPass
{
protected:
	GLuint pVertexBuffer;
	GLuint pVertexArray;
public:
	CShowTexturePass(void);
	~CShowTexturePass(void);

	virtual int Init();
	virtual void RenderBegin(void *pArgs);
	virtual void RenderEnd();
};


#endif

