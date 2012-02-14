package com.googlegode.tunnelk.views;

import android.content.Context;

import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagType;

public class TagLayoutFactory {
	/**
	 * Creates a tag layout in the specified context for the specified tag
	 * @param context the context
	 * @param type the tag
	 * @return the new layout
	 */
	public static TagLayout create(Context context, Tag tag){
		switch (tag.getType()) {
		case Temperature:
			return (new TagReadOnlyText(context, tag));
		case FanSpeedSetPoint:
			return (new TagSeekBar(context, tag));
		case FanSpeed:
			return (new TagReadOnlyText(context, tag));
		case Humidity:
			return (new TagReadOnlyText(context, tag));
		case Relay:
			return (new TagOnOffSwitch(context, tag));
		default:
		}
		
		return (new TagReadOnlyText(context, tag));
	}
}
