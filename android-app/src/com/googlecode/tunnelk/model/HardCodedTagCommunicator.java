package com.googlecode.tunnelk.model;

import java.util.ArrayList;
//import java.util.Calendar;
import java.util.Collection;
//import java.util.Random;

/**
 * A TagCommunicator implemented using hard-coded tag data.
 */
public class HardCodedTagCommunicator implements TagCommunicator {
	private ArrayList<Tag> tags;
	
	public void getTagValues(){
//		Random r = new Random(Calendar.getInstance().getTimeInMillis());
		
		for (Tag tag : tags){
			if (tag.isReadOnly()){
				tag.setValue(0);
				//tag.setValue(tag.getValue() + r.nextInt(6) - 3);		
			}
		}
	}
		
	public Collection<Tag> getTagData() {
		Tag tag;
		
//		int i = 0;
		
		tags = new ArrayList<Tag>();
		
//		tag = new Tag("Inlet Temperature 1", "F", 72, TagType.Temperature, true);
//		tags.add(tag);
//		
//		tag = new Tag("Inlet Temperature 2", "F", 70, TagType.Temperature, true);
//		tags.add(tag);
//		
//		tag = new Tag("Fan Speed", "%", 50, TagType.FanSpeedSetPoint, false);
//		tags.add(tag);
//		
//		tag = new Tag("Fan On/Off", "", 0, TagType.Relay, false);
//		tags.add(tag);
		
//		tags.add(new JTag());
		tag = new MTag();
		tags.add(tag);
		
		tag = new JTag();
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
