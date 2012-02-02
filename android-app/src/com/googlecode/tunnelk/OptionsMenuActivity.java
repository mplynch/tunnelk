package com.googlecode.tunnelk;

import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;

public class OptionsMenuActivity extends Activity {

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);
		return true;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.options_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent intent = new Intent();

		switch (item.getItemId()) {
		case R.id.start_over:
			intent.setClass(this, SplashActivity.class);
			startActivity(intent);
			return true;

		case R.id.virtual_home:
			intent.setClass(this, TunnelkInitialActivity.class);
			startActivity(intent);
			return true;

		case R.id.physical_home:
			intent.setClass(this, PhysicalTunnelHMIActivity.class);
			startActivity(intent);
			return true;
		}

		return super.onOptionsItemSelected(item);
	}
}
