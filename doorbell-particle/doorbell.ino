/*
 * A Particle Doorbell Circuit
 *
 * This circuit serves 2 purposes:
 * 1. It plays a tune when a button (aka the doorbell) is pressed (or the "doorbell" method is called)
 *
 * curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/[deviceid]/doorbell -d params=x
 *
 * 2. It beeps when any door is opened (or the "beepbeep" method is called)
 *
 * curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/[deviceid]/beepbeep -d params=x
 *
 * 3. You can get the device's IP Address:
 *
 * curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/[deviceid]/ipAddress | jq .result
 *
 */

// https://github.com/technobly/Remote-RTTTL/blob/master/RemoteRTTTL.cpp
// https://community.particle.io/t/a-remote-rtttl-ringtone-text-transfer-language-song-player/1149
// http://www.picaxe.com/RTTTL-Ringtones-for-Tune-Command/
// http://ez4mobile.com/nokiatone/rtttf.htm

// A fun program for the Spark Core that works without the use of the tone() function
// Originally written by Brett Hagman (http://roguerobotics.com),
// Reworked by Technobly (http://technobly.com) for the Spark Core.
//
// This plays RTTTL (RingTone Text Transfer Language) songs
// by bit-banging a selected digital output.
//
// You can get more RTTTL songs from
// http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation
// https://github.com/hitov/megamelody/tree/master/ringtones

#include "SparkIntervalTimer.h"
#include "WebServer.h"
//#include "SparkIntervalTimer/firmware/SparkIntervalTimer.h"
//#include "Webduino/firmware/WebServer.h"

const int PIN_LED = D7;

// Button to trigger playback
// Connect a switch between PIN_DOORBELL_BUTTON and GND
const int PIN_DOORBELL_BUTTON = A5;

// Pin to enable amplifier
const int PIN_AMP_ENABLE = A0;

char* song = (char*)

//"20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
//"2.34kHzBeeps:d=4,o=7,b=240:d,p,d,p,d,p,d,p";
//"A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
"Addams Family:d=8,o=5,b=160:c,4f,a,4f,c,4b4,2g,f,4e,g,4e,g4,4c,2f,c,4f,a,4f,c,4b4,2g,f,4e,c,4d,e,1f,c,d,e,f,1p,d,e,f#,g,1p,d,e,f#,g,4p,d,e,f#,g,4p,c,d,e,f";
//"AlarmDoorBeepBeep:d=4,o=7,b=125:16e,16p,16e";
//"AlarmDoorBeepBoop:d=4,o=7,b=125:16e,16p,16c";
//"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//"Barbie girl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#,8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#,8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#";
// http://www.virtuallysmoothpiano.com/uploads/5/4/9/1/5491021/heads_ringing_piano.pdf
//"DrDre_Keep_Their_Heads_Ringing:d=4,o=5,b=100:4a4,4e,2d,8a4,16a4,8a4,8e,8e,2d";
//"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//"Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//"GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//"Happy Birthday Song:d=4,o=5,b=125:16c,32p,32c,32p,8d,32p,8c,32p,8f,32p,e,16p,16c,32p,32c,32p,8d,32p,8c,32p,8g,32p,f,8p,16c,32p,32c,32p,8c6,32p,8a,32p,8f,32p,8e,32p,8d,32p,16a#,32p,32a#,32p,8a,32p,8f,32p,8g,32p,f";
//"HeyBaby:d=4,o=5,b=900:8a4,16a#4,16b4,16c,16c#,16d,16d#,16e,16f,16f#,16g,16g#,16a,16a#,16b,16c6,8c#6,16d6,16d#6,16e6,16f6,p,p,16a4,16a#4,16b4,16c,16c#,16d,16d#,16e,16f,16f#,16g,16g#,16a,16a#,16b,16a#,16a,16g#,16g,16f#,16f,16e,16d#,16d,16c#,16c,16b4,16a#4,16a4";
//"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
//"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//"KnightRider:d=4,o=5,b=125:16e,16p,16f,16e,16e,16p,16e,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16e,16p,16f,16e,16e,16p,16f,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d";
//"LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
//"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
//"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//"MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
//"Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
//"RickRoll:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.,p,8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,2g6,b,c6.,8b,a,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.";
//"SMBtheme:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
//"SMBunderground:d=16,o=6,b=100:c,c5,a5,a,a#5,a#,2p,8p,c,c5,a5,a,a#5,a#,2p,8p,f5,f,d5,d,d#5,d#,2p,8p,f5,f,d5,d,d#5,d#,2p,32d#,d,32c#,c,p,d#,p,d,p,g#5,p,g5,p,c#,p,32c,f#,32f,32e,a#,32a,g#,32p,d#,b5,32p,a#5,32p,a5,g#5";
//"SMBwater:d=8,o=6,b=225:4d5,4e5,4f#5,4g5,4a5,4a#5,b5,b5,b5,p,b5,p,2b5,p,g5,2e.,2d#.,2e.,p,g5,a5,b5,c,d,2e.,2d#,4f,2e.,2p,p,g5,2d.,2c#.,2d.,p,g5,a5,b5,c,c#,2d.,2g5,4f,2e.,2p,p,g5,2g.,2g.,2g.,4g,4a,p,g,2f.,2f.,2f.,4f,4g,p,f,2e.,4a5,4b5,4f,e,e,4e.,b5,2c.";
//"SMBdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c";
//"Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//"StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//"The Simpsons:d=4,o=5,b=168:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
//"TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//"TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//"Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
//"YellowSubmarine:d=4,o=5,b=125:8a#,8b,2c#6,8a#,8g#,8a#,2f#,8a#,8a#,8g#,8f#,2d#,8a#,8a#,1g#,4c#6,4c#6,4c#6,8c#6,8d#6,8g#,8g#,8g#,8g#,2g#,8g#,8g#,4g#,2g#,8f#,8f#,8f#,8f#,2f#,4c#6,4c#6,4c#6,8c#6,8d#6,8g#,8g#,8g#,8g#,2g#,8g#,8g#,4g#,2g#,8f#,8f#";

