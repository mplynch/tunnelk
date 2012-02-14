package com.googlecode.tunnelk.model;

import java.util.*;

/**
 * A TagCommunicator implemented using hard-coded tag data.
 */
public class HardCodedTagCommunicator implements TagCommunicator {
	private ArrayList<Tag> tags;
	
	public void getTagValues(){
		Random r = new Random(Calendar.getInstance().getTimeInMillis());
		
		for (Tag tag : tags){
			if (tag.isReadOnly())
				tag.setValue(r.nextInt(100));
		}
	}
		
	public Collection<Tag> getTagData() {
		Tag tag;
		
		tags = new ArrayList<Tag>();
		
		tag = new Tag("Inlet Temperature 1", "F", 72, TagType.Temperature, true);
		tags.add(tag);
		
		tag = new Tag("Inlet Temperature 2", "F", 70, TagType.Temperature, true);
		tags.add(tag);
		
		tag = new Tag("Fan Speed", "%", 50, TagType.FanSpeedSetPoint, false);
		tags.add(tag);
		
		tag = new Tag("Fan On/Off", "", 0, TagType.Relay, false);
		tags.add(tag);
		
		return tags;
	}

	public void update(Tag tag) {
		// Do nothing
	}

	public void update(Collection<Tag> tag) {
		// Do nothing
	}
}