package com.googlecode.tunnelk;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class ChooseTunnelActivity extends OptionsMenuActivity implements
		OnItemClickListener {
	private final String TAG = "ChooseTunnelActivity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		String[] tunnelTypes = { getResources().getString(R.string.physical),
				getResources().getString(R.string.virtual) };

		setContentView(R.layout.choose_tunnel_type);

		ListView listView = (ListView) findViewById(R.id.listViewTunnelType);

		listView.setAdapter(new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_1, tunnelTypes));

		listView.setOnItemClickListener(this);
	}

	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {
		Intent intent = new Intent();

		String selection = (String) (((TextView) view).getText());

		if (selection.equals(this.getResources().getString(R.string.physical))) {
			intent.setClass(this, PhysicalTunnelHMIActivity.class);
			this.startActivity(intent);
		}

		else if (selection.equals(this.getResources().getString(
				R.string.virtual))) {
			intent.setClass(this, TunnelkInitialActivity.class);
			this.startActivity(intent);
		}
	}
}
