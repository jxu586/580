#ifndef _OPENGL_HEADER_H_
#define _OPENGL_HEADER_H_

#if defined(WIN32) || defined(linux)
#include <GL/glew.h>

#elif defined(__APPLE__)
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#endif

#endif