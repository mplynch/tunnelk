package com.googlecode.tunnelk;

import java.util.Collection;
import java.util.Timer;
import java.util.TimerTask;

import com.googlecode.tunnelk.model.*;
import com.googlegode.tunnelk.views.TagLayout;
import com.googlegode.tunnelk.views.TagLayoutFactory;

import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;

public class PhysicalTunnelHMIActivity extends OptionsMenuActivity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);
		
		TagCommunicator comm = new HardCodedTagCommunicator();
		
		Collection<Tag> tags = comm.getTagData();
		
		TagManager.getInstance().addTags(tags);
		
		for (Tag tag : tags){
			TagLayout tagLayout = TagLayoutFactory.create(this, tag);
			
			if (tagLayout != null)
				layout.addView(tagLayout);
		}
		
		setContentView(layout);
		
		Timer timer = new Timer();
		timer.scheduleAtFixedRate(new UpdateTagsTask(comm), 0, 1000);
	}
	
	private static final class UpdateTagsTask extends TimerTask {
		private TagCommunicator comm;
		
		public UpdateTagsTask(TagCommunicator comm){
			super();
			
			this.comm = comm;
		}
		
		@Override
		public void run() {
			comm.getTagValues();
		}
	}
}
