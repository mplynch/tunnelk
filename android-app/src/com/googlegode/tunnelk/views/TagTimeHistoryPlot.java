package com.googlegode.tunnelk.views;

import java.text.DecimalFormat;
import java.text.FieldPosition;
import java.text.Format;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Collection;
import java.util.Date;
import java.util.Observable;
import java.util.Observer;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RelativeLayout;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagManager;

import com.androidplot.xy.*;

public class TagTimeHistoryPlot extends RelativeLayout implements Observer,
	View.OnClickListener {
	private View.OnClickListener clickListener;
	
	private XYPlot plot;

	public TagTimeHistoryPlot(Context context, Collection<String> tagNames) {
		super(context);

		LayoutInflater li = (LayoutInflater) getContext().getSystemService(
				Context.LAYOUT_INFLATER_SERVICE);

		li.inflate(R.layout.time_history_plot, this, true);

		plot = (XYPlot) findViewById(R.id.tagTimeHistoryPlot);

		// By default, AndroidPlot displays developer guides to aid in laying
		// out your plot.
		// To get rid of them call disableAllMarkup():
		plot.disableAllMarkup();

		plot.setDomainLabel("Time (seconds)");
		plot.setDomainValueFormat(new SecondFormat());

		plot.setRangeValueFormat(new DecimalFormat("0"));
		plot.setRangeBoundaries(0, 100, BoundaryMode.AUTO);
		plot.setOnClickListener(this);
		long time = Calendar.getInstance().getTimeInMillis();
		plot.setDomainBoundaries(time - 60000, time, BoundaryMode.FIXED);
		
		TagManager manager = TagManager.getInstance();

		for (String name : tagNames) {
			Tag tag = manager.getTag(name);

			if (tag == null)
				continue;

			int color = ColorRotator.getNextColor();

			LineAndPointFormatter historyFormat = new LineAndPointFormatter(
					color, null, null);
			// StepFormatter historyFormat = new StepFormatter(color, null);

			plot.setRangeLabel(tag.getUnits());

			// plot.addSeries(history, historyFormat);
			plot.addSeries(tag.getHistory(), historyFormat);

			tag.addObserver(this);
		}
	}

	public void onClick(View v) {
		if (v.getId() == R.id.tagTimeHistoryPlot){
			if (this.clickListener != null)
				this.clickListener.onClick(v);
		}
	}
	
	/**
	 * Sets the method to call when the plot is clicked.
	 * 
	 * @param clickListener the View.OnClickListener method to call
	 */
	public void setClickListener(View.OnClickListener clickListener){
		this.clickListener = clickListener;
	}

	public void update(Observable observable, Object data) {
		this.post(new Runnable() {
			public void run() {
				long time = Calendar.getInstance().getTimeInMillis();

				plot.setDomainBoundaries(time - 60000, time, BoundaryMode.FIXED);

				plot.redraw();
			}
		});
	}

	private class SecondFormat extends Format {
		private static final long serialVersionUID = -5912310861073422560L;

		private SimpleDateFormat dateFormat = new SimpleDateFormat("s");

		@Override
		public StringBuffer format(Object obj, StringBuffer toAppendTo,
				FieldPosition pos) {
			long timestamp = ((Number) obj).longValue();
			Date date = new Date(timestamp);
			return dateFormat.format(date, toAppendTo, pos);
		}

		@Override
		public Object parseObject(String source, ParsePosition pos) {
			return null;

		}
	}

}
