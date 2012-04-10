package com.googlecode.tunnelk.views;

import java.util.Observable;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;

import android.content.Context;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.TextView;
import android.widget.ToggleButton;

public class TagOnOffSwitch extends TagLayout implements
		OnCheckedChangeListener {

	public TagOnOffSwitch(Context context, Tag tag) {
		super(context, tag);
	}

	@Override
	protected void initialize() {
		this.inflateLayout(R.layout.tag_layout_on_off_switch);

		TextView nameLabel = (TextView) findViewById(R.id.tagLabel);

		if (nameLabel != null)
			nameLabel.setText(tag.getName());

		ToggleButton toggleButton = (ToggleButton) findViewById(R.id.tagToggleButton);

		if (toggleButton != null) {
			toggleButton.setChecked(tag.getValue() == 1);
			toggleButton.setOnCheckedChangeListener(this);
		}

		TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

		if (valueLabel != null)
			valueLabel.setText(tag.getValue() + " " + tag.getUnits());
	}

	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		tag.setValue(isChecked ? 1 : 0);
	}
	
	@Override
	public void update(Observable observable, Object data) {
		post(new Runnable() {
			public void run() {
				ToggleButton toggleButton = (ToggleButton) findViewById(R.id.tagToggleButton);

				if (toggleButton != null) {
					toggleButton.setChecked(tag.getValue() == 1);
				}
			}
		});
	}
}
