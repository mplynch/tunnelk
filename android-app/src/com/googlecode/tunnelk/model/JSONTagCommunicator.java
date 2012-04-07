package com.googlecode.tunnelk.model;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URIUtils;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * A TagCommunicator implemented using a JSON-based web service.
 */
public class JSONTagCommunicator implements TagCommunicator {
	private final String method = "http";
	private final String host = "tunnelksim.appspot.com";
	private final String path = "/command/ExchangeTags";

	private boolean updatingTags;

	private LinkedList<Tag> updateQueue;

	public JSONTagCommunicator() {
		updatingTags = false;

		updateQueue = new LinkedList<Tag>();
	}

	public synchronized void exchangeTags() {
		try {
			ObjectMapper mapper = new ObjectMapper();

			TagManager manager = TagManager.getInstance();

			HttpClient httpclient = new DefaultHttpClient();

			List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();

			if (!updateQueue.isEmpty()) {
				for (Tag tag : updateQueue) {
					nameValuePairs.add(new BasicNameValuePair(tag.getName(),
							Integer.toString(tag.getValue())));
				}
			}

			URI uri = URIUtils.createURI(method, host, -1, path,
					URLEncodedUtils.format(nameValuePairs, "UTF-8"), null);

			HttpPost request = new HttpPost(uri);

			updatingTags = true;

			try {
				HttpResponse response = httpclient.execute(request);

				HttpEntity entity = response.getEntity();

				String content = EntityUtils.toString(entity);

				List<Tag> tags = mapper.readValue(content,
						new TypeReference<List<Tag>>() {
						});

				for (Tag responseTag : tags) {
					Tag tag = manager.getTag(responseTag.getName());

					if (tag == null) {
						tag = responseTag;
						tag.addObserver(this);
						manager.addTag(tag);
					}

					tag.setValue(responseTag.getValue());
				}
			} catch (ClientProtocolException e) {
				System.out.println(e.toString());
			} catch (IOException e) {
				System.out.println(e.toString());
			}
		} catch (Exception e) {
			System.out.println(e.toString());
		}

		updatingTags = false;
	}

	public synchronized void update(Observable observable, Object data) {
		// Check for a circular change notification
		if (updatingTags)
			return;

		// Check for bad input
		if (observable.getClass() != Tag.class)
			return;

		// Ensure a tag is only queued once
		if (updateQueue.contains(observable))
			return;

		updateQueue.add((Tag) observable);
	}
}
