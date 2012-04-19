package com.googlecode.tunnelk;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.googlecode.tunnelk.model.*;
import com.googlecode.tunnelk.views.TagLayoutAdapter;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ListView;

public class PhysicalTunnelHMIActivity extends TunnelKActivity {
	private final int chooseTagsRequestCode = 1;

	private final int showPlotRequestCode = 2;

	private boolean viewGenerated = false;

	private Timer timer;

	public void generateViews() {
		final PhysicalTunnelHMIActivity activity = this;

		runOnUiThread(new Runnable() {
			public void run() {
				List<Tag> allTags = new ArrayList<Tag>(TagManager.getInstance().getAllTags());
				ArrayList<Tag> tags = new ArrayList<Tag>();
				
				for (Tag tag : allTags) {
					if ((tag.getType() == TagType.LED) || (tag.getType() == TagType.Relay))
						continue;
					
					tags.add(tag);
				}

				ListView listViewTagsList = (ListView) findViewById(R.id.listViewTagsList);
				listViewTagsList.setAdapter(new TagLayoutAdapter(activity, R.id.tagLabel, tags));

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

		// LinearLayout layout = (LinearLayout)
		// findViewById(R.id.linearLayoutHMIWidgets);
		LinearLayout layout = (LinearLayout) findViewById(R.id.linearLayoutHMI);
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

		SharedPreferences p = this.getSharedPreferences("TunnelkPrefs", 0);

		String url = p.getString("@string/controller_address", getResources()
				.getString(R.string.default_controller_address));

		TagCommunicator comm = new JSONTagCommunicator(url);

		timer = new Timer();
		timer.scheduleAtFixedRate(new UpdateTagsTask(this, comm), 0, 1000);
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
