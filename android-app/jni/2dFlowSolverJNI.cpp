// JNI wrapper of Jim Master's 2d flow solver

#include <jni.h>
#include <string>

extern "C" {
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg1,
                                                                       jstring filename, jlong offset, jlong length);
   JNIEXPORT jstring Java_com_googlecode_tunnelk_FlowSolver2d_getmsg(JNIEnv* env, jobject obj);
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_step(JNIEnv* env, jobject obj);
};

static std::string message;

JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg,
                                                                    jstring filename, jlong offset, jlong length)
{
   extern int foo(int, const char*, long, long, std::string&);
   jboolean iscopy;
   const char *mfile = env->GetStringUTFChars(filename,&iscopy);
   return foo(arg, mfile, offset, length, message);
}

JNIEXPORT jstring Java_com_googlecode_tunnelk_FlowSolver2d_getmsg(JNIEnv* env, jobject obj)
{
   return env->NewStringUTF(message.c_str());
}

static int stepNumber = 0;

JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_step(JNIEnv* env, jobject obj)
{
   return stepNumber++;
}
