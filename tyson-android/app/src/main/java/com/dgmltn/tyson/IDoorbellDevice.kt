package com.dgmltn.tyson

/**
 * Created by dmelton on 2/16/17.
 */

interface IDoorbellDevice {
    fun dingdong(): Int
    fun beepbeep(): Int
    fun getRingtone(): Int
    fun setRingtone(ringtone: Int): Int
}
