#!/bin/bash

cd build &&
cmake -DCMAKE_BUILD_TYPE=Debug ../src &&
make &&
cd metabox &&
./MetaBox