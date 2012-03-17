package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class ShowResultsActivity extends TunnelKActivity
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
        intent.setClass(this,VirtualTunnelActivity.class);
        startActivity(intent);
    }
}
