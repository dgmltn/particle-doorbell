package com.dgmltn.tyson

import android.os.AsyncTask
import io.particle.android.sdk.cloud.ParticleDevice

/**
 * Created by dmelton on 2/16/17.
 */

class ParticleDoorbellDevice(private val sdkDevice: ParticleDevice) : IDoorbellDevice {

    init {
        AsyncTask.execute {
            sdkDevice.refresh()
        }
    }

    override fun dingdong(): Int {
        return sdkDevice.callFunction("doorbell")
    }

    override fun beepbeep(): Int {
        return sdkDevice.callFunction("beepbeep")
    }

    override fun getRingtone(): Int {
        return sdkDevice.getIntVariable("current")
    }

    override fun setRingtone(ringtone: Int): Int {
        val sindex = "$ringtone"
        return sdkDevice.callFunction("ringtone", listOf(sindex))
    }
}
