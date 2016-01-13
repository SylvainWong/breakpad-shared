LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE            := breakpad
LOCAL_SRC_FILES         := $(LOCAL_PATH)/breakpad_jni.cc
LOCAL_CPP_EXTENSION     := .cc
LOCAL_C_INCLUDES        := $(LOCAL_PATH)
LOCAL_STATIC_LIBRARIES  := breakpad_client
LOCAL_LDLIBS            := -llog

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/googlebreakpad/Android.mk
