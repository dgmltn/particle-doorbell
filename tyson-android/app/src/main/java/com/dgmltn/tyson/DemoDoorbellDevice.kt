package com.dgmltn.tyson

/**
 * Created by dmelton on 2/16/17.
 */

class DemoDoorbellDevice : IDoorbellDevice {

    var current = 4

    override fun dingdong(): Int {
        return 1
    }

    override fun beepbeep(): Int {
        return 1
    }

    override fun getRingtone(): Int {
        Thread.sleep(5000)
        return current
    }

    override fun setRingtone(ringtone: Int): Int {
        current = ringtone
        return ringtone
    }
}
