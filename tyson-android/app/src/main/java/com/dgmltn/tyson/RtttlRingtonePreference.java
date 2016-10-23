package com.dgmltn.tyson;

/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.media.MediaPlayer;
import android.preference.ListPreference;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;

public class RtttlRingtonePreference extends ListPreference {

    private static final String TAG = RtttlRingtonePreference.class.getSimpleName();

    public RtttlRingtonePreference(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    public RtttlRingtonePreference(Context context, AttributeSet attrs, int defStyleAttr) {
        this(context, attrs, defStyleAttr, 0);
    }

    public RtttlRingtonePreference(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public RtttlRingtonePreference(Context context) {
        this(context, null);
    }

    private void init() {
        setLayoutResource(R.layout.preference_material);
        readEntriesAndValues();
    }

    private int mClickedDialogEntryIndex = 0;

    @Override
    protected void onPrepareDialogBuilder(AlertDialog.Builder builder) {
        super.onPrepareDialogBuilder(builder);

        // Override the default behavior of the AlertDialog

        mClickedDialogEntryIndex = getValueIndex();
        builder.setSingleChoiceItems(
            getEntries(),
            mClickedDialogEntryIndex,
            new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    mClickedDialogEntryIndex = which;
                    String rtttl = getEntryValues()[mClickedDialogEntryIndex].toString();
                    Log.e("DOUG", "play rtttl: " + rtttl);
                    start(rttlResIds[mClickedDialogEntryIndex]);
                }
            });

        builder.setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                RtttlRingtonePreference.this.onClick(dialog, DialogInterface.BUTTON_NEGATIVE);
                stop();
                dialog.dismiss();
            }
        });

        builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                RtttlRingtonePreference.this.onClick(dialog, DialogInterface.BUTTON_POSITIVE);
                stop();
                dialog.dismiss();
            }
        });
    }

    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);
        stop();

        if (positiveResult && mClickedDialogEntryIndex >= 0 && getEntryValues() != null) {
            String value = getEntryValues()[mClickedDialogEntryIndex].toString();
            if (callChangeListener(value)) {
                setValue(value);
            }
        }
    }

    private int getValueIndex() {
        return findIndexOfValue(getValue());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Reading RTTTL songs from raw resources
    ///////////////////////////////////////////////////////////////////////////////////////////////

    int[] rttlResIds;

    private void readEntriesAndValues() {
        // Assume all "raw" resources are the rtttl ringtones
        Field[] fields = R.raw.class.getFields();

        CharSequence[] entries = new CharSequence[fields.length];
        CharSequence[] values = new CharSequence[fields.length];
        rttlResIds = new int[fields.length];

        for (int count = 0; count < fields.length; count++) {
            try {
                int rawResId = fields[count].getInt(null);
                String rawValue = getStringFromRaw(getContext(), rawResId);
                int colon = rawValue.indexOf(':');
                entries[count] = rawValue.substring(0, colon);
                values[count] = rawValue;
                rttlResIds[count] = rawResId;
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }

        setEntries(entries);
        setEntryValues(values);
    }

    private String getStringFromRaw(Context c, int rawResId) throws IOException {
        Resources r = c.getResources();
        InputStream is = r.openRawResource(rawResId);
        String statesText = convertStreamToString(is);
        is.close();
        return statesText;
    }

    private String convertStreamToString(InputStream is) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        int i = is.read();
        while (i != -1) {
            baos.write(i);
            i = is.read();
        }
        return baos.toString();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // RTTTL playback
    ///////////////////////////////////////////////////////////////////////////////////////////////

    private static MediaPlayer mediaPlayer;

    private MediaPlayer.OnCompletionListener onCompletion = new MediaPlayer.OnCompletionListener() {
        @Override
        public void onCompletion(MediaPlayer mp) {
            stop();
            Toast.makeText(getContext(), "onCompletion", Toast.LENGTH_SHORT).show();
        }
    };

    public void start(int rawResId) {
        stop();
        mediaPlayer = MediaPlayer.create(getContext(), rawResId);
        if (mediaPlayer == null) {
            Toast.makeText(getContext(), "unable to play", Toast.LENGTH_SHORT).show();
        }
        else {
            mediaPlayer.setOnCompletionListener(onCompletion);
            mediaPlayer.start();
        }
    }

    public void stop() {
        if (mediaPlayer != null) {
            if (mediaPlayer.isPlaying()) {
                mediaPlayer.stop();
            }
            mediaPlayer.release();
            mediaPlayer = null;
        }
    }

}
