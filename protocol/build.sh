#!/bin/bash
# em++ -O2 common.hpp -o common.bc
CC=emcc
OPT=
$CC $OPT interface.cpp -o interface.bc
$CC $OPT protocol.cpp -o protocol.bc
$CC $OPT device.cpp -o device.bc
$CC $OPT test.cpp -o test.bc
$CC $OPT --bind protocol.bc device.bc interface.bc test.bc -o c.bc
$CC $OPT c.bc -o c.js
