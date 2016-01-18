/*
 * breakpad_jni.c
 *
 * Created by Wang Siyuan
 *
 * This file is a part of breakpad which defines some jni interfaces to Java. 
 * 
 */

#include <jni.h>
#include <android/log.h>
#include <sys/stat.h>

#include "googlebreakpad/src/client/linux/handler/exception_handler.h"
#include "googlebreakpad/src/client/linux/handler/minidump_descriptor.h"


#define BREAKPAD_CLASS "com/ijinshan/base/app/BreakPad"

#define TAG "BREAKPAD"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, TAG, __VA_ARGS__)


google_breakpad::MinidumpDescriptor * descriptor = NULL;
google_breakpad::ExceptionHandler * exceptionHandler = NULL;


bool dump_callback(const google_breakpad::MinidumpDescriptor & descriptor,
                   void * context,
                   bool succeeded)
{
  LOGD("%s\n", __func__);

  LOGD("Dump path: %s\n", descriptor.path());
  return succeeded;
}


static jint jni_native_init(JNIEnv * env, jclass clazz, jstring path)
{
  LOGD("%s\n", __func__);

  const char * c_path = NULL;
  // TODO: check if c_path is available here.
  c_path = env->GetStringUTFChars(path, NULL);

  struct stat st;
  if (stat(c_path, &st) == 0 && st.st_mode & S_IFDIR)
  {
    LOGD("%s directory is already exist\n", c_path);
  }
  else if (ENOENT == errno)
  {
    if (mkdir(c_path, 0770) != 0)
    {
      LOGE("Create %s directory failed.\n", c_path);
      return -1;
    }
    else
    {
      LOGD("Create %s directory succeeded.\n", c_path);
    }
  }
  else
  {
    LOGE("Unknown error when check %s directory.\n", c_path);
    return -1;
  }

  descriptor = new google_breakpad::MinidumpDescriptor(c_path);
  exceptionHandler = new google_breakpad::ExceptionHandler(*descriptor,
                                                           NULL,
                                                           dump_callback,
                                                           NULL,
                                                           true,
                                                           -1);

  return 0;
}


static jint jni_native_crash(JNIEnv * env, jclass clazz)
{
  LOGD("%s\n", __func__);
  // TODO: this method is called by java to generate a native crash to test 
  // breakpad.
  volatile int* a = reinterpret_cast<volatile int*>(NULL);
  *a = 1;
  return 0;
}


static JNINativeMethod g_methods[] = {
  {"nativeInit", "(Ljava/lang/String;)I", (void*)jni_native_init},
  {"nativeCrash", "()I", (void*)jni_native_crash},
};


static int register_natives(JNIEnv * env)
{
  LOGD("%s\n", __func__);

  jclass clazz;
  clazz = env->FindClass(BREAKPAD_CLASS);
  if (clazz == NULL)
  {
    return JNI_FALSE;
  }

  int numMethods = sizeof(g_methods) / sizeof(g_methods[0]);
  if (env->RegisterNatives(clazz, g_methods, numMethods) < 0)
  {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


JNIEXPORT jint JNI_OnLoad(JavaVM * vm, void * reserved)
{
  LOGD("%s\n", __func__);

  // TODO: add some log here.
  JNIEnv * env = NULL;

  if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) 
  {
    return -1;
  }
  assert(env != NULL);

  if (!register_natives(env))
  {
    return -1;
  }

  return JNI_VERSION_1_4;
}


JNIEXPORT void JNI_OnUnload(JavaVM * jvm, void * reserved)
{
  LOGD("%s\n", __func__);

  if (descriptor != NULL)
  {
    delete descriptor;
    descriptor = NULL;
  }
  if (exceptionHandler != NULL)
  {
    delete exceptionHandler;
    exceptionHandler = NULL;
  }
}