#!/bin/bash

cd build &&
cmake -DCMAKE_BUILD_TYPE=Debug ../src &&
make &&
./frame_test/FrameTest