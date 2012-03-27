package com.googlecode.tunnelk;

import java.util.Calendar;

import android.app.Application;
import android.content.Intent;
import android.widget.Toast;

public class TunnelKApplication extends Application {
	/**
	 * The frequency with which to check for inactivity, in milliseconds.
	 */
	public final int sleepTime = 1000;

	/**
	 * The time to wait before starting splash screen, in milliseconds.
	 */
	public final int timeoutPeriod = 30000;

	/**
	 * The intent used to start the application's splash screen.
	 */
	public static final String SPLASH_INTENT = "com.googlecode.tunnelk.intent.action.START_SPLASH_SCREEN";

	private static TunnelKApplication instance;

	private TunnelKActivity currentActivity;

	private long lastInteraction;

	@Override
	public void onCreate() {
		super.onCreate();

		instance = this;

		this.startInactivityMonitor();
	}

	/**
	 * Gets the singleton instance of the this class.
	 * 
	 * @return the instance
	 */
	public static TunnelKApplication getInstance() {
		if (TunnelKApplication.instance == null)
			throw new IllegalStateException("An attempt was made to retrieve"
					+ " the singleton TunnelKApplication instance before it"
					+ " was created.");

		return instance;
	}

	/**
	 * Starts a thread which monitors the application for inactivity.
	 */
	private void startInactivityMonitor() {
		lastInteraction = Calendar.getInstance().getTimeInMillis();

		new Thread(new Runnable() {
			public void run() {
				boolean toastShown = false;
				long currentTime;

				while (true) {
					try {
						Thread.sleep(sleepTime);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

					if (currentActivity.getClass() == SplashActivity.class)
						continue;

					currentTime = Calendar.getInstance().getTimeInMillis();

					if ((currentTime > lastInteraction + timeoutPeriod - 5000)
							&& !toastShown) {
						toastShown = true;
						
						currentActivity.showToast(getString(R.string.timeout),
								5000);
					}

					if (currentTime > lastInteraction + timeoutPeriod) {
						lastInteraction = currentTime;
						
						toastShown = false;

						currentActivity.startSplashActivity();
					}
				}
			}
		}).start();
	}

	/**
	 * Records the time stamp for the last interaction received by this
	 * application.
	 */
	public void recordInteraction() {
		this.lastInteraction = Calendar.getInstance().getTimeInMillis();
	}

	/**
	 * Sets the current activity in the application. This method is called in
	 * the onResume method of activities belonging to the application.
	 */
	public void setCurrentActivity(TunnelKActivity activity) {
		currentActivity = (TunnelKActivity) activity;
	}
}