package com.googlecode.tunnelk.model;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * A TagCommunicator implemented using a JSON-based web service.
 */
public class JSONTagCommunicator implements TagCommunicator {
	private final String baseUrl = "http://tunnelksim.appspot.com";
	private final String getTagsPath = "/command/GetTags";
	private final String setTagsPath = "/command/SetTags";

	public void getTagValues() {
		try {
			ObjectMapper mapper = new ObjectMapper();

			TagManager manager = TagManager.getInstance();

			HttpClient httpclient = new DefaultHttpClient();

			HttpPost request = new HttpPost(baseUrl + getTagsPath);

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
	}

	public void update(Tag tag) {
		List<Tag> tags = new ArrayList<Tag>();
		tags.add(tag);
		update(tags);
	}

	public void update(Collection<Tag> tag) {
		// TODO Auto-generated method stub

	}

}
