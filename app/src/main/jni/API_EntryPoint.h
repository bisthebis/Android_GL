//
// Created by boris on 26/05/2016.
//



#ifndef HELLO_GL_CLONE_API_ENTRYPOINT_H
#define HELLO_GL_CLONE_API_ENTRYPOINT_H

#include <jni.h>


extern "C" {
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};


#endif //HELLO_GL_CLONE_API_ENTRYPOINT_H
