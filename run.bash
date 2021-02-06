#!/bin/bash
# DO NOT MODIFY THIS FILE

# This script compiles and runs the webserver.

c++ -o src/hackServer src/main.cpp

src/hackServer localhost 8080
