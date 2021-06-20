// Copyright (c) 2021 Jan Delgado <jdelgado[at]gmx.net>
// https://github.com/jandelgado/carl
//
// Kalibriersketch für das Tastenfeld
// Calibration sketch for the key pad
//
constexpr auto PIN_BUTTONS = A2;

void setup() {
    pinMode(PIN_BUTTONS, INPUT);
    Serial.begin(9600);
}

void loop() {
    Serial.println(analogRead(PIN_BUTTONS));
    delay(50);
}
