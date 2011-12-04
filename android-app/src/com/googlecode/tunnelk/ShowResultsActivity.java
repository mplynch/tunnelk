package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class ShowResultsActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.show_results);

    }

    public void newSimClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,TunnelkInitialActivity.class);
        startActivity(intent);
    }
}
