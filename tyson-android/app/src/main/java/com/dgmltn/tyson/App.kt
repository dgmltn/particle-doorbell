package com.dgmltn.tyson

import android.app.Application
import android.os.AsyncTask
import android.util.Log

import io.particle.android.sdk.cloud.ParticleCloud
import io.particle.android.sdk.cloud.ParticleCloudException
import io.particle.android.sdk.cloud.ParticleCloudSDK
import io.particle.android.sdk.cloud.ParticleDevice
import timber.log.Timber

/**
 * Created by dmelton on 12/22/16.
 */

class App : Application() {

    override fun onCreate() {
        super.onCreate()
        if (BuildConfig.DEBUG) {
            Timber.plant(Timber.DebugTree())
        }
        else {
            // Don't log on production
        }
        ParticleCloudSDK.init(this)

        AsyncTask.execute {
            device.refresh()
        }
    }

    companion object {
        private val cloud by lazy {
            val it = ParticleCloudSDK.getCloud()
            Timber.e("got cloud: ${it.accessToken}")
            it.logIn(BuildConfig.PARTICLE_USERNAME, BuildConfig.PARTICLE_PASSWORD)
            Timber.e("Logged In")
            it
        }

        val device: ParticleDevice by lazy {
            val it = cloud.getDevice(BuildConfig.PARTICLE_TYSON_DEVICE_ID)
            Timber.e("Got Device")
            it
        }
    }
}
