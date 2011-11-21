// JNI wrapper of Jim Master's 2d flow solver

#include <jni.h>
#include <string>

extern "C" {
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg1,
                                                                       jstring meshfile, jstring outputfile,
                                                                       jstring solfile, jlong offset, jlong length);
   JNIEXPORT jstring Java_com_googlecode_tunnelk_FlowSolver2d_getmsg(JNIEnv* env, jobject obj);
   JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_step(JNIEnv* env, jobject obj);
};

std::string message;
const char* mesh_filename = 0;
const char* stdout_filename = 0;
const char* solution_filename = 0;
long mesh_offset = 0;

JNIEXPORT int JNICALL Java_com_googlecode_tunnelk_FlowSolver2d_init(JNIEnv* env, jobject obj, jint arg1,
                                                                    jstring meshfile, jstring outputfile,
                                                                    jstring solfile, jlong offset, jlong length)
{
   jboolean iscopy;

   mesh_filename = env->GetStringUTFChars(meshfile, &iscopy);
   stdout_filename = env->GetStringUTFChars(outputfile, &iscopy);
   solution_filename = env->GetStringUTFChars(solfile, &iscopy);
   mesh_offset = offset;

   extern int run();
   run();

   return 0;
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
