package com.googlecode.tunnelk;

import java.io.IOException;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.IBinder;
import android.widget.TextView;
import android.widget.Toast;

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
            AssetFileDescriptor assFD = assetManager.openFd( "naca0012.mesh.png" );
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

        doBindService();
        
        setContentView(R.layout.main);
        TextView tv = (TextView) findViewById(R.id.solverOutput);
        tv.setHorizontallyScrolling(true);
        tv.setText(message);
    }
    
    @SuppressWarnings("unused")
	private LocalService mBoundService;

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            // This is called when the connection with the service has been
            // established, giving us the service object we can use to
            // interact with the service.  Because we have bound to a explicit
            // service that we know is running in our own process, we can
            // cast its IBinder to a concrete class and directly access it.
            mBoundService = ((LocalService.LocalBinder)service).getService();

            // Tell the user about this for our demo.
            Toast.makeText(TunnelkInitialActivity.this, "onServiceConnected",
                    Toast.LENGTH_SHORT).show();
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has been
            // unexpectedly disconnected -- that is, its process crashed.
            // Because it is running in our same process, we should never
            // see this happen.
            mBoundService = null;
            Toast.makeText(TunnelkInitialActivity.this, "onServiceDisconnected",
                    Toast.LENGTH_SHORT).show();
        }
    };

    boolean mIsBound = false;
    
    void doBindService() {
        // Establish a connection with the service.  We use an explicit
        // class name because we want a specific service implementation that
        // we know will be running in our own process (and thus won't be
        // supporting component replacement by other applications).
        bindService(new Intent(TunnelkInitialActivity.this, 
                LocalService.class), mConnection, Context.BIND_AUTO_CREATE);
        mIsBound = true;
    }

    void doUnbindService() {
        if (mIsBound) {
            // Detach our existing connection.
            unbindService(mConnection);
            mIsBound = false;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        doUnbindService();
    }
    
}
