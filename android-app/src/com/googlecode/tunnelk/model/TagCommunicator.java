package com.googlecode.tunnelk.model;

import java.util.Collection;

public interface TagCommunicator {
	/**
	 * Gets the values of all tags from the controller.  You must call 
	 * getTagData before attempting to retrieve tag values.
	 */
	public void getTagValues();
	
	/**
	 * Returns a collection of all tags used by the controller.
	 */
	public Collection<Tag> getTagData();
	
	/**
	 * Updates the value of the specified tag on the controller.
	 * @param tag the tag to update
	 */
	public void update(Tag tag);
	
	/**
	 * Updates the values of the specified tags on the controller.
	 * @param tag a collection of tags to update
	 */
	public void update(Collection<Tag> tag);
}
