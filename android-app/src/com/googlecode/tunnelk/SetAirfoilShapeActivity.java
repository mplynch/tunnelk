package com.googlecode.tunnelk;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.os.Bundle;
import android.view.View;

public class SetAirfoilShapeActivity extends TunnelKActivity
{
	private ImageButton camberButton;
	private ImageButton circleButton;
	private ImageButton diamondButton;
	private ImageButton nacaAirfoilButton;
	private ImageButton pigButton;
	private ImageButton priusButton;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.set_airfoil_shape);

        camberButton = (ImageButton) findViewById(R.id.camberButton);
        circleButton = (ImageButton) findViewById(R.id.circleButton);
        diamondButton = (ImageButton) findViewById(R.id.diamondButton);
        nacaAirfoilButton = (ImageButton) findViewById(R.id.nacaAirfoilButton);
        pigButton = (ImageButton) findViewById(R.id.pigButton);
        priusButton = (ImageButton) findViewById(R.id.priusButton);

    }
    public void buttonClicked(View v){
		Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);
        SharedPreferences.Editor e = p.edit();
        if(v==camberButton) {
            e.putString("@string/tunnel_shape","camber");
        }
        else if (v==circleButton) {
            e.putString("@string/tunnel_shape","circle");
        }
        else if (v==diamondButton) {
            e.putString("@string/tunnel_shape","diamond");
        }
        else if (v==nacaAirfoilButton) {
            e.putString("@string/tunnel_shape","naca_airfoil");
        }
        else if (v==pigButton) {
            e.putString("@string/tunnel_shape","pig");
        }
        else if (v==priusButton) {
            e.putString("@string/tunnel_shape","prius");
        }
        e.commit();
        Intent intent = new Intent();
        intent.setClass(this,RotateShapeActivity.class);
        startActivity(intent);
    }
}
