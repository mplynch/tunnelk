package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;

public class RotateShapeActivity extends TunnelKActivity
{
	float rotation=0;
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
        rotateShape(-10);
    }

    public void rotateCWClicked(View v){
        rotateShape(10);
    }

    public void rotateShape(float value){
        RotateAnimation anim = new RotateAnimation(rotation, rotation + value,
                Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,0.5f);
        rotation = (rotation + value) % 360;

        anim.setInterpolator(new LinearInterpolator());
        anim.setDuration(1000);
        anim.setFillEnabled(true);

        anim.setFillAfter(true);
        shape.startAnimation(anim);
    }
}
