# Build settings for Android technology

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := 2dFlowSolver
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := driver.cpp convert.cpp flowSolve.cpp \
readinput.cpp readmesh.cpp readCmplxmesh.cpp tristat.cpp fluxp.cpp fluxm.cpp \
dfluxp.cpp dfluxm.cpp dfluxWall.cpp lu_solve.cpp gplot.cpp tplot.cpp pplot.cpp \
gradx.cpp grady.cpp fluxV.cpp dfluxV.cpp List.cpp cost.cpp printResid.cpp
include $(BUILD_STATIC_LIBRARY)
