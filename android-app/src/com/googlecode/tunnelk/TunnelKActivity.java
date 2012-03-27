package com.googlecode.tunnelk;

import java.util.Date;

import android.app.Activity;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

public class TunnelKActivity extends Activity {
	protected Date lastInteraction;

	/**
	 * Returns whether or not the default options menu should be displayed in
	 * this activity. Inheriting classes should override this method and return
	 * false to disable the default options menu. The menu is enabled for all
	 * subclasses by default.
	 * 
	 * @return true or false
	 */
	protected boolean getDefaultOptionsMenuEnabled() {
		return true;
	}

	/**
	 * Returns whether or not narration should take place when this activity's
	 * onStart method is executed. Inheriting classes should override this
	 * method and return true to enable narration each time the activity is
	 * started.
	 * 
	 * @return true or false
	 */
	protected boolean getNarrationOnStartEnabled() {
		return false;
	}

	/**
	 * Returns the resource ID of the default sound resource to play as
	 * narration for this activity.
	 * 
	 * @return the resource ID or 0 if no narration should take place
	 */
	protected int getNarrationResourceId() {
		return 0;
	}

	/**
	 * Plays the audio narration file specified by this class'
	 * getNarrationResourceId() method.
	 */
	protected void narrate() {
		int resourceId = getNarrationResourceId();

		if (resourceId == 0)
			return;

		narrate(resourceId);
	}

	/**
	 * Plays the audio narration file with the specified resourceId.
	 * 
	 * @param resouceId
	 *            the id
	 */
	protected void narrate(int resourceId) {
		MediaPlayer mediaPlayer = MediaPlayer.create(this, resourceId);

		if (mediaPlayer != null)
			mediaPlayer.start();
	}

	/**
	 * Shows a toast on this activity with the specified message.
	 * 
	 * @param message
	 *            the message
	 */
	public void showToast(final String message, final int duration) {
		final TunnelKActivity activity = this;
		
		runOnUiThread(new Runnable() {
			public void run() {
				Toast toast = Toast.makeText(activity, message, duration);
				toast.show();
			}
		});
	}

	/**
	 * Starts the splash screen activity for the application.
	 */
	public void startSplashActivity() {
		Intent intent = new Intent();
		intent.setClass(this, SplashActivity.class);
		startActivity(intent);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Make the volume buttons adjust the music volume
		setVolumeControlStream(AudioManager.STREAM_MUSIC);

		// Remove the title bar.
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Enable full screen mode, and prevent the screen from turning off.
		getWindow().addFlags(
				WindowManager.LayoutParams.FLAG_FULLSCREEN
						| WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);

		if (getDefaultOptionsMenuEnabled()) {
			MenuInflater inflater = getMenuInflater();
			inflater.inflate(R.menu.options_menu, menu);
		}

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
			intent.setClass(this, VirtualTunnelActivity.class);
			startActivity(intent);
			return true;

		case R.id.physical_home:
			intent.setClass(this, PhysicalTunnelHMIActivity.class);
			startActivity(intent);
			return true;
		}

		return super.onOptionsItemSelected(item);
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);
		return true;
	}

	@Override
	protected void onResume() {
		super.onResume();
		TunnelKApplication.getInstance().setCurrentActivity(this);
	}

	@Override
	protected void onStart() {
		super.onStart();

		if (getNarrationOnStartEnabled())
			narrate();
	}

	@Override
	public void onUserInteraction() {
		super.onUserInteraction();

		TunnelKApplication.getInstance().recordInteraction();
	}
}