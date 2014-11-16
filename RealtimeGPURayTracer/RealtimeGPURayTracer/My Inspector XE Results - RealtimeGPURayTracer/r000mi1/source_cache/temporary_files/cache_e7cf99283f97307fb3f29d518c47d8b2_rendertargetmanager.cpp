#include "RenderTargetManager.h"

#include "utility.h"
#include "error.h"

#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

CRenderTargetManager::CRenderTargetManager(int numBuffers, bool hasZBuffer, int numMultiSampling)
{
	this->numBuffers = numBuffers;
	bZBuffer = hasZBuffer;
	this->numMultiSampling = numMultiSampling;

	if (numMultiSampling > 1) {
		pFrameBuffers = new GLuint[2];
		memset(pFrameBuffers, 0, sizeof(GLuint) * 2);

		pFBTextures = new GLuint[numBuffers * 2];
		memset(pFBTextures, 0, sizeof(GLuint) * numBuffers * 2);
	} else {
		pFrameBuffers = new GLuint[1];
		memset(pFrameBuffers, 0, sizeof(GLuint) * 1);

		pFBTextures = new GLuint[numBuffers];
		memset(pFBTextures, 0, sizeof(GLuint) * numBuffers);
	}

	if (bZBuffer) {
		for (int i = 0; i < numBuffers - 1; i++) {
			drawBufferFlags[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	} else {
		for (int i = 0; i < numBuffers; i++) {
			drawBufferFlags[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

}

CRenderTargetManager::~CRenderTargetManager(void)
{
	SAFE_DELETE_ARRAY(pFrameBuffers);
	SAFE_DELETE_ARRAY(pFBTextures);
}

void CRenderTargetManager::AddBuffer(GLint internalFormat, GLint format, GLint type, GLenum fbAttachment, bool needMipMap, int id)
{
	buffersInfo[id].internalFormat = internalFormat;
	buffersInfo[id].channelType = type;
	buffersInfo[id].format = format;
	buffersInfo[id].needMipMap = needMipMap;
	buffersInfo[id].fbAttachment = fbAttachment;
}

void CRenderTargetManager::GetTexFormat(GLint target, GLint internalFormat, GLint &format, GLint &type)
{
	glGetInternalformativ(target, internalFormat, GL_TEXTURE_IMAGE_FORMAT, 1, &format);
	glGetInternalformativ(target, internalFormat, GL_TEXTURE_IMAGE_TYPE, 1, &type);
}

int CRenderTargetManager::GenFrameBuffers(int width, int height)
{
	w = width;
	h = height;
	// multiSampling FrameBuffer
	if (numMultiSampling > 1) {
		// release all buffers and textures
		SAFE_DO(pFrameBuffers[1], glDeleteFramebuffers(1, &pFrameBuffers[1]));
		for (int i = 0; i < numBuffers; i++) {
			SAFE_DO(pFBTextures[numBuffers + i], glDeleteTextures(1, &pFBTextures[numBuffers + i]));
		}
		// gen buffers
		glGenFramebuffers(1, &pFrameBuffers[1]);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFrameBuffers[1]);

		// initialized buffers
		for (int i = 0; i < numBuffers; i++) {
			//GLint texFormat, texType;
			//GetTexFormat(GL_TEXTURE_2D_MULTISAMPLE, buffersInfo[i].internalFormat, texFormat, texType);
			//if (texFormat != buffersInfo[i].format || texType != buffersInfo[i].channelType) {
			printf("Texture type. if: %x; f: %x; t: %x\n.", buffersInfo[i].internalFormat, buffersInfo[i].format, buffersInfo[i].channelType);
			// buffersInfo[i].format = texFormat;
			// buffersInfo[i].channelType = texType;
			//}

			glGenTextures(1, &pFBTextures[numBuffers + i]);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, pFBTextures[numBuffers + i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numMultiSampling, buffersInfo[i].internalFormat, width, height, false);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, buffersInfo[i].fbAttachment, GL_TEXTURE_2D_MULTISAMPLE, pFBTextures[numBuffers + i], 0);
		}

		if (bZBuffer)
			glDrawBuffers(numBuffers - 1, drawBufferFlags);
		else glDrawBuffers(numBuffers, drawBufferFlags);

		GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fbStatus != GL_FRAMEBUFFER_COMPLETE) 
		{
			printf("FB error at Line %d: %x\n", __LINE__, fbStatus);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			return GET_ERROR_RETURN(ET_FRAMEBUFFER_CREATE_FAILURE);
		}
	}

	// release all buffers and textures
	SAFE_DO(pFrameBuffers[0], glDeleteFramebuffers(1, &pFrameBuffers[0]));
	for (int i = 0; i < numBuffers; i++) {
		SAFE_DO(pFBTextures[i], glDeleteTextures(1, &pFBTextures[i]));
	}
	// gen buffers
	glGenFramebuffers(1, &pFrameBuffers[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFrameBuffers[0]);

	// initialized buffers
	for (int i = 0; i < numBuffers; i++) {
		// GLint texFormat, texType;
		//GetTexFormat(GL_TEXTURE_2D_MULTISAMPLE, buffersInfo[i].internalFormat, texFormat, texType);
		//if (texFormat != buffersInfo[i].format || texType != buffersInfo[i].channelType) {
		printf("Texture type. if: %x; f: %x; t: %x\n.", buffersInfo[i].internalFormat, buffersInfo[i].format, buffersInfo[i].channelType);
		// buffersInfo[i].format = texFormat;
		// buffersInfo[i].channelType = texType;
		//}

		glGenTextures(1, &pFBTextures[i]);
		glBindTexture(GL_TEXTURE_2D, pFBTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, buffersInfo[i].internalFormat, width, height, 0, buffersInfo[i].format, buffersInfo[i].channelType, NULL);
		if (buffersInfo[i].needMipMap) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, buffersInfo[i].fbAttachment, GL_TEXTURE_2D, pFBTextures[i], 0);
	}

	if (bZBuffer)
		glDrawBuffers(numBuffers - 1, drawBufferFlags);
	else glDrawBuffers(numBuffers, drawBufferFlags);

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbStatus != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error at Line %d: %x\n", __LINE__, fbStatus);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return GET_ERROR_RETURN(ET_FRAMEBUFFER_CREATE_FAILURE);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return 0;
}

int CRenderTargetManager::Reshape(int width, int height)
{
	return GenFrameBuffers(width, height);
}

void CRenderTargetManager::BindForRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, pFrameBuffers[0]);
}

void CRenderTargetManager::BindForWrite()
{
	if (numMultiSampling > 1) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFrameBuffers[1]);
	} else {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFrameBuffers[0]);
	}

	int numBuffers = bZBuffer ? this->numBuffers - 1 : this->numBuffers;

	glDrawBuffers(numBuffers, drawBufferFlags);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CRenderTargetManager::Unbind()
{	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CRenderTargetManager::FlushBuffer()
{
	if (numMultiSampling > 1) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, pFrameBuffers[1]);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFrameBuffers[0]);

		int numBuffers = bZBuffer ? this->numBuffers - 1 : this->numBuffers;
		for (int i = 0; i < numBuffers; i++) {
			// copy color buffer
			glReadBuffer(drawBufferFlags[i]);
			glDrawBuffer(drawBufferFlags[i]);
			glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}

		if (bZBuffer)
			glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}