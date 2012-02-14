package com.googlegode.tunnelk.views;

import java.util.Observable;
import java.util.Observer;

import android.content.Context;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;

import com.googlecode.tunnelk.model.Tag;

public abstract class TagLayout extends RelativeLayout implements Observer {
	protected Tag tag;

	public TagLayout(Context context, Tag tag){
		super(context);
		
		this.tag = tag;

		this.tag.addObserver(this);
		
		this.initialize();
	}
	
	/**
	 * Convenience method.  Inflates the layout with the specified id.
	 * @param resource the id of the layout
	 */
	protected void inflateLayout(int resource){
		LayoutInflater li = (LayoutInflater) getContext().getSystemService(
				Context.LAYOUT_INFLATER_SERVICE);
		
		li.inflate(resource, this, true);
	}
	
	/**
	 * Inflates views, initializes widgets, and sets up event handlers
	 */
	protected abstract void initialize();

	public abstract void update(Observable observable, Object data);
}
