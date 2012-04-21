package com.googlecode.tunnelk.views;

import java.util.List;

import com.googlecode.tunnelk.R;
import com.googlecode.tunnelk.model.Tag;
import com.googlecode.tunnelk.model.TagManager;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.RelativeLayout;
import android.widget.Toast;
import android.widget.ToggleButton;

public class TagLayoutAdapter extends ArrayAdapter<Tag> implements
		OnCheckedChangeListener {
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

				toggleButtonLED.setVisibility(View.INVISIBLE);

				Tag ledTag = TagManager.getInstance().getTag(
						tag.getName() + "LED");

				if (toggleButtonLED != null) {
					toggleButtonLED.setVisibility(View.INVISIBLE);

					if (ledTag != null) {
						Drawable background;

						toggleButtonLED.setTag(ledTag);

						toggleButtonLED.setChecked(ledTag.getValue() == 1);

						if (toggleButtonLED.isChecked()) {
							tag.setValue(1);

							background = getContext().getResources()
									.getDrawable(R.drawable.led_on);
						}

						else {
							tag.setValue(0);

							background = getContext().getResources()
									.getDrawable(R.drawable.led_off);
						}

						toggleButtonLED.setOnCheckedChangeListener(this);

						toggleButtonLED.setBackgroundDrawable(background);

						toggleButtonLED.setVisibility(View.VISIBLE);
					}
				}

				ToggleButton buttonGraph = (ToggleButton) v.findViewById(R.id.toggleButtonGraph);

				if (buttonGraph != null) {
					buttonGraph.setTag(tag);

					buttonGraph.setOnCheckedChangeListener(this);
				}
			}
		}

		return v;
	}

	private void toggleLED(ToggleButton button) {
		Drawable background;
		Tag tag = (Tag) button.getTag();

		if (button.isChecked()) {
			tag.setValue(1);

			background = getContext().getResources().getDrawable(
					R.drawable.led_on);
		}

		else {
			tag.setValue(0);

			background = getContext().getResources().getDrawable(
					R.drawable.led_off);
		}

		button.setBackgroundDrawable(background);
	}

	private void toggleGraph(ToggleButton button) {
		Drawable background;
		Tag tag = (Tag) button.getTag();

		if (button.isChecked()) {
			background = getContext().getResources().getDrawable(
					R.drawable.graph_on);
			
			tag.setGraphToggled(true);
		}

		else {
			background = getContext().getResources().getDrawable(
					R.drawable.graph_off);
			
			tag.setGraphToggled(false);
		}

		button.setBackgroundDrawable(background);
	}

	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		int id = buttonView.getId();

		if (id == R.id.toggleButtonLED) {
			this.toggleLED((ToggleButton) buttonView);
		}

		else if (id == R.id.toggleButtonGraph) {
			this.toggleGraph((ToggleButton) buttonView);
		}
	}
}
