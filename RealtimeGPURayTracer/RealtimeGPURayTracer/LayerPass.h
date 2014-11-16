#ifndef _LAYER_PASS__H_
#define _LAYER_PASS__H_

#include "RenderPass.h"

class CLayerPass :
	public CRenderPass
{
protected:
	int LayerHeight;
	int LayerWidth;
	int flag;
	GLuint pVertexBuffer;
	GLuint pVertexArray;


public:
	CLayerPass(int w,int h,int flag);
	~CLayerPass(void);

	virtual int Init();
	virtual void RenderBegin(void *pArgs);
	void RenderStart(void *pArgs);
	virtual void RenderEnd();

	GLuint GetMinBuffer();
	GLuint GetMaxBuffer();
};

#endif
