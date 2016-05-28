/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <map>
#include <sstream>
#include <vector>
//#define _GLIBCXX_USE_C99_MATH 1
//#include <glm/mat4x4.hpp>


#include "gl_code.h"
#include "ShaderUtility.h"
#include "CommonGL_Utility.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


/*
auto gVertexShader =
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";
    //*/
auto gVertexShader =
        "attribute vec4 vPosition;\n"
                //"uniform vec3 color;"
                "uniform mat4 projection;"
                "void main() {\n"
                "  gl_Position = projection * vPosition;\n"
                "}\n";


auto gFragmentShader =
    "precision mediump float;\n"
    "uniform vec3 color; \n"
    "void main() {\n"
    "  gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

GLuint gProgram;
GLint gvPositionHandle;
GLint gProjectionHandle;
GLint gColorHandle;
std::vector<float> ortho;

//TMP en attendant glm :

std::vector<float> getOrtho(float left, float right, float top, float bottom, float near, float far)
{
    std::vector<float> matrix;
    matrix.resize(16, 0);
    matrix[0] = 2/(right-left);
    matrix[3] = -((right+left)/(right-left));
    matrix[5] = 2/(top-bottom);
    matrix[7] = -((top+bottom)/(top-bottom));
    matrix[10] = -2 / (far - near);
    matrix[11] = -((far+near)/(far-near));
    matrix[15] = 1;
    return matrix;

}

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    //TMP avant GLM
    float ratio = (float)w/(float)h;
    ortho = getOrtho(-1, 1, 1.0f/ratio, -1.0f/ratio, -1, 1);
    gProjectionHandle = glGetUniformLocation(gProgram, "projection");
    gColorHandle = glGetUniformLocation(gProgram, "color");

    LOGI("setupGraphics(%d, %d)", w, h);
    if(glIsProgram(gProgram))
        glDeleteProgram(gProgram); //When screen gets resized, remove old program to avoid leak
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    gProjectionHandle = glGetUniformLocation(gProgram, "projection");
    gColorHandle = glGetUniformLocation(gProgram, "color");
    LOGI("Color, Projection : %d, %d", gColorHandle, gProjectionHandle);

    static unsigned int N_times = 0;
    ++N_times;
    std::ostringstream ss;
    ss << "Number of render calls : " << N_times;
    LOGI(ss.str().c_str());

    //glm::mat4 ortho;
    glUniformMatrix4fv(1, 1, GL_FALSE, &ortho[0]);

    if((N_times/20)%3 == 0)
        glUniform3f(gColorHandle, 1, 0, 0);
    else if((N_times/20)%3 == 1)
        glUniform3f(gColorHandle, 0, 1, 0);
    else if((N_times/20)%3 == 2)
        glUniform3f(gColorHandle, 0, 0, 1);

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}



