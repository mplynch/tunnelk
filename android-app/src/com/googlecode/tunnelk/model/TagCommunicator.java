package com.googlecode.tunnelk.model;

import java.util.Observer;

public interface TagCommunicator extends Observer {
	/**
	 * Exchanges the values of all tags with the controller.
	 */
	public void exchangeTags();
}
