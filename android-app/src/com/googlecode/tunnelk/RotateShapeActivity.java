package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class RotateShapeActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.rotate_shape);

    }

    public void simulateClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,TunnelkInitialActivity.class);
        startActivity(intent);
    }
}
