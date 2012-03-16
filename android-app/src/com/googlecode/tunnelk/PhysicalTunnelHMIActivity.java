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
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;

public class PhysicalTunnelHMIActivity extends TunnelKActivity {
	private final int chooseTagsRequestCode = 1;
	private final int showPlotRequestCode = 2;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.physical_tunnel_hmi);

		// LinearLayout layout = new LinearLayout(this);
		LinearLayout layout = (LinearLayout) findViewById(R.id.linearLayoutHMIWidgets);
		layout.setOrientation(LinearLayout.VERTICAL);

		TagCommunicator comm = new HardCodedTagCommunicator();

		Collection<Tag> tags = comm.getTagData();

		TagManager.getInstance().addTags(tags);

		for (Tag tag : tags) {
			TagLayout tagLayout = TagLayoutFactory.create(this, tag);

			if (tagLayout != null)
				layout.addView(tagLayout);
		}

		// setContentView(layout);

		Timer timer = new Timer();
		timer.scheduleAtFixedRate(new UpdateTagsTask(comm), 0, 5000);
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
			//startActivity(intent);
			startActivityForResult(intent, showPlotRequestCode);
		}
		
		else if (requestCode == showPlotRequestCode){
			
		}
	}

	public void onTrendGraphClick(View v) {
		if (v.getId() != R.id.buttonTrendGraph)
			return;

		Intent intent = new Intent(this, ChooseTagsActivity.class);
		startActivityForResult(intent, chooseTagsRequestCode);
	}

	private static final class UpdateTagsTask extends TimerTask {
		private TagCommunicator comm;

		public UpdateTagsTask(TagCommunicator comm) {
			super();

			this.comm = comm;
		}

		@Override
		public void run() {
			comm.getTagValues();
		}
	}
}
