package com.googlegode.tunnelk.views;

import java.util.Observable;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;

import android.content.Context;
import android.os.Handler;
import android.widget.TextView;

public class TagReadOnlyText extends TagLayout {

	public TagReadOnlyText(Context context, Tag tag) {
		super(context, tag);
	}

	@Override
	protected void initialize() {
		this.inflateLayout(R.layout.tag_layout_read_only_text);

		TextView nameLabel = (TextView) findViewById(R.id.tagLabel);

		if (nameLabel != null)
			nameLabel.setText(tag.getName());

		TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

		if (valueLabel != null)
			valueLabel.setText(tag.getValue() + " " + tag.getUnits());
	}

	@Override
	public void update(Observable observable, Object data) {
		post(new Runnable() {
			public void run() {
				TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

				if (valueLabel != null)
					valueLabel.setText(tag.getValue() + " " + tag.getUnits());
			}
		});
	}
}
