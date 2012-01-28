package com.googlecode.tunnelk;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.TextView;

public class PhysicalTunnelHMIActivity extends Activity implements SeekBar.OnSeekBarChangeListener {	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.physical_tunnel_hmi);

        SeekBar seekBarFanSpeed = (SeekBar) findViewById(R.id.seekBarFanSpeed);
        
        seekBarFanSpeed.setOnSeekBarChangeListener(this);
    }
    
    public void onProgressChanged(SeekBar seekBar, int progress,
			boolean fromUser) {
    	TextView textViewFanSpeed = (TextView) findViewById(R.id.textViewFanSpeedPercent);
    	
    	textViewFanSpeed.setText(progress + "%");
    	
    	// TODO Notify subscribers of change
	}

	public void onStartTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}

	public void onStopTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub
		
	}    
}
