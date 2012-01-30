package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class ShowResultsActivity extends OptionsMenuActivity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
                
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
                                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.show_results);

    }

    public void newSimClicked(View v){
        Intent intent = new Intent();
        intent.setClass(this,TunnelkInitialActivity.class);
        startActivity(intent);
    }
}
