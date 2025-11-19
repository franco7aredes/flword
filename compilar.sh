#!/bin/sh
g++ tinyword.cxx -o tinyword $(fltk-config --cxxflags --ldflags)
