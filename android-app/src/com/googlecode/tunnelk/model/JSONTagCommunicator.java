package com.googlecode.tunnelk.model;

import java.io.IOException;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * A TagCommunicator implemented using a JSON-based web service.
 */
public class JSONTagCommunicator implements TagCommunicator {
	private String url;
	
	public JSONTagCommunicator(String url) {
		this.url = url;
	}
	
	public void exchangeTags() {
		try {
			ObjectMapper mapper = new ObjectMapper();

			TagManager manager = TagManager.getInstance();

			HttpClient httpclient = new DefaultHttpClient();

			LinkedList<NameValuePair> tagQueue = new LinkedList<NameValuePair>();

			Collection<Tag> tags = manager.getAllTags();

			HttpPost request = new HttpPost(url);
			//HttpPost request = new HttpPost("http://192.168.2.50/alldata");
//			HttpPost request = new HttpPost(
//					"http://tunnelksim.appspot.com/command/ExchangeTags");

			for (Tag tag : tags) {
				synchronized (tag) {
					if (tag.isChangedLocally()) {
						tagQueue.add(new BasicNameValuePair(tag.getName(),
								Integer.toString(tag.getValue())));

						tag.setChangedLocally(false);
					}
				}
			}

			if (!tagQueue.isEmpty())
				request.setEntity(new UrlEncodedFormEntity(tagQueue));

			try {
				HttpResponse response = httpclient.execute(request);

				HttpEntity entity = response.getEntity();

				String content = EntityUtils.toString(entity);

				List<Tag> responseTags = mapper.readValue(content,
						new TypeReference<List<Tag>>() {
						});

				for (Tag responseTag : responseTags) {
					Tag tag = manager.getTag(responseTag.getName());

					if (tag == null) {
						tag = responseTag;
						tag.addObserver(this);
						manager.addTag(tag);
					}

					// Check to ensure the tag hasn't been changed locally
					if (!tag.isChangedLocally()) {
						tag.deleteObserver(this);
						tag.setValue(responseTag.getValue());
						tag.addObserver(this);
					}
				}
			} catch (ClientProtocolException e) {
				System.out.println(e.toString());
			} catch (IOException e) {
				System.out.println(e.toString());
			}
		} catch (Exception e) {
			System.out.println(e.toString());
		}
	}

	public void update(Observable observable, Object data) {
		Tag tag = (Tag) observable;

		synchronized (tag) {
			// Check for bad input
			if (observable.getClass() != Tag.class)
				return;

			tag.setChangedLocally(true);
		}
	}
}
