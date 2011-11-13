package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class SetTunnelConditionsActivity extends Activity
{
	private Spinner windSpeedSpinner;
	private Spinner temperatureSpinner;
	private Spinner humiditySpinner;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.set_tunnel_conditions);

        windSpeedSpinner = (Spinner) findViewById(R.id.windSpeedSpinner);
        temperatureSpinner = (Spinner) findViewById(R.id.temperatureSpinner);
        humiditySpinner = (Spinner) findViewById(R.id.humiditySpinner);

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
                this, R.array.low_med_high_array, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        windSpeedSpinner.setAdapter(adapter);
        temperatureSpinner.setAdapter(adapter);
        humiditySpinner.setAdapter(adapter);

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
        e.putInt("@string/humidity", humiditySpinner.getSelectedItemPosition());

        e.commit();
    }

    @Override
    public void onResume() {
        super.onResume();
        Context context = getApplicationContext();
        SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);

        int windSpeedPos = p.getInt("@string/wind_speed",1);
        int temperaturePos = p.getInt("@string/temperature",1);
        int humidityPos = p.getInt("@string/humidity",1);

        windSpeedSpinner.setSelection(windSpeedPos);
        temperatureSpinner.setSelection(temperaturePos);
        humiditySpinner.setSelection(humidityPos);
    }
}
