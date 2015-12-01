# Game #

[![Build Status][Travis Badge]][Travis Link]

## Overview ##

This is my project for CS 585: Introduction to Game Development.

## Support ##

This project is currently only confirmed to support Ubuntu 15.*.

## Requirements ##

  * CMake 2.8+
  * gcc 4.9.2+
  * SFML 2.3+ (sudo apt-get install libsfml-dev)

## Build Steps ##

1. Clone the repository
2. Navigate to the local repo directory
3. mkdir build
4. cd build
5. Call **cmake ..**
6. Call **make**

To also build the unit tests call **cmake .. -DBUILD_TESTS=ON** instead of
**cmake ..**.

## Usage ##

To run it from the project root directory simply call **./build/gdev**.
To run the tests call **./build/tests**.

## Game ##

Currently all that is available is a simple test that allows you to control a
block. To move the block press the left, up, down, or right arrows.

## See ##

[Standards Document][Standards]

## References ##

I used [this repo](https://github.com/bast/gtest-demo)
to figure out how to set up the GTest library and how to integrate 
it into Travis-CI.

[Standards]: https://goo.gl/BE4d1N
[Travis Badge]: https://travis-ci.org/invaderjon/gdev.svg?branch=simpleSim
[Travis Link]: https://travis-ci.org/invaderjon/gdev
