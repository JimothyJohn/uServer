#!/usr/bin/env bash

PIO=$HOME/.platformio/penv/bin/pio

$PIO run -t upload
$PIO run -t uploadfs
$PIO device monitor
