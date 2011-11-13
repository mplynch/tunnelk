package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class SetTunnelConditionsActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.set_tunnel_conditions);

        Spinner windSpeedSpinner = (Spinner) findViewById(R.id.windSpeedSpinner);
        Spinner temperatureSpinner = (Spinner) findViewById(R.id.temperatureSpinner);
        Spinner humiditySpinner = (Spinner) findViewById(R.id.humiditySpinner);

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
}
