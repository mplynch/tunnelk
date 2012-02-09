package com.googlecode.tunnelk.model;

import java.util.Observable;

public class Tag extends Observable {
	private String name;

	private String units;

	private int initialValue;
	
	private TagType type;

	private int value;

	/**
	 * Creates a new Tag instance.
	 * @param name the name of the Tag
	 * @param units the unit of measure used by the Tag
	 * @param initialValue the initial value of the Tag
	 */
	public Tag(String name, String units, int initialValue, TagType type) {
		this.name = name;

		this.units = units;

		this.initialValue = initialValue;
		
		this.type = type;
		
		initialize();
	}

	/**
	 * Gets the initial value of this tag
	 * @return the value
	 */
	public int getInitialValue() {
		return initialValue;
	}
	
	/**
	 * Gets the name of this tag
	 * @return the name
	 */
	public String getName() {
		return name;
	}
	
	/**
	 * Gets the type for this Tag.
	 * @return the type
	 */
	public TagType getType(){
		return type;
	}

	/**
	 * Gets the unit of measure of this tag
	 * @return the pin
	 */
	public String getUnits() {
		return units;
	}

	/**
	 * Gets the value of this tag
	 * @return the value
	 */
	public int getValue() {
		return value;
	}

	/**
	 * Initializes the value of this tag to its initial value
	 */
	public void initialize()
	{
		setValue(initialValue);
	}
	
	/**
	 * Sets the value of this Tag, causing Observers to be notified only if
	 * the specified value differs from the Tag's current value.
	 * @param newValue the new value for the tag
	 */
	public void setValue(int newValue) {
		if (value != newValue) {
			value = newValue;

			setChanged();

			notifyObservers();
		}
	}
}
