package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class SetAirfoilShapeActivity extends TunnelKActivity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.set_airfoil_shape);

    }
    public void drawShapeClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,DrawShapeActivity.class);
        startActivity(intent);
    }
    public void nextClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,RotateShapeActivity.class);
        startActivity(intent);
    }
}
