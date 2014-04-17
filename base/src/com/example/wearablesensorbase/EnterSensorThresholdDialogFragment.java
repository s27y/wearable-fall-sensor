package com.example.wearablesensorbase;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

/**
 * @author Yang
 *
 */
public class EnterSensorThresholdDialogFragment extends DialogFragment {
	NoticeDialogListener mListener;
	float inputThresholding=  0f;
	public interface NoticeDialogListener {
        public void onDialogPositiveClick(DialogFragment dialog);
        public void onDialogNegativeClick(DialogFragment dialog);
    }
	
	public float getInputThresholding()
	{
		return inputThresholding;
	}
	// Override the Fragment.onAttach() method to instantiate the NoticeDialogListener
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        // Verify that the host activity implements the callback interface
        try {
            // Instantiate the NoticeDialogListener so we can send events to the host
            mListener = (NoticeDialogListener) activity;
        } catch (ClassCastException e) {
            // The activity doesn't implement the interface, throw exception
            throw new ClassCastException(activity.toString()
                    + " must implement NoticeDialogListener");
        }
    }
	
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
   	 AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
   	 LayoutInflater inflater = getActivity().getLayoutInflater();
   	 View promptsView = inflater.inflate(R.layout.dialog_sensor_thresholding, null);
   	 
   	builder.setView(promptsView);

		final EditText userInput = (EditText) promptsView
						.findViewById(R.id.editTextDialogUserInput);

		// set dialog message
		builder
			.setCancelable(false)
			.setPositiveButton("OK",
			  new DialogInterface.OnClickListener() {
			    public void onClick(DialogInterface dialog,int id) {
				// get user input and set it to result
				// edit text
			    	//inputThresholding
			    	String value = userInput.getText().toString();
			    	try{
			    		inputThresholding =Float.parseFloat(value);
			    	}catch(Exception e)
			    	{
			    	}
			    	mListener.onDialogPositiveClick(EnterSensorThresholdDialogFragment.this);
			    }
			  })
			.setNegativeButton("Cancel",
			  new DialogInterface.OnClickListener() {
			    public void onClick(DialogInterface dialog,int id) {
				dialog.cancel();
				mListener.onDialogNegativeClick(EnterSensorThresholdDialogFragment.this);
			    }
			  });

		// create alert dialog
		AlertDialog alertDialog = builder.create();

		return alertDialog;

    }
    

}