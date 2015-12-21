LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := jni_test
LOCAL_SRC_FILES := jni_test.cpp
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
