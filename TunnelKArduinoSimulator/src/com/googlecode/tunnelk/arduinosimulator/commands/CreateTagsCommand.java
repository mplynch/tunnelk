package com.googlecode.tunnelk.arduinosimulator.commands;

import java.io.IOException;

import javax.servlet.ServletException;

import org.bibeault.frontman.*;

import com.google.appengine.api.datastore.*;
import com.googlecode.tunnelk.arduinosimulator.model.Tag;
import com.googlecode.tunnelk.arduinosimulator.model.TagType;

public class CreateTagsCommand implements Command {
	@Override
	public void execute(CommandContext context) {
		Tag tag;
		
		DatastoreService datastore = DatastoreServiceFactory
				.getDatastoreService();

		tag = new Tag("InletTemp1", "degF", 72, TagType.Temperature, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("InletTemp2", "degF", 72, TagType.Temperature, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("ExhaustTemp1", "degF", 50, TagType.Temperature, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("ExhaustTemp2", "degF", 50, TagType.Temperature, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("RelativeHumidity", "%", 50, TagType.Humidity, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("FanSpeed", "RPM", 0, TagType.FanSpeed, true);
		datastore.put(tag.toEntity());
		
		tag = new Tag("FanSpeedSetPoint", "%", 0, TagType.FanSpeedSetPoint, false);
		datastore.put(tag.toEntity());

		tag = new Tag("AirOn", "", 0, TagType.Relay, false);
		datastore.put(tag.toEntity());
		
		try {
			context.forwardToCommand("ExchangeTags");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ServletException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