//-------------------------
// Bit-banging RTTTL Player
//-------------------------

#define OCTAVE_OFFSET 0

// Notes defined in microseconds (Period/2)
// from note C to B, Octaves 3 through 7
// Convert to frequency by f = 500000 / notes[i]
int notes[] =
{0,
3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,
1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,
956,903,852,804,759,716,676,638,602,568,536,506,
478,451,426,402,379,358,338,319,301,284,268,253,
239,226,213,201,190,179,169,159,151,142,134,127};

byte default_dur = 4;
byte default_oct = 6;
byte lowest_oct = 3;
int bpm = 63;
int num;
long wholenote;
long duration;
byte note;
byte scale;
bool songDone = false;
char *songPtr;

void begin_rtttl(char *p) {
  // Absolutely no error checking in here

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 2;  // this is the time for whole note (in milliseconds)

  // Save current song pointer...
  songPtr = p;
}

bool next_rtttl() {

  char *p = songPtr;
  // if notes remain, play next note
  if(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }

    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // Save current song pointer...
    songPtr = p;

    // now play the note
    if(note) {
      tone(500000 / notes[(scale - lowest_oct) * 12 + note]);
      delay(duration * 2);
      noTone();
    }
    else {
      delay(duration * 2);
    }
    return 1; // note played successfully.
  }
  else {
    return 0; // all done
  }
}

//----------------------
// DAC-based tone player
//----------------------

int SINE_LENGTH = 100;
byte SINE[] = {127, 134, 142, 150, 158, 166, 173, 181, 188, 195, 201, 207, 213, 219, 224, 229, 234, 238, 241, 245, 247, 250, 251, 252, 253, 254, 253, 252, 251, 250, 247, 245, 241, 238, 234, 229, 224, 219, 213, 207, 201, 195, 188, 181, 173, 166, 158, 150, 142, 134, 127, 119, 111, 103, 95, 87, 80, 72, 65, 58, 52, 46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 3, 2, 1, 0, 0, 0, 1, 2, 3, 6, 8, 12, 15, 19, 24, 29, 34, 40, 46, 52, 58, 65, 72, 80, 87, 95, 103, 111, 119,};

IntervalTimer audio_clock;

// Bit shifting volume: 0-6
int VOLUME = 0;

volatile float t = 0;
volatile float step;

