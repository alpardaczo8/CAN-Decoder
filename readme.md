# CAN Decoder

A command-line tool that decodes CAN bus log files into human-readable signal values using DBC database files. Built in C++ with CMake.

---

### Table of contents
- [Background](#background)
    - [CAN Bus](#can-bus)
    - [DBC files](#dbc-files)
- [Architecture](#architecture)
- [Prequites](#prequities)
- [Building](#building)
- [How to run](#how-to-run)
- [Output format](#output-format)
- [Roadmap](#roadmap)

---

## Background

### CAN Bus

The Controller Area Network (CAN) bus is a communication protocol widely used in automotive and industrial systems. It allows microcontrollers and devices — such as the engine control unit, transmission, brakes, and instrument cluster — to communicate with each other without a central host computer.

Each message on the bus has a **CAN ID** that identifies its source, and a **payload** of up to 8 bytes. Individual values (speed, temperature, torque, etc.) are encoded as **signals** — sequences of bits packed into that payload.

Raw CAN data is typically captured using tools like `candump` on Linux, producing log files in the format:

```
(1700000000.123456) vcan0 000000AA#3D893D893D893D89
  ^-- timestamp        ^-- interface  ^-- CAN ID  ^-- payload (hex)
```

### DBC Files

A DBC (Database CAN) file is the standard format for describing the structure of CAN messages. It defines which CAN IDs carry which signals, where each signal's bits are located in the payload, and how to convert raw bit values into physical units using a scale and offset.

A signal definition looks like:

```
SG_ WheelSpeedFR : 7|16@0+ (0.0062,-67.67) [0|250] "mph" Vector__XXX
                   ^ ^  ^^   ^      ^
                   | |  ||   scale  offset
                   | |  |little/big endian
                   | |  signed/unsigned
                   | bit length
                   start bit
```

The physical value is computed as: `value = raw × scale + offset`

## Architecture

The project is structured as a static library (`CANDecoderLib`) consumed by the `CANDecoder` executable, making the core logic reusable for future frontends (e.g. a QML instrument cluster).

```
CANDecoder/
├── include/can_decoder/
│   ├── DbcParser.hpp       # DBC message/signal structures and parser
│   ├── LogParser.hpp       # candump log parser and decode orchestration
│   ├── SignalDecoder.hpp   # bit extraction and physical value conversion
│   └── Compatibility.hpp  # compiler feature detection (from_chars)
├── src/
│   ├── DbcParser.cpp
│   ├── LogParser.cpp
│   ├── SignalDecoder.cpp
│   └── main.cpp
├── files/
│   ├── sample.dbc
│   └── sample.log
└── CMakeLists.txt
```

**Data flow:**

```
DBC file → DbcParser → in-memory signal database
                               │
candump log → LogParser ───────┤
                               │
                        SignalDecoder → decoded physical values → CSV output
```

**Key components:**

- **`DbcParser`** — parses `BO_` (message) and `SG_` (signal) sections from a DBC file into an `unordered_map<uint32_t, DbcMessage>` keyed by CAN ID
- **`LogParser`** — streams the log file line by line, extracts the CAN ID and payload bytes from each frame, looks up the matching message in the DBC database, and writes decoded values to CSV
- **`SignalDecoder`** — handles both Intel (little-endian, `@1`) and Motorola (big-endian, `@0`) bit extraction, sign extension, and the scale/offset formula

## Prequities

- CMake 3.16 or later
- A C++20-capable compiler:
  - GCC 11+
  - Clang 12+
  - MSVC 2019+

> **Note:** `std::from_chars` for floating-point types requires one of the compiler versions above. On older compilers the project automatically falls back to `std::stod`.

## Building

```bash
git clone https://github.com/alpardaczo8/CAN-Decoder.git
cd CAN-Decoder
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## How to run

```bash
./CANDecoder <dbc_file> <log_file> <output_csv>
```

**Example:**

```bash
./CANDecoder files/sample.dbc files/sample.log files/output.csv
```

## Output format

Decoded values are written to a CSV file with one row per signal per CAN frame:

```
timestamp,message,signal,value,unit
1700000000.000000,KINEMATICS,STEERING_TORQUE,0,
1700000000.000000,WHEEL_SPEEDS,WHEEL_SPEED_FR,30.0012,mph
1700000000.010000,WHEEL_SPEEDS,WHEEL_SPEED_FR,45.0026,mph
```

## Roadmap

- [x] DBC file parser (Messages, signals, offset/scale, endianness)
- [x] candump log file parser
- [x] Intel (little-endian) signal extraction
- [x] Motorola (big-endian) signal extraction
- [x] CSV output
- [ ] QML Instrument cluster frontend
- [ ] QT signals/slots bridge for live gauge updates
- [ ] VAL_ value table support (enum like signals)
- [ ] Multi DBC support