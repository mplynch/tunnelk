package com.googlecode.tunnelk.model;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

public class JTag extends Tag {

	public JTag() {
		super("Light On/Off", "", 1, TagType.Relay, false);
	}

	public void setValue(int newValue) {
		new JThread(newValue).start();
	}

	class JThread extends Thread {
		int value;

		JThread(int v) {
			value = v;
		}

		public void run() {
			try {
				HttpClient httpclient = new DefaultHttpClient();
				HttpPost httppost = new HttpPost("http://192.168.1.37/lightswitch");
				try {
					List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
					nameValuePairs.add(new BasicNameValuePair("lightswitch", Integer.toString(value)));
					httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
					httpclient.execute(httppost);
				} catch (ClientProtocolException e) {
				} catch (IOException e) {
				}
			} catch (Exception e) {
			}
		}
	}

}
