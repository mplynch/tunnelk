package com.googlecode.tunnelk;

import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class SplashActivity extends TunnelKActivity {
	@Override
	protected int getNarrationResourceId() {
		return R.raw.welcome;
	}

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

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.splash);
	}

	public void splashClicked(View v) {
		this.narrate();

		Intent intent = new Intent();
		intent.setClass(this, ChooseTunnelActivity.class);
		startActivity(intent);
	}
}
