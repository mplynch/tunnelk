package com.googlecode.tunnelk;

import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class SettingsActivity extends Activity {
	SharedPreferences preferences;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Context context = getApplicationContext();
		preferences = context.getSharedPreferences("TunnelkPrefs", 0);

		setContentView(R.layout.settings_layout);
	}

	@Override
	protected void onResume() {
		super.onResume();

		Context context = getApplicationContext();
		SharedPreferences p = context.getSharedPreferences("TunnelkPrefs", 0);

		String url = p.getString("@string/controller_address",
				"http://192.168.2.50/alldata");

		EditText editText = (EditText) findViewById(R.id.editTextControllerAddress);

		if (editText != null)
			editText.setText(url);
	}

	public void buttonOK_onClick(View v) {
		String urlString;

		EditText editText = (EditText) findViewById(R.id.editTextControllerAddress);

		if (editText != null) {
			urlString = editText.getText().toString();
			
			try {
				new URL(urlString);
			} catch (MalformedURLException e) {
				Toast.makeText(this, "Please enter a valid URL.", Toast.LENGTH_SHORT);
				
				return;
			}

			SharedPreferences.Editor editor = preferences.edit();

			editor.putString("@string/controller_address", urlString);
			
			editor.apply();
		}
		
		setResult(RESULT_OK, new Intent());
		finish();
	}
	
	public void buttonCancel_onClick(View v) {
		setResult(RESULT_CANCELED, new Intent());
		finish();
	}
}
