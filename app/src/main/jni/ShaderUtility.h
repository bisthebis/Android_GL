//
// Created by boris on 28/05/2016.
//

#ifndef HELLO_GL_CLONE_SHADERUTILITY_H
#define HELLO_GL_CLONE_SHADERUTILITY_H

#include <GLES2/gl2.h>

GLuint loadShader(GLenum shaderType, const char* pSource);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

#endif //HELLO_GL_CLONE_SHADERUTILITY_H
