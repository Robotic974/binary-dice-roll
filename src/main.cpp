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
uint8_t const LED_PIN[] = { 3, 10, 11 };
uint8_t const LED_COUNT = 3;

// -----------------------------------------------------------------------------
// Interface de commande des LEDs
// -----------------------------------------------------------------------------

class Led {

    public:

        void begin(uint8_t const pin) {

        }

        void light(uint8_t const brightness) {

        }

        void fadeOut() {

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

}

void roll() {

}

void randomize() {

}

// -----------------------------------------------------------------------------
// Initialisation
// -----------------------------------------------------------------------------

void setup() {

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