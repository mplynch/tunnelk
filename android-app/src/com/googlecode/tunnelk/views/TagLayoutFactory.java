package com.googlecode.tunnelk.views;

import java.util.Collection;

import android.content.Context;

import com.googlecode.tunnelk.model.Tag;

public class TagLayoutFactory {
	/**
	 * Creates a tag layout in the specified context for the specified tag
	 * 
	 * @param context
	 *            the context on which to create the tag layout
	 * @param tag
	 *            the tag
	 * @return the new layout
	 */
	public static TagLayout create(Context context, Tag tag) {
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
		case LED:
			return (new TagOnOffSwitch(context, tag));
		default:
		}

		return (new TagReadOnlyText(context, tag));
	}

	/**
	 * Creates an X-Y plot displaying the value of the specified tags over 
	 * time.  The tags are observed for changes, and the plot is automatically
	 * updated with any new data.
	 * 
	 * @param context
	 *            the context on which to create the plot
	 * @param tagNames
	 *            a collection containing the names of the tags to display
	 * @return the new plot layout
	 */
	public static TagTimeHistoryPlot createPlot(Context context,
			Collection<String> tagNames) {
		return (new TagTimeHistoryPlot(context, tagNames));
	}
}
