# ESP32 Traffic Light Controller with Pedestrian Crossing

A traffic light and pedestrian crosswalk built on an ESP32 as a **finite state machine**. It drives 5 LEDs and a seven-segment countdown display with only 3 GPIO pins, using two daisy-chained **74HC595 shift registers**.

<img width="768" height="1024" alt="FB9F5871-F0C2-4E36-8C58-8229CCC93903_1_105_c" src="https://github.com/user-attachments/assets/a00b8a7a-c90e-45cd-b19e-977a03750081" />

## How it works

The light cycles through four states:

| State | Traffic light | Pedestrian signal | Duration |
|---|---|---|---|
| `GREEN` | Green | STOP | 10 s (listens for the button) |
| `YELLOW` | Yellow | STOP | 3 s |
| `RED` | Red | STOP | 20 s |
| `REDGO` | Red | WALK | 20 s, then a 9–0 countdown |

- During green, the program polls the pedestrian button using `millis()`, so a press anywhere in that 10-second window is registered.
- If the button was pressed, the light goes to `REDGO` instead of `RED` and the WALK light turns on.
- After the walk phase, a **countdown** runs from 9 to 0 on the seven-segment display. The STOP light and the display blink together every half second, like a real "don't walk" warning.

## Hardware

- ESP32 development board
- 2 × 74HC595 shift registers (daisy-chained)
- LEDs: green, yellow, red, WALK, STOP (with current-limiting resistors)
- Common-cathode seven-segment display
- Push button

| ESP32 pin | Connects to |
|---|---|
| GPIO 21 | 74HC595 data (DS) |
| GPIO 18 | 74HC595 clock (SH_CP) |
| GPIO 19 | 74HC595 latch (ST_CP) |
| GPIO 4 | Pedestrian button (internal pull-up, active low) |

## Design details

**Shift registers.** Each update sends two bytes with `shiftOut()`, MSB first, while the latch is low. The first byte ends up in the second register (the seven-segment display) and the second byte in the first register (the lights). Raising the latch updates all 16 outputs at once.

**Bitmapped light states.** Each state is one byte in a lookup table, with the bit layout `X X X G Y R WALK STOP`:

```cpp
B00010001, // GREEN:  green on, STOP on
B00001001, // YELLOW: yellow on, STOP on
B00000101, // RED:    red on, STOP on
B00000110  // REDGO:  red on, WALK on
```

The seven-segment digits use a second lookup table in `DP G F E D C B A` order, so displaying a number is a single array lookup.

**State machine.** An `enum State` tracks the current state, and `changeLights()` maps each state to its byte, which keeps the light logic separate from the hardware output.

## Possible improvements

- Replace the blocking `delay()` calls with a fully non-blocking, `millis()`-based state machine, so the button can be read in every state.
- Debounce the button in software.
- Make the walk time scale with the countdown length.

## Built with

C++ · Arduino IDE · ESP32
