package com.googlecode.tunnelk;

public class FlowSolver2d
{
    public native int init(int arg);

    public native int step();

    static {
        System.loadLibrary("2dFlowSolverJNI");
    }
}
