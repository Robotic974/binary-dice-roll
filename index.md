---
layout: default
title: Lancé de Dé Binaire à 6 Faces
---

# {{ page.title }}

<span class="github"></span> [Code source][src]{: target="_blank" }
{: .repo }

<div class="robotic974"></div>


## Organisation de l'atelier

- [Objectif de l'atelier](#purpose)
- [Cartes de développement](#dev-boards)
- [Réalisation du circuit](#wiring)
- [Configuration du projet PlatformIO](#pio)
- [Guide d'implémentation](#guide)
- [Solution et code source complet](#source)


<a name="purpose"></a>

## Objectif de l'atelier

L'objectif de cet atelier est de programmer un petit automate chargé de simuler le lancé d'un dé à 6 faces et de révéler le numéro de la face tirée aléatoirement sur un afficheur binaire à 3 LEDs. Chaque tirage est commandé par un bouton poussoir. Pendant la phase de tirage, les LEDs clignotent alternativement avec une progression de va-et-vient selon une fréquence dégressive. On pourra également éteindre chaque LED progressivement en utilisant la modulation de largeur d'impulsion des broches PWM du micro-contrôleur.

<video autoplay loop muted preload="auto" class="media media-60 round-8 shadow">
    <source src="{{ 'assets/videos/demo.mp4' | relative_url }}" type="video/mp4">
</video>

Démonstration du lancé de dé
{: .caption }



<div id="binary-table">
    <table>
        <thead>
            <th>Face</th>
            <th>État des LEDs</th>
        </thead>
        <tbody>
            <tr>
                <td>1</td>
                <td>
                    <span class="tag dark">0</span>
                    <span class="tag dark">0</span>
                    <span class="tag">1</span>
                </td>
            </tr>
            <tr>
                <td>2</td>
                <td>
                    <span class="tag dark">0</span>
                    <span class="tag">1</span>
                    <span class="tag dark">0</span>
                </td>
            </tr>
            <tr>
                <td>3</td>
                <td>
                    <span class="tag dark">0</span>
                    <span class="tag">1</span>
                    <span class="tag">1</span>
                </td>
            </tr>
            <tr>
                <td>4</td>
                <td>
                    <span class="tag">1</span>
                    <span class="tag dark">0</span>
                    <span class="tag dark">0</span>
                </td>
            </tr>
            <tr>
                <td>5</td>
                <td>
                    <span class="tag">1</span>
                    <span class="tag dark">0</span>
                    <span class="tag">1</span>
                </td>
            </tr>
            <tr>
                <td>6</td>
                <td>
                    <span class="tag">1</span>
                    <span class="tag">1</span>
                    <span class="tag dark">0</span>
                </td>
            </tr>
        </tbody>
    </table>
</div>

Affichage binaire des faces du dé
{: .caption }


<a name="dev-boards"></a>

## Cartes de développement

J'utilise ici une carte [Arduino Nano][nano]{: target="_blank"}, mais vous pouvez tout à fait réaliser le même montage avec une carte [Arduino Uno][uno]{: target="_blank"}. Le raccordement des LEDs et des boutons sur les broches de la carte Arduino est exactement le même.

<div class="boards round-8 shadow">
    <div class="board uno"></div>
    <div class="board nano"></div>
</div>

Cartes Arduino Uno (à gauche) et Nano (à droite)
{: .caption }


<a name="wiring"></a>

## Réalisation du circuit

![Câblage du circuit][wiring]{: .media .round-8 .shadow }

Câblage du circuit
{: .caption }

### Brochage de la carte Arduino

Pour pouvoir éteindre les LEDs de manière progressive, nous utiliserons les broches de l'ATmega328P capables de fournir un signal électrique par modulation de largeur d'impulsion (PWM). Pour les repérer, vous pouvez vous reporter au [schéma de la carte Arduino](#){: #show_pinout}. Ces broches sont généralement désignées par une étiquette portant le préfixe <span class="tag">&#126;</span> :

<div id="overlay">
    <img src="{{ 'assets/images/nano-pinout.png' | relative_url }}" />
</div>

- <span class="tag">&#126;D3</span>  Broche de commande de la LED du bit 2<sup>0</sup>
- <span class="tag">&#126;D10</span> Broche de commande de la LED du bit 2<sup>1</sup>
- <span class="tag">&#126;D11</span> Broche de commande de la LED du bit 2<sup>2</sup>
- <span class="tag">A2</span>        Broche de lecture du bouton
{: .pinout }

Chaque LED est connectée en série avec une résistance de 220 Ω pour limiter l'intensité du courant qui la traverse, et le bouton est associé à une résistance de rappel ***pull-down*** de 10 kΩ pour stabiliser la lecture du signal à l'état `LOW` lorsqu'il est relâché.


<a name="pio"></a>

## Configuration du projet PlatformIO

L'organisation du projet PlatformIO est la suivante :

```sh
binary-dice-roll      # le dossier du projet
├── platformio.ini    # le fichier de configuration du projet
└── src               # le dossier des codes sources
    └── main.cpp      # le programme principal
```

Voici les directives à inscrire dans le fichier de configuration `platformio.ini` :

```ini
[env:dice]
platform  = atmelavr
board     = nanoatmega328new  ; pour une carte Arduino Nano récente  [OptiBoot]
; board   = nanoatmega328     ; pour une carte Arduino Nano obsolète [ATmegaBOOT]
; board   = uno               ; pour une carte Arduino Uno
framework = arduino
```


<a name="guide"></a>

## Guide d'implémentation

### Architecture globale du programme

Le programme principal s'articule à la manière d'un automate fini à 3 états :

```cpp
#include <Arduino.h>

/*
 * Définition de l'ensemble des états de l'automate
 */
enum class State : uint8_t {
    PLAY,     // lancé du dé
    ROLL,     // phase de tirage (le dé roule)
    RANDOMIZE // affichage de la face obtenue par tirage aléatoire
};

State state; // état courant de l'automate

void play()      {} // routine à exécuter dans l'état PLAY
void roll()      {} // routine à exécuter dans l'état ROLL
void randomize() {} // routine à exécuter dans l'état RANDOMIZE

/*
 * Initialisation du programme
 */
void setup() {

    state = State::PLAY; // affectation de l'état initial de l'automate

}

/*
 * Boucle de contrôle principale du programme
 */
void loop() {

    switch (state) {

        case State::PLAY:      play();      break;
        case State::ROLL:      roll();      break;
        case State::RANDOMIZE: randomize();

    }

}
```

### Interface de commande des LEDs

Pour simplifier et améliorer la lisibilité du code source, il peut être judicieux de définir une classe `Led` pour implémenter l'interface de commande des LEDs :

```cpp
class Led {

    public:

        /*
         * Initialisation de la LED
         */
        void begin(uint8_t const pin);

        /*
         * Fixe le niveau d'intensité lumineuse de la LED
         */
        void light(uint8_t const brightness);

        /*
         * Affaiblit progressivement le niveau d'intensité lumineuse de la LED
         */
        void fadeOut();

    private:

        uint8_t _pin;        // broche de commande de la LED
        uint8_t _brightness; // intensité lumineuse de la LED [0-255]

};
```

Il vous appartiendra d'implementer chacune de ces méthodes. Dans la mesure où le code correspondant est très concis, vous pouvez intégrer la définition de cette classe directement dans le code source principal `main.cpp`.

Une fois la classe `Led` définie, vous pouvez définir votre afficheur à LEDs ainsi :

```cpp
uint8_t const LED_PIN[] = { 3, 10, 11 };
uint8_t const LED_COUNT = 3;

Led led[LED_COUNT];
```

Puis initialiser l'afficheur dans la fonction `setup()` :

```cpp
void setup() {

    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        led[i].begin(LED_PIN[i]);
    }

    state = State::PLAY;

}
```


<a name="source"></a>

## Solution et code source complet

Lorsque vous aurez terminé cet exercice et obtenu un programme fonctionnel, ou si vous vous arrachez les cheveux sans être parvenu au résultat attendu, vous pourrez aller jeter un coup d'oeil sur le code source de la solution que je vous propose.  :wink:

[Accéder au code source de la solution][src]{: .button target="_blank" }
{: .center }


[src]:    https://github.com/Robotic974/binary-dice-roll
[nano]:   https://docs.arduino.cc/hardware/nano
[uno]:    https://docs.arduino.cc/hardware/uno-rev3
[wiring]: {{ 'assets/images/wiring.jpg' | relative_url }}