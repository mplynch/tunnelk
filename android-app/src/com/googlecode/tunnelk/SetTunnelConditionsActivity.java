package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class SetTunnelConditionsActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.set_tunnel_conditions);

    }
    public void nextClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,SetAirfoilShapeActivity.class);
        startActivity(intent);
    }
}
