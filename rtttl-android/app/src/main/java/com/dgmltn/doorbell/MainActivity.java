package com.dgmltn.doorbell;

import android.media.AudioAttributes;
import android.media.MediaDataSource;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

import butterknife.ButterKnife;
import butterknife.OnClick;

import static android.media.AudioAttributes.CONTENT_TYPE_MUSIC;
import static android.media.AudioAttributes.USAGE_MEDIA;

public class MainActivity extends AppCompatActivity {

    MediaPlayer mediaPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.start)
    public void start(View view) {
        if (mediaPlayer != null) {
            mediaPlayer.release();
        }

//		mediaPlayer = MediaPlayer.create(getApplicationContext(), R.raw.halloween);
        mediaPlayer = new MediaPlayer();

        AudioAttributes attributes = new AudioAttributes
            .Builder()
            .setContentType(CONTENT_TYPE_MUSIC)
            .setUsage(USAGE_MEDIA)
            .build();
        mediaPlayer.setAudioAttributes(attributes);

        //TODO: fix flutter plugin to use setAudioAttributes
//			mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);

        MediaDataSource source = new StringMediaDataSource(
            "Bod:d=8,o=5,b=250:a,b,a,4c#6,a,4c#6,c#6,4b,16a,16b,16a,e,4p,e,f#,e,4g,4g,4g,4f#,e," +
                "2p,a,b,a,4c#6,a," +
                "4c#6,c#6,4b,16a,16b,16a,e,4p,e,f#,e,4g,4g,4g,4g,g#,a,b,4e,4p,a,c#6,a,p,f#,e,4p,a,c#6,a,p,f#,e,2p,4f#,4a,4a,4b,4c6,b,a,p,a,b,a," +
				"4c#6,a,4c#6,c#6,4b,16a,16b,16a,e,4p,e,f#,e,4g,4g,4g,4g#,a");
        mediaPlayer.setDataSource(source);
        mediaPlayer.setOnCompletionListener(onCompletion);
        mediaPlayer.start();
    }

    @OnClick(R.id.stop)
    public void stop(View view) {
        if (mediaPlayer != null) {
            mediaPlayer.stop();
            mediaPlayer.release();
        }
    }

    MediaPlayer.OnCompletionListener onCompletion = new MediaPlayer.OnCompletionListener() {
        @Override
        public void onCompletion(MediaPlayer mp) {
            mediaPlayer.release();
            Toast.makeText(getApplicationContext(),
                           "onCompletion",
                           Toast.LENGTH_LONG)
                 .show();
        }
    };

    class StringMediaDataSource extends MediaDataSource {

        private final byte[] mSource;

        public StringMediaDataSource(String source) {
            mSource = source.getBytes();
        }

        @Override
        public int readAt(long position, byte[] buffer, int offset, int size) {

            int length = mSource.length;
            if (position >= length) {
                return -1; // -1 indicates EOF
            }

            if (position + size > length) {
                size -= (position + size) - length;
            }

            System.arraycopy(mSource, (int) position, buffer, offset, size);
            return size;
        }

        @Override
        public long getSize() {
            return mSource.length;
        }

        @Override
        public void close() {
        }
    }
}
