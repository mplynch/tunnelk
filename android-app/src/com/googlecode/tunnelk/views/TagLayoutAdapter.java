package com.googlecode.tunnelk.views;

import java.util.Collection;
import java.util.List;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagManager;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.ToggleButton;

public class TagLayoutAdapter extends ArrayAdapter<Tag> {
	private List<Tag> tags;

	public TagLayoutAdapter(Context context, int textViewResourceId,
			List<Tag> tags) {
		super(context, textViewResourceId, tags);

		this.tags = tags;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View v = convertView;
		
		if (v == null) {
			LayoutInflater vi = (LayoutInflater) this.getContext()
					.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			v = vi.inflate(R.layout.tag_list_item, null);

			Tag tag = tags.get(position);

			if (tag != null) {
				RelativeLayout tagListItemLayout = (RelativeLayout) v
						.findViewById(R.id.tagListItemLayout);

				if (tagListItemLayout != null) {
					TagLayout tagLayout = TagLayoutFactory.create(getContext(),
							tag);

					if (tagLayout != null)
						tagListItemLayout.addView(tagLayout);
				}

				ToggleButton toggleButtonLED = (ToggleButton) v
						.findViewById(R.id.toggleButtonLED);

				Tag ledTag = TagManager.getInstance().getTag(
						tag.getName() + "LED");

				if ((toggleButtonLED != null) && (ledTag != null)) {
					toggleButtonLED.setTag(tag);
					
					toggleButtonLED.setChecked(tag.getValue() == 1);
				}

				Button buttonGraph = (Button) v.findViewById(R.id.buttonGraph);

				if (buttonGraph != null) {
					buttonGraph.setTag(tag);
				}
			}
		}

		return v;
	}
}
