package com.googlegode.tunnelk.views;

import java.util.Observable;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.TimeHistoryPlotActivity;
import com.googlecode.tunnelk.model.Tag;

import android.content.Context;
import android.content.Intent;
import android.widget.TextView;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;

public class TagReadOnlyText extends TagLayout implements
		View.OnLongClickListener, View.OnCreateContextMenuListener,
		MenuItem.OnMenuItemClickListener {

	private final Context context;

	public TagReadOnlyText(Context context, Tag tag) {
		super(context, tag);

		this.context = context;
	}

	@Override
	protected void initialize() {
		this.inflateLayout(R.layout.tag_layout_read_only_text);

		TextView nameLabel = (TextView) findViewById(R.id.tagLabel);

		if (nameLabel != null) {
			nameLabel.setText(tag.getName());
		}

		TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

		if (valueLabel != null)
			valueLabel.setText(tag.getValue() + " " + tag.getUnits());

		this.setOnCreateContextMenuListener(this);

		this.setOnLongClickListener(this);
	}

	public void onCreateContextMenu(ContextMenu menu, View v,
			ContextMenuInfo menuInfo) {
		MenuInflater inflater = new MenuInflater(context);

		inflater.inflate(R.menu.tag_menu, menu);

		menu.getItem(0).setOnMenuItemClickListener(this);
	}

	public boolean onLongClick(View arg0) {
		this.showContextMenu();

		// post(new Runnable() {
		// public void run() {
		// Intent intent = new Intent();
		// intent.setClass(context, TimeHistoryPlotActivity.class);
		// intent.putExtra("TUNNELKTAGNAME", tag.getName());
		// context.startActivity(intent);
		// }
		// });

		return true;
	}

	public boolean onMenuItemClick(MenuItem arg0) {
		Intent intent = new Intent();

		switch (arg0.getItemId()) {
		case R.id.trend:
			intent.putExtra(getResources().getString(R.string.trending_tags),
					new String[] { tag.getName() });
			intent.setClass(context, TimeHistoryPlotActivity.class);
			context.startActivity(intent);
			break;

		default:
			return false;
		}

		return true;
	}

	@Override
	public void update(Observable observable, Object data) {
		post(new Runnable() {
			public void run() {
				TextView valueLabel = (TextView) findViewById(R.id.tagValueLabel);

				if (valueLabel != null)
					valueLabel.setText(tag.getValue() + " " + tag.getUnits());
			}
		});
	}
}
