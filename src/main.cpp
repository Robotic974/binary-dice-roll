/**
 * -----------------------------------------------------------------------------
 * @file   main.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Lancé de Dé Binaire à 6 Faces
 * -----------------------------------------------------------------------------
 */
#include <Arduino.h>

// -----------------------------------------------------------------------------
// Constantes globales
// -----------------------------------------------------------------------------

uint8_t const BTN_PIN   = A2;
uint8_t const LED_COUNT = 3;
uint8_t const LED_PIN[] = { 3, 10, 11 };

uint16_t const ROLLING_TIME      = 6000;
uint8_t  const ROLLING_STEPS     = 4;
uint8_t  const ROLLING_PATTERN[] = { 0, 1, 2, 1 };

// -----------------------------------------------------------------------------
// Interface de commande des LEDs
// -----------------------------------------------------------------------------

class Led {

    public:

        void begin(uint8_t const pin) {
            _pin        = pin;
            _brightness = 0;
            pinMode(_pin, OUTPUT);
        }

        void light(uint8_t const brightness) {
            analogWrite(_pin, _brightness = brightness);
        }

        void fadeOut() {
            if (_brightness) {
                analogWrite(_pin, _brightness *= .9f);
            }
        }

    private:
        uint8_t _pin;
        uint8_t _brightness;

};

// -----------------------------------------------------------------------------
// Ensemble des états de l'automate
// -----------------------------------------------------------------------------

enum class State : uint8_t { PLAY, ROLL, RANDOMIZE };

// -----------------------------------------------------------------------------
// Variables globales
// -----------------------------------------------------------------------------

Led   led[LED_COUNT];
State state;

// -----------------------------------------------------------------------------
// Implémentation du comportement de l'automate en fonction de son état
// -----------------------------------------------------------------------------

void play() {

    if (digitalRead(BTN_PIN)) state = State::ROLL;

}

void roll() {

    static uint8_t  roll_index = 0;
    static uint16_t roll_delay = 0; // temporisation courante
    static uint16_t roll_wait  = 0; // temporisation cumulée
    static uint32_t last_fade  = millis();
    static uint32_t last_roll  = millis();

    uint32_t const now = millis();

    if (now - last_fade >= 10) {
        last_fade = now;
        for (uint8_t i = 0; i < LED_COUNT; ++i) {
            led[i].fadeOut();
        }
    }

    if (now - last_roll < roll_delay) return;
    last_roll = now;

    led[ROLLING_PATTERN[roll_index]].light(0xff);

    roll_index++;
    if (roll_index == ROLLING_STEPS) roll_index = 0;

    roll_wait  += roll_delay;
    if (roll_wait < ROLLING_TIME) {
        roll_delay += 10;
        return;
    }

    roll_wait = roll_delay = 0;
    state = State::RANDOMIZE;

}

void randomize() {

    uint8_t const r = 1 + random(6);

    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        led[i].light((r >> i) & 0x1 ? 0xff : 0);
    }

    state = State::PLAY;

}

// -----------------------------------------------------------------------------
// Initialisation
// -----------------------------------------------------------------------------

void setup() {

    // randomise le générateur de nombres aléatoires
    randomSeed(analogRead(A0));

    pinMode(BTN_PIN, INPUT);

    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        led[i].begin(LED_PIN[i]);
    }

    state = State::PLAY;

}

// -----------------------------------------------------------------------------
// Boucle de contrôle principale
// -----------------------------------------------------------------------------

void loop() {

    switch (state) {

        case State::PLAY:      play();      break;
        case State::ROLL:      roll();      break;
        case State::RANDOMIZE: randomize();

    }

}

/* -----------------------------------------------------------------------------
 * MIT License
 * -----------------------------------------------------------------------------
 * Copyright (c) 2023 Stéphane Calderoni (https://github.com/m1cr0lab)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * -----------------------------------------------------------------------------
 */