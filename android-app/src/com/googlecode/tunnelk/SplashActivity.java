package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class SplashActivity extends TunnelKActivity {
	@Override
	protected boolean getNarrationOnStartEnabled() {
		return false;
	};

	@Override
	protected boolean getDefaultOptionsMenuEnabled() {
		return false;
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.splash);
	}

	public void splashClicked(View v) {
		this.narrate(R.raw.welcome);

		Intent intent = new Intent();
		intent.setClass(this, ChooseTunnelActivity.class);
		startActivity(intent);
	}
}
