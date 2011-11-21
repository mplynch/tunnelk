package com.googlecode.tunnelk;

public class FlowSolver2d
{
    public native int init(String meshfile, String outputfile, String solfile, long offset, long fileSize);

    public native String getmsg();
    
    public native int step();

    static {
        System.loadLibrary("2dFlowSolverJNI");
    }
}
