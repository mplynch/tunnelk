package com.googlecode.tunnelk.arduinosimulator.model;

import java.util.Map;

import com.google.appengine.api.datastore.Entity;

public class Tag {
	private int initialValue;

	private boolean readOnly;

	private String name;

	private TagType type;

	private String units;

	protected int value;
	
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

		this.initialValue = this.value = initialValue;

		this.type = type;

		this.readOnly = readOnly;

		initialize();
	}

	public static Tag fromEntity(Entity entity) {
		boolean readOnly = false;
		int initialValue = 0, value = 0;
		Map<String, Object> map;
		String name = "", units = "", typeString = "";
		TagType type = null;

		map = entity.getProperties();

		if (map.containsKey("name"))
			name = map.get("name").toString();

		if (map.containsKey("units"))
			units = map.get("units").toString();

		if (map.containsKey("initialValue"))
			initialValue = ((Long)map.get("initialValue")).intValue();

		if (map.containsKey("value"))
			value = ((Long)map.get("value")).intValue();
		
		if (map.containsKey("type")) {
			typeString = map.get("type").toString();

			type = TagType.valueOf(typeString);
		}

		if (map.containsKey("readOnly"))
			readOnly = (Boolean) map.get("readOnly");
		
		Tag tag = new Tag(name, units, initialValue, type, readOnly);
		
		tag.setValue(value);
		
		return tag;
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
	 * Returns whether or not this tag is read-only
	 * 
	 * @return true or false
	 */
	public boolean isReadOnly() {
		return readOnly;
	}

	/**
	 * Sets the value of this Tag.
	 * 
	 * @param newValue
	 *            the new value for the tag
	 */
	public void setValue(int newValue) {	
		this.value = newValue;
	}

	public Entity toEntity() {
		Entity entity = new Entity("Tag", this.name);
		entity.setProperty("name", this.name);
		entity.setProperty("units", this.units);
		entity.setProperty("initialValue", this.initialValue);
		entity.setProperty("type", this.type.toString());
		entity.setProperty("readOnly", this.readOnly);
		entity.setProperty("value", this.value);
		return entity;
	}

	@Override
	public String toString() {
		return getName();
	}
}
