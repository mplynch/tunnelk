package com.googlecode.tunnelk;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.SystemClock;
import android.view.View;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class VirtualTunnelActivity extends TunnelKActivity
{
	private TextView windSpeedTextView;
	private TextView altitudeTextView;
	private TextView angleOfAttackTextView;
	private ImageView shape;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.tunnelk);
        final TextView tv = (TextView) findViewById(R.id.solverOutput);

        windSpeedTextView = (TextView) findViewById(R.id.windSpeedDisplay);
        altitudeTextView = (TextView) findViewById(R.id.altitudeDisplay);
        angleOfAttackTextView = (TextView) findViewById(R.id.angleOfAttackDisplay);
        shape = (ImageView) findViewById(R.id.displayShapeImageView);

        mHandler = new Handler();

        readFileTask = new Runnable() {
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
    }

    private Handler mHandler;
    private Runnable readFileTask;

    public void setTunnelConditionsClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,SetTunnelConditionsActivity.class);
        startActivity(intent);
    }

    public void setAirfoilShapeClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,SetAirfoilShapeActivity.class);
        startActivity(intent);
    }

    public void rotateShapeClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,RotateShapeActivity.class);
        startActivity(intent);
    }

    public void simulateClicked(View v){
        doBindService();
        mHandler.removeCallbacks(readFileTask);
        mHandler.postDelayed(readFileTask, 5000);
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
            Toast.makeText(VirtualTunnelActivity.this, "onServiceConnected",
                    Toast.LENGTH_SHORT).show();
        }

        public void onServiceDisconnected(ComponentName className) {
            // This is called when the connection with the service has been
            // unexpectedly disconnected -- that is, its process crashed.
            // Because it is running in our same process, we should never
            // see this happen.
            mBoundService = null;
            Toast.makeText(VirtualTunnelActivity.this, "onServiceDisconnected",
                    Toast.LENGTH_SHORT).show();
        }
    };

    boolean mIsBound = false;
    
    void doBindService() {
        // Establish a connection with the service.  We use an explicit
        // class name because we want a specific service implementation that
        // we know will be running in our own process (and thus won't be
        // supporting component replacement by other applications).
        bindService(new Intent(VirtualTunnelActivity.this, 
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

    @Override
    public void onResume() {
        super.onResume();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);

        int windSpeedPos = p.getInt("@string/wind_speed",1);
        int altitudePos = p.getInt("@string/altitude",1);
        int anglePos = p.getInt("@string/angle_of_attack",0);

        if(windSpeedPos==0)
            windSpeedTextView.setText("Wind Speed: Low");
        if(windSpeedPos==1)
            windSpeedTextView.setText("Wind Speed: Medium");
        if(windSpeedPos==2)
            windSpeedTextView.setText("Wind Speed: High");

        if(altitudePos==0)
            altitudeTextView.setText("Altitude: Low");
        if(altitudePos==1)
            altitudeTextView.setText("Altitude: Medium");
        if(altitudePos==2)
            altitudeTextView.setText("Altitude: High");

        angleOfAttackTextView.setText("Angle of Attack: "+anglePos);

        String shapeString = p.getString("@string/tunnel_shape", "camber");
        if(shapeString.equals("camber"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.camber));
        if(shapeString.equals("circle"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.circle));
        if(shapeString.equals("diamond"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.diamond));
        if(shapeString.equals("naca_airfoil"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.naca_airfoil));
        if(shapeString.equals("pig"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.pig));
        if(shapeString.equals("prius"))
            shape.setImageDrawable(getResources().getDrawable(R.drawable.prius));
    }
}
