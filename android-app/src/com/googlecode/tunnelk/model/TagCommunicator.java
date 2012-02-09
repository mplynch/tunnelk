package com.googlecode.tunnelk.model;

public class TagCommunicator {
	public void getTagValues(){
		
	}
	
	public void requestTags(){
		TagManager manager = TagManager.getInstance();
		Tag tag;
		
		tag = new Tag("inletTemp1", "A0", 72, TagType.Temperature);
		manager.addTag(tag);
		
		tag = new Tag("inletTemp2", "A1", 70, TagType.Temperature);
		manager.addTag(tag);
		
		tag = new Tag("fanSpeedSetPoint", "A2", 0, TagType.FanSpeedSetPoint);
	}
	
	public void updateTagValue(Tag tag){
		
	}
}
