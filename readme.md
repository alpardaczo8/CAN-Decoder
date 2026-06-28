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

### DBC Files

## Architecture

## Prequities

## Building

## How to run

## Output format

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