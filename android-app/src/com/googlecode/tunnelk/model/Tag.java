package com.googlecode.tunnelk.model;

import java.util.Calendar;
import java.util.Date;
import java.util.Observable;

import com.fasterxml.jackson.annotation.JsonIgnore;

public class Tag extends Observable {
	private int initialValue;

	private boolean changedLocally;

	private boolean readOnly;

	private String name;

	private TagType type;

	private String units;

	protected int value;

	private TagHistorySeries history;

	/**
	 * Creates a new Tag instance.
	 */
	public Tag() {

	}

	/**
	 * Creates a new Tag instance.
	 * 
	 * @param name
	 *            the name of the Tag
	 * @param units
	 *            the unit of measure used by the Tag
	 * @param initialValue
	 *            the initial value of the Tag
	 * @param readOnly
	 *            a flag indicating whether or not the tag is read only
	 */
	public Tag(String name, String units, int initialValue, TagType type,
			boolean readOnly) {
		this.name = name;

		this.units = units;

		this.initialValue = initialValue;

		this.type = type;

		this.readOnly = readOnly;

		initialize();
	}

	/**
	 * Gets the tag history series for this tag
	 * 
	 * @return the series
	 */
	public TagHistorySeries getHistory() {
		return this.history;
	}

	/**
	 * Gets the initial value of this tag
	 * 
	 * @return the value
	 */
	public int getInitialValue() {
		return initialValue;
	}

	/**
	 * Gets the name of this tag
	 * 
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * Gets the type for this Tag.
	 * 
	 * @return the type
	 */
	public TagType getType() {
		return type;
	}

	/**
	 * Gets the unit of measure of this tag
	 * 
	 * @return the pin
	 */
	public String getUnits() {
		return units;
	}

	/**
	 * Gets the value of this tag
	 * 
	 * @return the value
	 */
	public int getValue() {
		return value;
	}

	/**
	 * Initializes the value of this tag to its initial value
	 */
	public void initialize() {
		setValue(initialValue);
	}

	/**
	 * Gets whether or not this tag has been changed locally
	 * 
	 * @return true or false
	 */
	@JsonIgnore
	public boolean isChangedLocally() {
		return changedLocally;
	}

	/**
	 * Returns whether or not this tag is read-only
	 * 
	 * @return true or false
	 */
	public boolean isReadOnly() {
		return readOnly;
	}

	/**
	 * Sets whether or not this tag has been changed locally
	 * @param true or false
	 */
	@JsonIgnore
	public void setChangedLocally(boolean value) {
		changedLocally = value;
	}

	/**
	 * Sets the initial value of this tag.
	 * 
	 * @param initialValue
	 *            the initialValue to set
	 */
	public void setInitialValue(int initialValue) {
		this.initialValue = initialValue;
	}

	/**
	 * Sets the name of this tag.
	 * 
	 * @param name
	 *            the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Sets whether this tag is read-only.
	 * 
	 * @param readOnly
	 *            the readOnly to set
	 */
	public void setReadOnly(boolean readOnly) {
		this.readOnly = readOnly;
	}

	/**
	 * Sets the type of this tag.
	 * 
	 * @param type
	 *            the type to set
	 */
	public void setType(TagType type) {
		this.type = type;
	}

	/**
	 * Sets the units of this tag.
	 * 
	 * @param units
	 *            the units to set
	 */
	public void setUnits(String units) {
		this.units = units;
	}

	/**
	 * Sets the value of this Tag, causing Observers to be notified only if the
	 * specified value differs from the Tag's current value.
	 * 
	 * @param newValue
	 *            the new value for the tag
	 */
	public void setValue(int newValue) {
		Date timestamp = Calendar.getInstance().getTime();

		if (history == null)
			this.history = new TagHistorySeries("TempName");

		history.addEntry(timestamp, newValue);

		// TODO: Make design decision regarding whether or not to send every
		// update
		if (value != newValue) {
			value = newValue;

			setChanged();

			notifyObservers();
		}
	}

	@Override
	public String toString() {
		return getName();
	}
}
