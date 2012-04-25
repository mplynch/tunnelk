package com.googlecode.tunnelk;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import java.util.Date;

public class ShowResultsActivity extends TunnelKActivity
{
	private SharedPreferences prefs;
	private final Handler mTwitterHandler = new Handler();
	private String outputType = "mach";
	ImageView resultsImage;

    final Runnable mUpdateTwitterNotification = new Runnable() {
        public void run() {
            Toast.makeText(getBaseContext(), "Tweet sent !", Toast.LENGTH_LONG).show();
        }
    };

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        this.prefs = PreferenceManager.getDefaultSharedPreferences(this);

        setContentView(R.layout.show_results);

        resultsImage = (ImageView) findViewById(R.id.resultsImageView);

    }

    public void newSimClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,VirtualTunnelActivity.class);
        startActivity(intent);
    }

	/**
	 * Send a tweet. If the user hasn't authenticated to Tweeter yet, he'll be redirected via a browser
	 * to the twitter login page. Once the user authenticated, he'll authorize the Android application to send
	 * tweets on the users behalf.
	 */
    public void tweetClicked(View v) {
        if (TwitterUtils.isAuthenticated(prefs)) {
            sendTweet();
        } else {
		    Intent i = new Intent(getApplicationContext(), PrepareRequestTokenActivity.class);
		    i.putExtra("tweet_msg",getTweetMsg());
		    startActivity(i);
        }
    }

	private String getTweetMsg() {
		return "Tweeting from Android App at " + new Date().toLocaleString();
	}

    public void sendTweet() {
        Thread t = new Thread() {
            public void run() {

                try {
                    TwitterUtils.sendTweet(prefs,getTweetMsg());
                    mTwitterHandler.post(mUpdateTwitterNotification);
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }

        };
        t.start();
    }

    public void machClicked(View v) {
        outputType = "mach";
        onResume();
    }

    public void pressureClicked(View v) {
        outputType = "pressure";
        onResume();
    }

    public void densityClicked(View v) {
        outputType = "density";
        onResume();
    }

    @Override
    public void onResume() {
        super.onResume();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);
        String filename;
        String[] machArray = {"m0.6","m0.8","m1.0"};
        String[] densityArray = {"d1.0","d0.75","d0.5"};

        filename = p.getString("@string/tunnel_shape", "camber");
        filename+="_";
        int windSpeedPos = p.getInt("@string/wind_speed",1);
        int altitudePos = p.getInt("@string/altitude",1);
        int anglePos = p.getInt("@string/angle_of_attack",0);
        filename+=machArray[windSpeedPos];
        filename+="_";
        filename+=densityArray[altitudePos];
        filename+="_a"+anglePos+".0";
        filename+="_";
        filename+=outputType;
        filename+=".jpg";

        Bitmap bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory()+"/results/"+filename);
        if(bitmap!=null)
           resultsImage.setImageBitmap(bitmap);

        //Log.i("ShowResults", filename);
    }
}
