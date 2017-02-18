package com.dgmltn.tyson

import android.media.MediaPlayer
import android.os.AsyncTask
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.widget.Toast
import timber.log.Timber

class MainActivity : AppCompatActivity() {

    val recycler by lazy { findViewById(R.id.ringtones) as RecyclerView }
    val adapter by lazy { RingtonesAdapter(this) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        adapter.listener = { deviceSetRingtone(it.index) }
        recycler.layoutManager = LinearLayoutManager(this)
        recycler.adapter = adapter
    }

    override fun onStart() {
        super.onStart()

        AsyncTask.execute {
            val current = App.device.getRingtone()
            runOnUiThread { adapter.selectedIndex = current }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Particle functions
    ///////////////////////////////////////////////////////////////////////////////////////////////

    private fun doBeepbeep() {
        AsyncTask.execute {
            App.device.beepbeep()
        }
    }

    private fun doDoorbell() {
        AsyncTask.execute {
            App.device.dingdong()
        }
    }

    private fun deviceSetRingtone(index: Int) {
        Timber.e("deviceSetRingtone($index)")
        AsyncTask.execute {
            App.device.setRingtone(index)
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // RTTTL playback
    ///////////////////////////////////////////////////////////////////////////////////////////////

    private var mediaPlayer: MediaPlayer? = null

    private val onCompletion = MediaPlayer.OnCompletionListener {
        stop()
        Toast.makeText(this, "onCompletion", Toast.LENGTH_SHORT).show()
    }

    fun start(rawResId: Int) {
        stop()
        mediaPlayer = MediaPlayer.create(this, rawResId)
        if (mediaPlayer == null) {
            Toast.makeText(this, "unable to play", Toast.LENGTH_SHORT).show()
        } else {
            mediaPlayer?.setOnCompletionListener(onCompletion)
            mediaPlayer?.start()
        }
    }

    fun stop() {
        if (mediaPlayer != null) {
            if (mediaPlayer?.isPlaying ?: false) {
                mediaPlayer?.stop()
            }
            mediaPlayer?.release()
            mediaPlayer = null
        }
    }

}
