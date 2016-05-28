LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := gl_code.cpp, API_EntryPoint.cpp
LOCAL_MODULE    := gl2jni


APP_STL := gnustl_static

include $(BUILD_SHARED_LIBRARY)