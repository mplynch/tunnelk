package com.googlecode.tunnelk;

import java.io.IOException;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
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
        String apkFilePath = null;
        long offset = -1;
        long fileSize = -1;
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = getPackageManager();
        try
        {
            appInfo = packMgmr.getApplicationInfo( "com.googlecode.tunnelk", 0 );
            apkFilePath = appInfo.sourceDir;
        }
        catch( PackageManager.NameNotFoundException e){
        }
        // Get the offset and length for the file that is in the assets folder
        AssetManager assetManager = getAssets();
        try {
            AssetFileDescriptor assFD = assetManager.openFd( "input.in.png" );
            if( assFD != null ) {
                offset = assFD.getStartOffset();
                fileSize = assFD.getLength();
                assFD.close();
            }
        }
        catch( IOException e) {}

        FlowSolver2d solver = new FlowSolver2d();
        message = "init = " + Integer.toString(solver.init(42,apkFilePath,offset,fileSize));
        message += "\nmsg = " + solver.getmsg();
        message += "\nstep = " + Integer.toString(solver.step());
        message += "\nstep = " + Integer.toString(solver.step());
        message += "\nstep = " + Integer.toString(solver.step());

        message += "\n\nlength = " + Long.toString(fileSize);
        message += "\noffset = " + Long.toString(offset);
        
        TextView tv = new TextView(this);
        tv.setText(message);
        setContentView(tv);
    }
}