void noTone() {
    audio_clock.end();
    t = 0;
    analogWrite(DAC1, 0);
    analogWrite(DAC2, 0);
}

void tone(int freq) {
    if (freq == 0) {
        noTone();
    }
    else {
        step = freq / 400.0;
        audio_clock.begin(playback_handler, 20, uSec);
    }
}

void playback_handler(void) {
    int s = SINE[(int)t];

    // The "+" side of the signal: 127 = 0, 255 = 4096
    analogWrite(DAC1, s <= 127 ? 0 : (s - 127) << VOLUME);

    // The "-" side of the signal: 127 = 0, 0 = 4096
    analogWrite(DAC2, s >= 127 ? 0 : (127 - s) << VOLUME);

    // Increment t
    t += step;
    while (t > SINE_LENGTH) t -= SINE_LENGTH;
}

//-------------------
// Particle functions
//-------------------

bool playing = false;

void doorbell() {
    if (playing == true) {
        return;
    }
    VOLUME = 2;
    playing = true;
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_AMP_ENABLE, HIGH);
    begin_rtttl(song);
}

void beepbeep() {
    if (playing == true) {
        return;
    }
    VOLUME = 0;
    char* beepbeep = (char*) "AlarmDoorBeepBeep:d=4,o=7,b=125:16e,16p,16e";
    playing = true;
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_AMP_ENABLE, HIGH);
    begin_rtttl(beepbeep);
}

int particle_doorbell(String command) {
    doorbell();
    return 42;
}

int particle_beepbeep(String command) {
    beepbeep();
    return 69;
}

//----------------
// Mini Web Server
//----------------

char myIpString[24];

#define PREFIX ""
WebServer webserver(PREFIX, 80);

void webserver_beepbeep(WebServer &server, WebServer::ConnectionType type, char *, bool) {
    server.httpSuccess("application/json");

    if (type != WebServer::HEAD) {
        beepbeep();
        /* this defines some HTML text in read-only memory aka PROGMEM.
         * This is needed to avoid having the string copied to our limited
         * amount of RAM. */
        P(okMessage) = "{\"status\": \"ok\"}";

        /* this is a special form of print that outputs from PROGMEM */
        server.printP(okMessage);
    }
}

void setupWebserver() {
    // Report this device's ip address as a normal Particle variable
    IPAddress myIp = WiFi.localIP();
    sprintf(myIpString, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
    Particle.variable("ipAddress", myIpString, STRING);

    /* run the same command if you try to load /index.html, a common
     * default page name */
    webserver.addCommand("beepbeep", &webserver_beepbeep);

    /* start the webserver */
    webserver.begin();
}

void loopWebserver() {
    char buff[64];
    int len = 64;

    /* process incoming connections one at a time forever */
    webserver.processConnection(buff, &len);
}

//-------------------
// MAIN PROGRAM
//-------------------

void setup(void) {
    pinMode(DAC1, OUTPUT);
    pinMode(DAC2, OUTPUT);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_DOORBELL_BUTTON, INPUT);
    pinMode(PIN_AMP_ENABLE, OUTPUT);

    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_AMP_ENABLE, LOW);

    Particle.function("doorbell", particle_doorbell);
    Particle.function("beepbeep", particle_beepbeep);

    setupWebserver();
 }

bool clicked = false;

void loop(void) {
    // The main loop() processes one note of the song at a time
    // to avoid blocking the background tasks for too long or else
    // the Spark Core would disconnect from the Cloud.

    if (playing) {
        // Play next note
        bool next = next_rtttl();

        // Check to see if that was the last note
        if (!next) {
            digitalWrite(PIN_LED, LOW); // Turn off the onboard Blue LED.
            digitalWrite(PIN_AMP_ENABLE, LOW);
            playing = false;
            analogWrite(DAC1, 0);
            analogWrite(DAC2, 0);
        }
    }

    else {
        // Read the state of the pushbutton value
        bool state = digitalRead(PIN_DOORBELL_BUTTON) == LOW;

        // Check if the pushbutton changed value
        if (state != clicked) {
            delay(500); // Debounce
            clicked = state;

            // They pressed the button. Start the music
            if (clicked) {
                Particle.publish("doorbell", NULL, 60, PRIVATE);
                doorbell();
            }
        }

        loopWebserver();
    }
}
