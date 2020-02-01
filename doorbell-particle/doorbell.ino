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
#include "MQTT.h"

const int PIN_LED = D7;

// Button to trigger playback
// Connect a switch between PIN_DOORBELL_BUTTON and GND
const int PIN_DOORBELL_BUTTON = A5;

// Pin to enable amplifier
const int PIN_AMP_ENABLE = A0;

char beepbeepBuffer[1024];
char doorbellBuffer[1024];

char* rickRoll = (char*)"Rick Roll:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.";
char* happyBirthday = (char*)"Happy Birthday:d=4,o=5,b=125:16c,32p,32c,32p,8d,32p,8c,32p,8f,32p,e,16p,16c,32p,32c,32p,8d,32p,8c,32p,8g,32p,f,8p,16c,32p,32c,32p,8c6,32p,8a,32p,8f,32p,8e,32p,8d,32p,16a#,32p,32a#,32p,8a,32p,8f,32p,8g,32p,f";

char* doorbellInit = (char*)"DrDre Keep Their Heads Ringin:d=4,o=5,b=100:4a4,4e,2d,8a4,16a4,8a4,8e,8e,2d";
char* beepbeepInit = (char*)"Beep Beep:d=4,o=7,b=125:16e,16p,16e";

// Here are some other good ones:
//    (char*)"Silent:d=4,b=125:p",
//    (char*)"Addams Family:d=4,o=6,b=50:32p,32c#,16f#,32a#,16f#,32c#,16c,8g#,32f#,16f,32g#,16f,32c#,16a#5,8f#,32c#,16f#,32a#,16f#,32c#,16c,8g#,32f#,16f,32c#,16d#,32f,f#",
//    (char*)"Back to the Future:d=16,o=5,b=200:4g.,p,4c.,p,2f#.,p,g.,p,a.,p,8g,p,8e,p,8c,p,4f#,p,g.,p,a.,p,8g.,p,8d.,p,8g.,p,8d.6,p,4d.6,p,4c#6,p,b.,p,c#.6,p,2d.6",
//    (char*)"Barbie Girl:d=8,o=5,b=125:g#,e,g#,c#6,4a,4p,f#,d#,f#,b,4g#,f#,e,4p,e,c#,4f#,4c#,4p,f#,e,4g#,4f#",
//    (char*)"Beep Beep:d=4,o=7,b=125:16e,16p,16e",
//    (char*)"Beep Boop:d=4,o=7,b=125:16e,16p,16c",
//    (char*)"Cantina:d=8,o=5,b=250:a,p,d6,p,a,p,d6,p,a,d6,p,a,p,g#,4a,a,g#,a,4g,f#,g,f#,4f.,d.,16p,4p.,a,p,d6,p,a,p,d6,p,a,d6,p,a,p,g#,a,p,g,p,4g.,f#,g,p,c6,4a#,4a,4g",
//    (char*)"DrDre Keep Their Heads Ringin:d=4,o=5,b=100:4a4,4e,2d,8a4,16a4,8a4,8e,8e,2d",
//    (char*)"Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#",
//    (char*)"Ghost Busters:d=4,o=5,b=112:16b,16b,8d#6,8b,8c#6,8a,2p,16b,16b,16b,16b,8a,8b,2p,16b,16b,8d#6,8b,8c#6,8a,2p,16b,16b,16b,16b,8a,8c#6,8b",
//    (char*)"Happy Birthday:d=4,o=5,b=125:16c,32p,32c,32p,8d,32p,8c,32p,8f,32p,e,16p,16c,32p,32c,32p,8d,32p,8c,32p,8g,32p,f,8p,16c,32p,32c,32p,8c6,32p,8a,32p,8f,32p,8e,32p,8d,32p,16a#,32p,32a#,32p,8a,32p,8f,32p,8g,32p,f",
//    (char*)"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#",
//    (char*)"Mission Impossible:d=16,o=5,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,p,a#4,c",
//    (char*)"Rick Roll:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.",
//    (char*)"SMB Theme:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6",
//    (char*)"Star Wars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6",
//    (char*)"Top Gun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#",
//    (char*)"We Wish you a Merry Christmas:d=8,o=5,b=140:4d,4g,g,a,g,f#,4e,4c,4e,4a,a,b,a,g,4f#,4d,4f#,4b,b,c6,b,a,4g,4e,4d,4e,4a,4f#,2g",
//    (char*)"Yellow Submarine:d=4,o=5,b=125:4c#6,4c#6,4c#6,8c#6,8d#6,8g#,8g#,8g#,8g#,2g#,8g#,8g#,4g#,2g#,8f#,8f#,8f#,8f#,2f#"

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
byte SINE[] = {
    127, 134, 142, 150, 158, 166, 173, 181, 188, 195, 201, 207, 213,
    219, 224, 229, 234, 238, 241, 245, 247, 250, 251, 252, 253, 254,
    253, 252, 251, 250, 247, 245, 241, 238, 234, 229, 224, 219, 213,
    207, 201, 195, 188, 181, 173, 166, 158, 150, 142, 134, 127, 119,
    111, 103,  95,  87,  80,  72,  65,  58,  52,  46,  40,  34,  29,
     24,  19,  15,  12,   8,   6,   3,   2,   1,   0,   0,   0,   1,
      2,   3,   6,   8,  12,  15,  19,  24,  29,  34,  40,  46,  52,
     58,  65,  72,  80,  87,  95, 103, 111, 119
 };

IntervalTimer audio_clock;

// Bit shifting volume: 0-6
int VOLUME = 0;
int MUTE = 0;

volatile float t = 0;
volatile float step;

