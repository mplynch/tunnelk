package com.googlecode.tunnelk;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;

public class RotateShapeActivity extends TunnelKActivity
{
	int rotation=0;
	ImageView shape;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.rotate_shape);
        shape = (ImageView) findViewById(R.id.rotateShapeImageView);
    }

    public void simulateClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,VirtualTunnelActivity.class);
        startActivity(intent);
    }

    public void rotateCCWClicked(View v){
        if(rotation>=0)
            rotateShape(-5);
    }

    public void rotateCWClicked(View v){
        if(rotation<=20)
            rotateShape(5);
    }

    public void rotateShape(int value){
        RotateAnimation anim = new RotateAnimation(rotation, rotation + value,
                Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,0.5f);
        rotation = rotation + value;

        anim.setInterpolator(new LinearInterpolator());
        anim.setDuration(1000);
        anim.setFillEnabled(true);

        anim.setFillAfter(true);
        shape.startAnimation(anim);
    }
    @Override
    public void onPause() {
        super.onPause();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);
        SharedPreferences.Editor e = p.edit();

        e.putInt("@string/angle_of_attack", rotation);

        e.commit();
    }

    @Override
    public void onResume() {
        super.onResume();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);

        rotation = p.getInt("@string/angle_of_attack",0);
        rotateShape(0);

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
