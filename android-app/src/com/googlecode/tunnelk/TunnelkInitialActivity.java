package com.googlecode.tunnelk;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.SystemClock;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class TunnelkInitialActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.main);
        final TextView tv = (TextView) findViewById(R.id.solverOutput);

        final Handler mHandler = new Handler();

        final Runnable readFileTask = new Runnable() {
            public void run() {
               File outputDir = getDir("output",Context.MODE_WORLD_READABLE);
               File solverFile = new File(outputDir,"2dflowsolver_output.txt");
               StringBuilder text = new StringBuilder();
               try {
                    BufferedReader br = new BufferedReader(new FileReader(solverFile));
                    String line;

                    while ((line = br.readLine()) != null) {
                        text.append(line);
                        text.append('\n');
                    }
                }
                catch (IOException e) {
                    //You'll need to add proper error handling here
               }
               tv.setText(text);
               mHandler.postAtTime(this,SystemClock.uptimeMillis()+5000);
            }
        };

        final Button simulateButton = (Button) findViewById(R.id.simulate);
        simulateButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                doBindService();
                mHandler.removeCallbacks(readFileTask);
                mHandler.postDelayed(readFileTask, 5000);
            }
        });
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
