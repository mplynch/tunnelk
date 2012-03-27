package com.googlecode.tunnelk.model;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

public class MTag extends Tag {

	public MTag() {
		super("Temperature Sensor 1", "", 1, TagType.Temperature, true);
	}

	@Override
	public void setValue(int value) {
		JThread thread = new JThread();

		thread.start();

		while (thread.isAlive())
			;

		super.setValue(thread.result);
	}

	class JThread extends Thread {
		public int result;

		JThread() {
		}

		private String inputStreamToString(InputStream is) {
			String line = "";
			StringBuilder total = new StringBuilder();

			// Wrap a BufferedReader around the InputStream
			BufferedReader rd = new BufferedReader(new InputStreamReader(is));

			// Read response until the end
			try {
				while ((line = rd.readLine()) != null) {
					total.append(line);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			// Return full string
			return total.toString();
		}

		private String[] getTemperatureValues(String content) {
			String[] values = content.split(",");
			return values;
		}

		public void run() {
			try {
				HttpClient httpclient = new DefaultHttpClient();
				HttpGet request = new HttpGet("http://192.168.2.50/lightswitch");

				try {
					HttpResponse response = httpclient.execute(request);
					HttpEntity entity = response.getEntity();
					String content = inputStreamToString(entity.getContent());
					String[] values = getTemperatureValues(content);
					double value = (double)Integer.parseInt(values[0]);

					value = value * 0.004882814;
					value = (value - 0.5) * 100;
					value = (value * 1.8) + 32;
					
					this.result = (int)value;
					
				} catch (ClientProtocolException e) {
				} catch (IOException e) {
				}
			} catch (Exception e) {
			}
		}
	}

}
