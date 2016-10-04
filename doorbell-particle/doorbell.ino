// 
// Check current doorbell state
// curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/[deviceid]/analogvalue
// curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/[deviceid]/range
//
// Show all open/close events:
// curl -H "Authorization: Bearer [token]" https://api.spark.io/v1/devices/events


// Pin that we'll blink when we detect doorbell
int led = D7;

// The pin that the current sensor is plugged into
int doorbell = A0;

// Storage of the current value
int analogvalue;
int analogmin;
int analogmax;
char analogrange[16];

int smoothed;
int smoothedmin;
int smoothedmax;
int samplesize;
char smoothedrange[16];

void setup() {

    pinMode(led,OUTPUT);

    pinMode(doorbell, INPUT);
    digitalWrite(led, LOW);

    Spark.variable("analog", &analogvalue, INT);
    Spark.variable("amin", &analogmin, INT);
    Spark.variable("amax", &analogmax, INT);
    Spark.variable("arange", analogrange, STRING);

    Spark.variable("smoothed", &smoothed, INT);
    Spark.variable("smin", &smoothedmin, INT);
    Spark.variable("smax", &smoothedmax, INT);
    Spark.variable("srange", smoothedrange, STRING);

    delay(3000);
    analogvalue = analogRead(doorbell);
    analogmin = analogvalue;
    analogmax = analogvalue;
    smoothed = analogvalue;
    smoothedmin = smoothed;
    smoothedmax = smoothed;
    samplesize = 1;
}

void loop() {
    analogvalue = analogRead(doorbell);
    if (analogvalue < analogmin) {
        analogmin = analogvalue;
    }
    if (analogvalue > analogmax) {
        analogmax = analogvalue;
    }
    sprintf(analogrange, "[%d,%d]", analogmin, analogmax);
    
    if (samplesize < 10) {
        samplesize++;
        if (samplesize == 10) {
            smoothedmin = smoothed;
            smoothedmax = smoothed;
        }
    }
    smoothed = (smoothed * (samplesize - 1) + analogvalue) / samplesize;
    if (smoothed > smoothedmax) {
        smoothedmax = smoothed;
    }
    if (smoothed < smoothedmin) {
        smoothedmin = smoothed;
    }
    sprintf(smoothedrange, "[%d,%d]", smoothedmin, smoothedmax);

    // Let's take samples every 10ms = 100x per sec
    delay(10);
}

