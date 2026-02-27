#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// 1. Point to the strip object in your main .ino file
extern Adafruit_NeoPixel strip; 

// 2. Create a "C-compatible" function
// The 'extern "C"' tells the compiler: "Make this function readable by actions.c"
extern "C" void update_led_strip_cpp(uint8_t r, uint8_t g, uint8_t b) {
    
    // We are now safely in C++ land!
    uint32_t color = strip.Color(r, g, b);
    
    strip.fill(color);
    strip.show();
}