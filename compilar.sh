#!/bin/sh
g++ flword.cxx -o flword $(fltk-config --cxxflags --ldflags)
