package com.googlecode.tunnelk.arduinosimulator.commands;

import java.io.IOException;
import java.io.StringWriter;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Random;
import java.util.logging.Logger;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;

import org.bibeault.frontman.*;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import com.google.appengine.api.datastore.*;
import com.googlecode.tunnelk.arduinosimulator.model.Tag;

import static com.google.appengine.api.datastore.FetchOptions.Builder.*;

public class ExchangeTagsCommand implements Command {
	private static final Logger log = Logger.getLogger(ExchangeTagsCommand.class.getName());
	
	@Override
	public void execute(CommandContext context) {
		HashMap<String, Tag> tagsMap = new HashMap<String, Tag>();
		HttpServletRequest request = context.getRequest();
		ObjectMapper mapper = new ObjectMapper();
		StringWriter writer = new StringWriter();

		DatastoreService datastore = DatastoreServiceFactory
				.getDatastoreService();

		Query query = new Query("Tag");

		PreparedQuery preparedQuery = datastore.prepare(query);

		// Results are limited to 25 entities
		if (preparedQuery.countEntities(withLimit(25)) == 0) {
			try {
				context.forwardToCommand("CreateTags");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (ServletException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			return;
		}

		Random r = new Random(Calendar.getInstance().getTimeInMillis());
		
		for (Entity entity : preparedQuery.asIterable()) {
			Tag tag = Tag.fromEntity(entity);

			if (tag.isReadOnly()) {
				int jitter = 0;
				jitter = r.nextInt(10);
				jitter -= 5;
				tag.setValue(tag.getValue() + jitter);
			}

			tagsMap.put(tag.getName(), tag);
		}

		@SuppressWarnings("unchecked")
		Enumeration<String> parameters = request.getParameterNames();
		
		while (parameters.hasMoreElements()) {
			String name = parameters.nextElement();

			Tag tag = tagsMap.get(name);

			if (tag == null)
				continue;

			if (tag.isReadOnly())
				continue;

			int value = Integer.parseInt(request.getParameter(name));

			tag.setValue(value);

			datastore.put(tag.toEntity());
			
			log.info("Received tag: " + tag.getName() + " = " + tag.getValue());
		}

		try {
			mapper.enable(SerializationFeature.INDENT_OUTPUT);
			mapper.writeValue(writer, tagsMap.values());
			context.setScopedVariable("tags", writer.toString());

			context.forwardToView("TagsPage");
		} catch (ServletException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
