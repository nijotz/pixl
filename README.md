# Intro

## LED Strips
A physical strip of LEDs whose color array will be given to FastLED

## LEDs
A group of one or more LED strip segments that will be considered one long strip

## Inputs
Something that produces a decimal number between 0 and 1.  It can be audio
data, a random number generator, a wave function, whatever.

## Visualizations
Right now, it is a one-dimensional arary of color.  In the future, this could
be two-, or even three-dimensional.

## Animations
This takes a visualization and maps it to LEDs. For example, this could take a
one-dimensional array of color from a visualization and map it to a grid of
LEDs to produce a plane.

## Looper
Controls calling the update/draw functions of all the components.
