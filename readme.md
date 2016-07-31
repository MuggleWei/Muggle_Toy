# Muggle Toy

## Introduction

This repo is a toy for myself, setup a simple framework and learn something I feel Interesting. For understand detail of concept and as little as possible third dependencies, I write some basic library(3d math, model load and so on) by myself, but it's experimental products, don't use it in real work. 

## Build

Ensure CMake and Python is installed, I use python to sync resource file when compile. 
For now, this repo only support build in windows.

Windows
 - md build && cd build
 - cmake .. -G "Visual Studio 14 2015 Win64"   # Or whatever generator you want to use cmake --help for a list.
 - start Muggle.sln