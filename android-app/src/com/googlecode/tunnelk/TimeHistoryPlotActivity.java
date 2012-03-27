package com.googlecode.tunnelk;

import java.util.Arrays;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.googlegode.tunnelk.views.*;

public class TimeHistoryPlotActivity extends TunnelKActivity implements
		View.OnClickListener {
	@Override
	protected boolean getDefaultOptionsMenuEnabled() {
		return false;
	}

	private TagTimeHistoryPlot plot;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Intent sender = getIntent();
		String[] names = sender.getStringArrayExtra(getResources().getString(
				R.string.trending_tags));
		plot = TagLayoutFactory.createPlot(this, Arrays.asList(names));
		
		plot.setClickListener(this);

		setContentView(plot);
	}
	
	public void onClick(View v){
		setResult(RESULT_OK);
		finish();
	}
}
