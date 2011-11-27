package com.googlecode.tunnelk.test;

import com.googlecode.tunnelk.SetTunnelConditionsActivity;

import android.test.ActivityInstrumentationTestCase2;
import android.view.KeyEvent;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;

public class SetTunnelConditionsActivityTest extends
		ActivityInstrumentationTestCase2<SetTunnelConditionsActivity> {

    private SetTunnelConditionsActivity mActivity;
    private Spinner mWindSpeedSpinner;
    private Spinner mTemperatureSpinner;
    private Spinner mPressureSpinner;
    private SpinnerAdapter mSpinnerData;
    private int mPos;
    public static final int ADAPTER_COUNT = 3;
    public static final int INITIAL_POSITION = 0;
    public static final int TEST_POSITION = 2;
    public static final int TEST_STATE_DESTROY_POSITION = 2;
    
	public SetTunnelConditionsActivityTest() {
	    super("com.googlecode.tunnelk.SetTunnelConditionsActivity", SetTunnelConditionsActivity.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        setActivityInitialTouchMode(false);

        mActivity = getActivity();

        mWindSpeedSpinner = (Spinner) mActivity.findViewById(
                com.googlecode.tunnelk.R.id.windSpeedSpinner
        );
        mTemperatureSpinner = (Spinner) mActivity.findViewById(
                com.googlecode.tunnelk.R.id.temperatureSpinner
        );
        mPressureSpinner = (Spinner) mActivity.findViewById(
                com.googlecode.tunnelk.R.id.pressureSpinner
        );

	    mSpinnerData = mWindSpeedSpinner.getAdapter();
    }

    public void testPreConditions() {
        assertTrue(mWindSpeedSpinner != null);
        assertTrue(mTemperatureSpinner != null);
        assertTrue(mPressureSpinner != null);
        assertTrue(mSpinnerData != null);
        assertEquals(mSpinnerData.getCount(),ADAPTER_COUNT);
    }

    public void testSpinnerUI() {
        mActivity.runOnUiThread(
            new Runnable() {
                public void run() {
                    mWindSpeedSpinner.requestFocus();
                    mWindSpeedSpinner.setSelection(INITIAL_POSITION);
                }
            }
        );

        this.sendKeys(KeyEvent.KEYCODE_DPAD_CENTER);
        for (int i = 1; i <= TEST_POSITION; i++) {
            this.sendKeys(KeyEvent.KEYCODE_DPAD_DOWN);
        }

        this.sendKeys(KeyEvent.KEYCODE_DPAD_CENTER);

        mPos = mWindSpeedSpinner.getSelectedItemPosition();
        assertEquals(mPos,TEST_POSITION);
    }
}

