#ifndef __swgl_h__
#define __swgl_h__

//#include "GLee.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//implement the following function call to archive the opengl pipeline
void swTranslated(GLdouble x, GLdouble y, GLdouble z);
void swScaled(GLdouble x, GLdouble y, GLdouble z);
void swRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

void swMatrixMode(GLenum mode);
void swLoadIdentity(void);
void swLoadMatrixd(const GLdouble * m);
void swMultMatrixd(const GLdouble * m);
void swPushMatrix(void);
void swPopMatrix(void);

void swuLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
 	           GLdouble centerX, GLdouble centerY, GLdouble centerZ,
 	           GLdouble upX, GLdouble upY, GLdouble upZ);
void swFrustum(	GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal);
void swuPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void swViewport(GLint x, GLint y, GLsizei width, GLsizei height);

bool swTransformation(const GLdouble h[4], GLdouble w[4]);

//---------------------------------------------------------------------------
//cghw2
//---------------------------------------------------------------------------
void writepixel(int x, int y, GLdouble r, GLdouble g, GLdouble b);
void writepixel(GLdouble x, GLdouble y, GLdouble z, GLdouble r, GLdouble g, GLdouble b);

//Bresenham's algorithm
//bool BresenhamLine(int x1, int y1, int x2, int y2, GLdouble r, GLdouble g, GLdouble b)
bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b);

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b);

bool swInitZbuffer(int width, int height);
bool swClearZbuffer();

#endif                  /* __swgl_h__ */
