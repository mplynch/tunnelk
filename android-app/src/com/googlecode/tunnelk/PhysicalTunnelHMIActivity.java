package com.googlecode.tunnelk;

import java.util.Collection;
import java.util.Timer;
import java.util.TimerTask;

import com.googlecode.tunnelk.model.*;
import com.googlegode.tunnelk.views.TagLayout;
import com.googlegode.tunnelk.views.TagLayoutFactory;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;

public class PhysicalTunnelHMIActivity extends TunnelKActivity {
	private final int chooseTagsRequestCode = 1;

	private final int showPlotRequestCode = 2;

	private boolean viewGenerated = false;
	
	private Timer timer;

	public void generateViews() {
		final PhysicalTunnelHMIActivity activity = this;

		runOnUiThread(new Runnable() {
			public void run() {
				Collection<Tag> tags = TagManager.getInstance().getAllTags();

				LinearLayout layout = (LinearLayout) findViewById(R.id.linearLayoutHMIWidgets);

				TagManager.getInstance().addTags(tags);

				for (Tag tag : tags) {
					TagLayout tagLayout = TagLayoutFactory
							.create(activity, tag);

					if (tagLayout != null)
						layout.addView(tagLayout);
				}
				
				activity.viewGenerated = true;
			}
		});
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == chooseTagsRequestCode) {
			String[] tagNames = data.getStringArrayExtra(getResources()
					.getString(R.string.trending_tags));

			if (tagNames == null)
				return;

			Intent intent = new Intent(this, TimeHistoryPlotActivity.class);
			intent.putExtra(getResources().getString(R.string.trending_tags),
					tagNames);
			// startActivity(intent);
			startActivityForResult(intent, showPlotRequestCode);
		}

		else if (requestCode == showPlotRequestCode) {

		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.physical_tunnel_hmi);

		LinearLayout layout = (LinearLayout) findViewById(R.id.linearLayoutHMIWidgets);
		layout.setOrientation(LinearLayout.VERTICAL);
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
		timer.scheduleAtFixedRate(new UpdateTagsTask(this, comm), 0, 5000);
	}

	public void onTrendGraphClick(View v) {
		if (v.getId() != R.id.buttonTrendGraph)
			return;

		Intent intent = new Intent(this, ChooseTagsActivity.class);
		startActivityForResult(intent, chooseTagsRequestCode);
	}
	
	private static final class UpdateTagsTask extends TimerTask {
		private PhysicalTunnelHMIActivity activity;
		
		private TagCommunicator comm;

		public UpdateTagsTask(PhysicalTunnelHMIActivity activity,
				TagCommunicator comm) {
			super();

			this.comm = comm;
			
			this.activity = activity;
		}

		@Override
		public void run() {
			comm.exchangeTags();
			
			if (!activity.viewGenerated) {
				activity.generateViews();
			}
		}
	}
}
