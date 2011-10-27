package com.googlecode.tunnelk;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

public class TunnelkInitialActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        String message;
        
        FlowSolver2d solver = new FlowSolver2d();
        message = "init = " + Integer.toString(solver.init(42));
        message += "\nmsg = " + solver.getmsg();
        message += "\nstep = " + Integer.toString(solver.step());
        message += "\nstep = " + Integer.toString(solver.step());
        message += "\nstep = " + Integer.toString(solver.step());
        
        TextView tv = new TextView(this);
        tv.setText(message);
        setContentView(tv);
    }
}
