package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.content.SharedPreferences;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.widget.Toast;
import java.util.Date;

public class ShowResultsActivity extends TunnelKActivity
{
	private SharedPreferences prefs;
	private final Handler mTwitterHandler = new Handler();

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
}
