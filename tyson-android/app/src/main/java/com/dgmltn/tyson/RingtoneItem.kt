package com.dgmltn.tyson

import android.content.Context
import android.content.res.Resources

import java.io.ByteArrayOutputStream
import java.io.IOException
import java.io.InputStream
import java.util.ArrayList

class RingtoneItem internal constructor(context: Context, val index: Int) {

    val rawResId: Int = rttlResIds[index]
    val rtttl by lazy { getStringFromRaw(context, rawResId) }
    val entry by lazy { rtttl.substring(0, rtttl.indexOf(':'))}

    companion object {

        private val rttlResIds = intArrayOf(
            R.raw.silent,
            R.raw.barbie_girl,
            R.raw.beep_beep,
            R.raw.beep_boop,
            R.raw.dr_dre_keep_their_heads_ringin,
            R.raw.gadget,
            R.raw.happy_birthday_song,
            R.raw.jeopardy,
            R.raw.mission_impossible,
            R.raw.rick_roll,
            R.raw.smb_theme,
            R.raw.star_wars,
            R.raw.top_gun,
            R.raw.we_wish_you_a_merry_christmas,
            R.raw.yellow_submarine
        )

        fun generate(context: Context) =
            rttlResIds.indices.map { RingtoneItem(context, it) }


        private fun getStringFromRaw(c: Context, rawResId: Int): String {
            val r = c.resources
            val inp = r.openRawResource(rawResId)
            val statesText = convertStreamToString(inp)
            inp.close()
            return statesText
        }

        private fun convertStreamToString(inp: InputStream): String {
            val baos = ByteArrayOutputStream()
            var i = inp.read()
            while (i != -1) {
                baos.write(i)
                i = inp.read()
            }
            return baos.toString()
        }
    }

}
