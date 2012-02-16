package com.googlecode.tunnelk.model;

import java.util.*;

/**
 * A TagCommunicator implemented using hard-coded tag data.
 */
public class HardCodedTagCommunicator implements TagCommunicator {
	private ArrayList<Tag> tags;
	
	private int i;
	
	public void getTagValues(){
		if (i > 100)
			i = 0;
		
		for (Tag tag : tags){
			if (tag.isReadOnly())
				tag.setValue(i);
		}
		
		i++;
	}
		
	public Collection<Tag> getTagData() {
		Tag tag;
		
		i = 0;
		
		tags = new ArrayList<Tag>();
		
		tag = new Tag("Inlet Temperature 1", "F", 72, TagType.Temperature, true);
		tags.add(tag);
		
		tag = new Tag("Inlet Temperature 2", "F", 70, TagType.Temperature, true);
		tags.add(tag);
		
		tag = new Tag("Fan Speed", "%", 50, TagType.FanSpeedSetPoint, false);
		tags.add(tag);
		
		tag = new Tag("Fan On/Off", "", 0, TagType.Relay, false);
		tags.add(tag);
		
		tags.add(new JTag());
		
		return tags;
	}

	public void update(Tag tag) {
		// Do nothing
	}

	public void update(Collection<Tag> tag) {
		// Do nothing
	}
}
