package com.googlecode.tunnelk;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;

import com.googlecode.tunnelk.model.JSONTagCommunicator;
import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagCommunicator;
import com.googlecode.tunnelk.model.TagManager;
import com.googlecode.tunnelk.views.*;

public class TimeHistoryPlotActivity extends TunnelKActivity implements
		View.OnClickListener {
	private Timer timer;
	
	private TagCommunicator comm;
	
	private boolean skipInit = false;

	@Override
	protected boolean getDefaultOptionsMenuEnabled() {
		return false;
	}

	private TagTimeHistoryPlot plot;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		plot = TagLayoutFactory.createPlot(this);

		plot.setClickListener(this);

		setContentView(plot);
	}

	public void onClick(View v) {
		skipInit = true;
		
		setResult(RESULT_OK);
		finish();
	}

	@Override
	protected void onPause() {
		super.onPause();

		timer.cancel();
		
		if (!skipInit) {
			List<Tag> tags = new ArrayList<Tag>(TagManager.getInstance()
					.getAllTags());

			for (Tag tag : tags) {
				tag.initialize();
			}

			Thread thread = new Thread(new Runnable() {
				public void run() {
					comm.exchangeTags();
				}
			});

			thread.start();
			
			while (thread.isAlive())
				try {
					Thread.sleep(1);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		
		skipInit = false;
		
		SharedPreferences p = this.getSharedPreferences("TunnelkPrefs", 0);

		String url = p.getString("@string/controller_address",
				getResources().getString(R.string.default_controller_address));
		
		comm = new JSONTagCommunicator(url);

		timer = new Timer();
		timer.scheduleAtFixedRate(new UpdateTagsTask(comm), 0, 1000);
	}

	private static final class UpdateTagsTask extends TimerTask {

		private TagCommunicator comm;

		public UpdateTagsTask(TagCommunicator comm) {
			super();

			this.comm = comm;
		}

		@Override
		public void run() {
			comm.exchangeTags();
		}
	}
}
