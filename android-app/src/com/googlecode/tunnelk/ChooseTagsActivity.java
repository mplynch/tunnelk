package com.googlecode.tunnelk;

import java.util.Collection;
import java.util.ArrayList;

import com.googlecode.tunnelk.model.HardCodedTagCommunicator;
import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagCommunicator;
import com.googlecode.tunnelk.model.TagManager;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class ChooseTagsActivity extends Activity {
	private ArrayAdapter<Tag> adapter;

	private ListView listView;

	protected void onCreate(Bundle savedInstanceState) {
		Tag[] tagArray;

		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.list_view_dialog);

		listView = (ListView) findViewById(R.id.listViewChooser);
		listView.setItemsCanFocus(false);
		listView.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);

		Collection<Tag> tags = TagManager.getInstance().getAllTags();

		if (tags.size() == 0)
			tagArray = new Tag[] {};
		else
			tagArray = tags.toArray(new Tag[TagManager.getInstance().size()]);

		adapter = new ArrayAdapter<Tag>(this,
				android.R.layout.simple_list_item_multiple_choice, tagArray);

		listView.setAdapter(adapter);
	}

	private Collection<String> getCheckedTagNames() {
		ArrayList<String> checkedTags = new ArrayList<String>();
		int count = adapter.getCount();

		for (int i = 0; i < count; i++) {
			if (listView.isItemChecked(i)) {
				checkedTags.add(adapter.getItem(i).getName());
			}
		}

		return checkedTags;
	}

	public void onButtonClicked(View v) {
		int id = v.getId();
		Intent intent = new Intent();

		switch (id) {
		case R.id.buttonCancel:
			setResult(RESULT_CANCELED, intent);
			finish();
			break;

		case R.id.buttonOK:
			intent.putExtra(
					getResources().getString(R.string.trending_tags),
					(String[]) getCheckedTagNames().toArray(
							new String[TagManager.getInstance().size()]));
			setResult(RESULT_OK, intent);
			finish();
			break;

		default:
			return;
		}
	}
}
