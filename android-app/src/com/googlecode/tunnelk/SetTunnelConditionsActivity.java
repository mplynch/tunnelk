package com.googlecode.tunnelk;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class SetTunnelConditionsActivity extends TunnelKActivity
{
	private Spinner windSpeedSpinner;
	private Spinner temperatureSpinner;
	private Spinner pressureSpinner;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.set_tunnel_conditions);

        windSpeedSpinner = (Spinner) findViewById(R.id.windSpeedSpinner);
        temperatureSpinner = (Spinner) findViewById(R.id.temperatureSpinner);
        pressureSpinner = (Spinner) findViewById(R.id.pressureSpinner);

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                this, R.array.low_med_high_array, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        windSpeedSpinner.setAdapter(adapter);
        temperatureSpinner.setAdapter(adapter);
        pressureSpinner.setAdapter(adapter);

    }
    public void nextClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,SetAirfoilShapeActivity.class);
        startActivity(intent);
    }

    @Override
    public void onPause() {
        super.onPause();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);
        SharedPreferences.Editor e = p.edit();

        e.putInt("@string/wind_speed", windSpeedSpinner.getSelectedItemPosition());
        e.putInt("@string/temperature", temperatureSpinner.getSelectedItemPosition());
        e.putInt("@string/pressure", pressureSpinner.getSelectedItemPosition());

        e.commit();
    }

    @Override
    public void onResume() {
        super.onResume();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);

        int windSpeedPos = p.getInt("@string/wind_speed",1);
        int temperaturePos = p.getInt("@string/temperature",1);
        int pressurePos = p.getInt("@string/pressure",1);

        windSpeedSpinner.setSelection(windSpeedPos);
        temperatureSpinner.setSelection(temperaturePos);
        pressureSpinner.setSelection(pressurePos);
    }
}
