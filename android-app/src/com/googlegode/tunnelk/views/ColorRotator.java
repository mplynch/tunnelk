package com.googlegode.tunnelk.views;

import android.graphics.Color;

public class ColorRotator {
	public static final int[] COLORS = new int[] { Color.BLACK, Color.BLUE,
			Color.CYAN, Color.DKGRAY, Color.GRAY, Color.GREEN, Color.LTGRAY,
			Color.MAGENTA, Color.RED, Color.WHITE, Color.YELLOW };

	private static ColorRotator instance;

	protected int colorIndex = 0;

	/**
	 * Returns a color from the color palette.
	 * 
	 * @return a color
	 */
	public int nextColor() {
		return COLORS[getNextColorIndex()];
	}

	private int getNextColorIndex() {
		if (colorIndex >= COLORS.length)
			colorIndex = 0;

		return colorIndex++;
	}

	/**
	 * Returns the singleton instance of the color rotator.
	 * 
	 * @return the instance
	 */
	public static ColorRotator getInstance() {
		if (instance == null)
			instance = new ColorRotator();

		return instance;
	}

	/**
	 * Returns a color from the color palette.
	 * 
	 * @return a color
	 */
	public static int getNextColor() {
		return ColorRotator.getInstance().nextColor();
	}
}
