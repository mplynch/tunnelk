// JNI wrapper of Jim Master's 2d flow solver

#include <jni.h>

extern "C" {
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg1);
   JNIEXPORT jstring Java_com_googlecode_tunnelk_FlowSolver2d_getmsg(JNIEnv* env, jobject obj);
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_step(JNIEnv* env, jobject obj);
};

static char* message = 0;

JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg)
{
   extern int foo(int, char**);
   return foo(arg, &message);
}

JNIEXPORT jstring Java_com_googlecode_tunnelk_FlowSolver2d_getmsg(JNIEnv* env, jobject obj)
{
   return env->NewStringUTF(message);
}

static int stepNumber = 0;

JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_step(JNIEnv* env, jobject obj)
{
   return stepNumber++;
}
