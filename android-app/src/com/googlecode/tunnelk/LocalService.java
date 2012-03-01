package com.googlecode.tunnelk;

import java.io.IOException;
import java.io.File;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

public class LocalService extends Service {
	private NotificationManager mNM;

	// Unique Identification Number for the Notification.
	// We use it on Notification start, and to cancel it.
	private int NOTIFICATION = 42; // R.string.local_service_started;

	private Thread solverThread = null;

	/**
	 * Class for clients to access. Because we know this service always runs in
	 * the same process as its clients, we don't need to deal with IPC.
	 */
	public class LocalBinder extends Binder {
		LocalService getService() {
			return LocalService.this;
		}
	}

	@Override
	public void onCreate() {
		mNM = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

		// Display a notification about us starting. We put an icon in the
		// status bar.
		showNotification();
		solverThread = new Thread() {
			@Override
			public void run() {

				String message;
				String apkFilePath = null;
				long offset = -1;
				long fileSize = -1;
				ApplicationInfo appInfo = null;
				PackageManager packMgmr = getPackageManager();
				try {
					appInfo = packMgmr.getApplicationInfo(
							"com.googlecode.tunnelk", 0);
					apkFilePath = appInfo.sourceDir;
				} catch (PackageManager.NameNotFoundException e) {
				}
				// Get the offset and length for the file that is in the assets
				// folder
				AssetManager assetManager = getAssets();
				try {
					AssetFileDescriptor assFD = assetManager
							.openFd("naca0012.mesh.png");
					if (assFD != null) {
						offset = assFD.getStartOffset();
						fileSize = assFD.getLength();
						assFD.close();
					}
				} catch (IOException e) {
				}
                File outputDir = getDir("output",Context.MODE_WORLD_READABLE);
                File outputFile = new File(outputDir,"2dflowsolver_output.txt");
                File solutionFile = new File(outputDir,"solution.dat");
				FlowSolver2d solver = new FlowSolver2d();
				message = "init = "
						+ Integer.toString(solver.init(apkFilePath,outputFile.getAbsolutePath(),
								solutionFile.getAbsolutePath(),
								offset,fileSize));
				message += "\nmsg = " + solver.getmsg();
				message += "\nstep = " + Integer.toString(solver.step());
				message += "\nstep = " + Integer.toString(solver.step());
				message += "\nstep = " + Integer.toString(solver.step());

				message += "\n\nlength = " + Long.toString(fileSize);
				message += "\noffset = " + Long.toString(offset);

				showSolverCompletedNotification(message);
				showResults();
			}
		};

		solverThread.start();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.i("LocalService", "Received start id " + startId + ": " + intent);
		// We want this service to continue running until it is explicitly
		// stopped, so return sticky.
		return START_STICKY;
	}

	@Override
	public void onDestroy() {
		// Cancel the persistent notification.
		mNM.cancel(NOTIFICATION);

		// Tell the user we stopped.
		Toast.makeText(this, getText(R.string.solver_stopped), Toast.LENGTH_SHORT)
				.show();
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	// This is the object that receives interactions from clients. See
	// RemoteService for a more complete example.
	private final IBinder mBinder = new LocalBinder();

	/**
	 * Show a notification while this service is running.
	 */
	private void showNotification() {
		// In this sample, we'll use the same text for the ticker and the
		// expanded notification
		CharSequence text = getText(R.string.solver_started);

		// Set the icon, scrolling text and timestamp
		Notification notification = new Notification(R.drawable.stat_sample,
				text, System.currentTimeMillis());

		// The PendingIntent to launch our activity if the user selects this
		// notification
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
				new Intent(this, VirtualTunnelActivity.class), 0);

		// Set the info for the views that show in the notification panel.
		notification.setLatestEventInfo(this, text, text,
				contentIntent);

		// Send the notification.
		mNM.notify(NOTIFICATION, notification);
	}

	/**
	 * Show a notification while this service is running.
	 */
	private void showSolverCompletedNotification(String message) {
		// In this sample, we'll use the same text for the ticker and the
		// expanded notification
		CharSequence text = getText(R.string.solver_completed);

		// Set the icon, scrolling text and timestamp
		Notification notification = new Notification(R.drawable.stat_sample,
				text, System.currentTimeMillis());

		// The PendingIntent to launch our activity if the user selects this
		// notification
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
				new Intent(this, VirtualTunnelActivity.class), 0);

		// Set the info for the views that show in the notification panel.
		notification.setLatestEventInfo(this, text, text,
				contentIntent);

		// Send the notification.
		mNM.notify(NOTIFICATION, notification);
	}

	public void showResults(){
        Intent intent = new Intent();
        intent.setClass(this,ShowResultsActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }
}