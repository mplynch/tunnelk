package com.googlecode.tunnelk;

import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.googlecode.tunnelk.model.JSONTagCommunicator;
import com.googlecode.tunnelk.model.TagCommunicator;
import com.googlecode.tunnelk.views.*;

public class TimeHistoryPlotActivity extends TunnelKActivity implements
		View.OnClickListener {
	private Timer timer;

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

	public void onClick(View v) {
		setResult(RESULT_OK);
		finish();
	}

	@Override
	protected void onPause() {
		super.onPause();

		timer.cancel();
	}

	@Override
	protected void onResume() {
		super.onResume();

		TagCommunicator comm = new JSONTagCommunicator();

		timer = new Timer();
		timer.scheduleAtFixedRate(new UpdateTagsTask(comm), 0, 5000);
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
