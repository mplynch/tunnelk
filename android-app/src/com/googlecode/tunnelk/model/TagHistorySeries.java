package com.googlecode.tunnelk.model;

import java.util.ArrayList;
import java.util.Date;

import com.androidplot.series.XYSeries;

public class TagHistorySeries implements XYSeries {
	/**
	 * The maximum number of entries logged before the oldest entry is deleted
	 * to make room for each additional new entry.
	 */
	public static final int MAX_ENTRIES = 300;

	private ArrayList<Date> timestamps;

	private ArrayList<Integer> values;

	private String title;

	public TagHistorySeries(String title) {
		timestamps = new ArrayList<Date>();

		values = new ArrayList<Integer>();

		this.title = title;
	}

	public synchronized void addEntry(Date timestamp, int value) {
		if (timestamps.size() > MAX_ENTRIES) {
			timestamps.remove(0);
			values.remove(0);
		}

		timestamps.add(timestamp);

		values.add(value);
	}

	public String getTitle() {
		return title;
	}

	public synchronized int size() {
		return timestamps.size();
	}

	public synchronized Number getX(int index) {
		return timestamps.get(index).getTime();
	}

	public synchronized Number getY(int index) {
		return values.get(index);
	}
}
