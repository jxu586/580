#ifndef _GLUT_HEADER_H_
#define _GLUT_HEADER_H_

#if defined(WIN32) || defined(linux)
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>

#endif

#endif