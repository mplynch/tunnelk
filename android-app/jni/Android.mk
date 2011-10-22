# Build settings for the Java Native Interface (JNI) Android wrapper of Jim
# Master's 2d flow solver.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := 2dFlowSolverJNI
LOCAL_SRC_FILES := 2dFlowSolverJNI.cpp
LOCAL_STATIC_LIBRARIES := 2dFlowSolver
include $(BUILD_SHARED_LIBRARY)

$(call import-module,2dFlowSolver)