void noTone() {
    audio_clock.end();
    t = 0;
    analogWrite(DAC1, 0);
    analogWrite(DAC2, 0);
}

void tone(int freq) {
    if (freq == 0 || MUTE == 1) {
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

void dingdong() {
    if (playing == true) {
        return;
    }
    VOLUME = 2;
    playing = true;
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_AMP_ENABLE, HIGH);

    // Special days :)
    int month = Time.month();
    int day = Time.day();
    if (month == 1 && day == 20) {
      begin_rtttl(happyBirthday);
    }
    else if (month == 3 && day == 31) {
      begin_rtttl(happyBirthday);
    }
    else if (month == 7 && day == 13) {
      begin_rtttl(happyBirthday);
    }
    else if (month == 11 && day == 18) {
      begin_rtttl(happyBirthday);
    }
    else if (random(5) == 0) {
      begin_rtttl(rickRoll);
    }
    else {
      begin_rtttl(doorbellBuffer);
    }

    mqttPublishDoorbellState("doorbell", false);
}

void beepbeep() {
    if (playing == true) {
        return;
    }
    VOLUME = 0;
    playing = true;
    digitalWrite(PIN_LED, HIGH);
    digitalWrite(PIN_AMP_ENABLE, HIGH);
    begin_rtttl(beepbeepBuffer);
    mqttPublishDoorbellState("beepbeep", false);
}

int particle_dingdong(String command) {
    dingdong();
    return 42;
}

int particle_beepbeep(String command) {
    beepbeep();
    return 69;
}

//----------------
// MQTT
//----------------
void mqttCallback(char* topic, byte* payload, unsigned int length);

char myIpString[24];

byte server[] = { 10, 5, 23, 6 };

// Set max mqtt message size to 1024 bytes
MQTT mqttClient(server, 1883, MQTT_DEFAULT_KEEPALIVE, mqttCallback, 1024);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = '\0';
    String message(p);

    if (message.equals("beepbeep")) {
        beepbeep();
    }
    else if (message.equals("dingdong")) {
        dingdong();
    }
    else if (message.startsWith("dingdong=")) {
        setNewRingtone(p + 9);
    }
}

void setNewRingtone(char* buffer) {
    int len = min(strlen(buffer), sizeof(doorbellBuffer) - 1);
    memset(doorbellBuffer, 0, sizeof(doorbellBuffer));
    memcpy(doorbellBuffer, buffer, len);
    mqttPublishDoorbellRingtone();
}

void setNewBeepbeep(char *buffer) {
    int len = min(strlen(buffer), sizeof(beepbeepBuffer) - 1);
    memset(beepbeepBuffer, 0, sizeof(beepbeepBuffer));
    memcpy(beepbeepBuffer, buffer, len);
}

int mqtt_status = 0;

bool setupMqtt() {
    Particle.variable("mqttstatus", mqtt_status);

    // connect to the server
    if (mqttClient.connect("doorbelljr")) {
        // subscribe
        mqttClient.subscribe("cda/downstairs/hallway/doorbell/device/command");
        mqttPublishDoorbellRingtone();
        return true;
    }
    return false;
}

void loopMqtt() {
    mqtt_status = mqttClient.isConnected() ? 1 : 0;

    if (!mqttClient.loop()) {
        // Not connected, try to reconnect
        if (!setupMqtt()) {
            // Reconnect failed, wait a few seconds
            delay(5000);
        }
    }
}

bool mqttPublishDoorbellButtonPressed(boolean pressed) {
    if (pressed) {
        return mqttClient.publish("cda/outside/frontporch/doorbell/button/state", (uint8_t*)"pressed", 7, false);
    }
    else {
        return mqttClient.publish("cda/outside/frontporch/doorbell/button/state", (uint8_t*)"unpressed", 9, true);
    }
}

bool mqttPublishDoorbellState(const char* state, boolean retain) {
    return mqttClient.publish("cda/downstairs/hallway/doorbell/state", (uint8_t*)state, strlen(state), retain);
}

bool mqttPublishDoorbellRingtone() {
    return mqttClient.publish("cda/downstairs/hallway/doorbell/ringtone", (uint8_t*)doorbellBuffer, strlen(doorbellBuffer), true);
}

//-------------------
// MAIN PROGRAM
//-------------------

uint32_t lastReset = 0;

void setup(void) {
    pinMode(DAC1, OUTPUT);
    pinMode(DAC2, OUTPUT);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_DOORBELL_BUTTON, INPUT);
    pinMode(PIN_AMP_ENABLE, OUTPUT);

    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_AMP_ENABLE, LOW);

    Particle.function("doorbell", particle_dingdong);
    Particle.function("dingdong", particle_dingdong);
    Particle.function("beepbeep", particle_beepbeep);

    lastReset = millis();

    setNewRingtone(doorbellInit);
    setNewBeepbeep(beepbeepInit);

    setupMqtt();
 }

bool pressed = false;

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
            mqttPublishDoorbellState("silent", true);
        }
    }

    else {
        // Read the state of the pushbutton value
        bool state = digitalRead(PIN_DOORBELL_BUTTON) == LOW;

        // Check if the pushbutton changed value
        if (state != pressed) {
            delay(500); // Debounce
            pressed = state;

            // They pressed the button. Start the music
            if (pressed) {
                Particle.publish("doorbell", "ding dong!", 60, PRIVATE);
                dingdong();
            }

            mqttPublishDoorbellButtonPressed(pressed);
        }

        // Reset every 24 hours
        if (millis() - lastReset > 24 * 60 * 60 * 1000UL) {
          System.reset();
        }

        loopMqtt();
    }
}
