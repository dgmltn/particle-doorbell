package com.dgmltn.doorbell;

import android.media.MediaPlayer;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

import butterknife.ButterKnife;
import butterknife.OnClick;

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

		mediaPlayer = MediaPlayer.create(getApplicationContext(), R.raw.ringdingdong);
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
				Toast.LENGTH_LONG).show();
		}
	};
}
