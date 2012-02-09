package com.googlegode.tunnelk.views;

import java.util.Observable;
import java.util.Observer;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;

import android.content.Context;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;

public class TagLayout extends RelativeLayout implements Observer {

	private Tag tag;
	
	public TagLayout(Context context) {
		super(context);
	}

	public void setTag(Tag tag) {
		if (this.tag != null)
			this.tag.deleteObserver(this);

		this.tag = tag;
		
		LayoutInflater li = (LayoutInflater) getContext().getSystemService(
				Context.LAYOUT_INFLATER_SERVICE); 

		switch (tag.getType()) {
		/* TODO: Fill in each case with code to make use of the corresponding
		 * layout. */
		case TemperatureSetPoint:
			break;
		case Temperature:
			break;
		case FanSpeedSetPoint:
			li.inflate(R.layout.tag_layout_seek_bar, this, true);
			break;
		case FanSpeed:
			break;
		case Humidity:
			break;
		default:
			break;
		}

		this.tag.addObserver(this);
	}

	public void update(Observable observable, Object data) {
		// TODO: Update the displayed value of the Tag.
	}
}
