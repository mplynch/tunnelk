package com.googlecode.tunnelk.model;

import java.util.Collection;
import java.util.HashMap;

/**
 * Manages the Tags used in the app. This class makes use of the Singleton
 * design pattern. Call getInstance() to get the singleton instance of the
 * class.
 */
public class TagManager {
	private static TagManager instance;

	private HashMap<String, Tag> tags;

	/**
	 * Creates a TagManager with zero tags.
	 */
	private TagManager() {
		tags = new HashMap<String, Tag>();
	}

	/**
	 * Adds the specified tag to this TagManager, enabling observers to receive
	 * notification when the tag is changed.
	 * 
	 * @param tag a tag to be added
	 */
	public void addTag(Tag tag) {
		tags.put(tag.getName(), tag);
	}

	/**
	 * Gets the tag with the specified name
	 * 
	 * @param name the name of the tag to retrieve
	 * @return the tag with the specified name
	 */
	public Tag getTag(String name) {
		return tags.get(name);
	}
	
	/**
	 * Returns a collection of all Tags stored in this TagManager.
	 * 
	 * @return 
	 */
	public Collection<Tag> getAllTags(){
		return tags.values();
	}

	/**
	 * Returns the singleton instance of the TagManager.
	 * 
	 * @return the instance
	 */
	public static TagManager getInstance() {
		if (TagManager.instance == null)
			TagManager.instance = new TagManager();

		return TagManager.instance;
	}

	/**
	 * Initializes all tags added to this TagManager.
	 */
	public void initializeTags() {
		Collection<Tag> tagCollection = tags.values();

		for (Tag tag : tagCollection) {
			tag.initialize();
		}
	}

	/**
	 * Removes the tag with the specified name from this TagManager.
	 * 
	 * @param name the name of the tag to remove
	 * @return the tag that was removed
	 */
	public Tag removeTag(String name) {
		return tags.remove(name);
	}
}
