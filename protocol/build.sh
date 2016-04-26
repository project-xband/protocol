#!/bin/bash
# em++ -O2 common.hpp -o common.bc
em++ -O2 interface.cpp -o interface.bc
em++ -O2 protocol.cpp -o protocol.bc
em++ -O2 device.cpp -o device.bc
em++ -O2 test.cpp -o test.bc
em++ -O2 protocol.bc device.bc interface.bc test.bc -o project.bc
em++ -O2 project.bc -o project.html
