# Intro
pixl is a library to make LED mapping and animations more modular and easier to
setup.

# Install
Download one of the release zips and add it to your project using the Arduino
IDE.  The cymatic-triangles example in the `examples/` folder is a good starting
point for starting a new project.

# Concepts
Below are the concepts used in the Library with examples of how to use them.

## LED Strips
A physical strip of LEDs. Right now, it's just a class that holds a color array
that will be given to FastLED for management.  Instatiate with the length of
the strip and give to FastLED in setup().

```cpp
LEDStrip strip = LEDStrip(50);

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 50);
}
```

## LEDs
A group of one or more LED strip segments that will be considered one long
strip by visualizations.  Visualizations expect an LEDs object to be passed in,
so even if no addressing adjustments are needed, a "pass through" LEDs object
will still need to be made.

### Passthrough
If just using one strip, pass in the strip object, the starting LED number
(0-based) and the length.

```cpp
LEDStrip strip = LEDStrip(50);
LEDs leds = LEDs(&strip, 0, 50);

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 50);
}
```

### String together physical strips
The LEDs object can be used to turn multiple physical LED strips into one long
virtual one.  If used in this way, pass in the number of strips, an array of
strip objects, an array of start LED numbers for each strip and an array of
lengths for each strip.  The following example takes 3 15-LED strips on ports
6, 7, and 8 and makes one virtual one with 45.

```cpp
LEDStrip strip1 = LEDStrip(15);
LEDStrip strip2 = LEDStrip(15);
LEDStrip strip3 = LEDStrip(15);

LEDstrip* strips[] = {&strip1, &strip2, &strip3};
int starts[] = {0, 0, 0};
int lengths[] = {15, 15, 15};

LEDs leds = LEDs(3, strips, starts, lengths);

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip1.leds, 15);
  FastLED.addLeds<WS2811, 7, RGB>(strip2.leds, 15);
  FastLED.addLeds<WS2811, 8, RGB>(strip3.leds, 15);
}
```

### Rearrange LEDs in a strip
The LEDs class can also be used to take sections of LEDs within one physical
strip and rearrange them.  The following example rearranges a strip so that it
starts at 0, goes to 14, jumps to 30, goes to 44, jumps to 15, and goes to 29.

```cpp
LEDStrip strip = LEDStrip(45);
LEDstrip* strips[] = {&strip, &strip, &strip};
int starts[] = {0, 30, 15};
int lengths[] = {15, 15, 15};

LEDs leds = LEDs(3, strips, starts, lengths);

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 45);
}
```

### Reverse addressing
It can also reverse the order of the LEDs if needed.  Simply add 'true' as the
last argument:

```cpp
LEDStrip strip = LEDStrip(50);
LEDs leds = LEDs(&strip, 0, 50, true);

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 50);
}
```

## Inputs
Something that produces a decimal number between 0 and 1.  It can be audio
data, a random number generator, a wave function, whatever.  Inputs are given
to visualizations to generat color based on the input.  Here is an example that
creates a RandomInput:

```cpp
LEDStrip strip = LEDStrip(50);
LEDs leds = LEDs(&strip, 0, 50);
Input* input;

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 50);
  input = new RandomInput();
}
```

## Visualizations
Visualizations turn inputs into color.  Right now, all visualizations are
one-dimensional arrays of color.  In the future, this could be two-, or even
three-dimensional.

```cpp
#define NUM_LEDS 50

LEDStrip strip = LEDStrip(NUM_LEDS);
LEDs leds = LEDs(&strip, 0, NUM_LEDS);
Input* input;
Visualization* viz;

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, NUM_LEDS);
  input = new RandomInput();
  viz = new TwinkleVisualization(&input, NUM_LEDS);
}
```

## Animations
This takes a visualization and maps it to LEDs. For example, this could take a
one-dimensional array of color from a visualization and map it to a grid of
LEDs to produce a plane.  Here is an example that takes a one-dimensional
ripple visualization and maps it to a triangle using a TriangleAnimation:

```cpp
#define NUM_LEDS 50

LEDStrip strip = LEDStrip(NUM_LEDS);
LEDs leds = LEDs(&strip, 0, NUM_LEDS);
Input* input;
Visualization* viz;
TriangleAnimation* anim;

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, NUM_LEDS);
  input = new RandomInput();
  viz = new TwinkleVisualization(&input, NUM_LEDS);
  anim = new TriangleAnimation(viz, leds);
  // TriangleAnimation an init() that does the mapping calculations. The
  // argument is what percentage of the visualization is mapped to the triangle.
  anim->init(0.4);
}
```

## Looper
The Looper controls updating and drawing of all the components.  It can be
used to control frames per second as well.

```cpp
#define NUM_LEDS 50

LEDStrip strip = LEDStrip(NUM_LEDS);
LEDs leds = LEDs(&strip, 0, NUM_LEDS);
Input* input;
Visualization* viz;
TriangleAnimation* anim;

void setup() {
  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, NUM_LEDS);
  input = new RandomInput();
  viz = new RippleVisualization(&input, NUM_LEDS);
  anim = new TriangleAnimation(viz, leds);

  // TriangleAnimation an init() that does the mapping calculations. The
  // argument is what percentage of the visualization is mapped to the triangle.
  anim->init(0.4);

  // Add all the components to the looper so they update every frame
  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);
  looper->setUpdatesPerSecond(30);
}

void loop() {
  // Call the looper's loop function to update all the components
  Looper::instance()->loop();
}
```
