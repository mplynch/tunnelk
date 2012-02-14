package com.googlegode.tunnelk.views;

import java.util.Observable;

import android.content.Context;
import android.widget.SeekBar;
import android.widget.TextView;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;

public class TagSeekBar extends TagLayout implements
		SeekBar.OnSeekBarChangeListener {

	public TagSeekBar(Context context, Tag tag) {
		super(context, tag);
	}

	@Override
	protected void initialize() {
		this.inflateLayout(R.layout.tag_layout_seek_bar);

		TextView nameLabel = (TextView) findViewById(R.id.tagLabel);

		if (nameLabel != null)
			nameLabel.setText(tag.getName());

		SeekBar seekBar = (SeekBar) findViewById(R.id.tagSeekBar);

		if (seekBar != null) {
			seekBar.setProgress(tag.getValue());
			seekBar.setOnSeekBarChangeListener(this);
		}

		TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

		if (valueLabel != null)
			valueLabel.setText(tag.getValue() + " " + tag.getUnits());
	}

	public void onProgressChanged(SeekBar seekBar, int progress,
			boolean fromUser) {
		tag.setValue(progress);

		TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

		if (valueLabel == null)
			return;

		valueLabel.setText(tag.getValue() + " " + tag.getUnits());
	}

	public void onStartTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub

	}

	public void onStopTrackingTouch(SeekBar seekBar) {
		// TODO Auto-generated method stub

	}

	@Override
	public void update(Observable observable, Object data) {
		post(new Runnable() {
			public void run() {
				TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

				if (valueLabel != null)
					valueLabel.setText(tag.getValue() + " " + tag.getUnits());

				SeekBar seekBar = (SeekBar) findViewById(R.id.tagSeekBar);

				if (seekBar != null)
					seekBar.setProgress(tag.getValue());
			}
		});
	}
}
