# Game #

[![Build Status][Travis Badge]][Travis Link]

## Overview ##

This is my project for CS 585: Introduction to Game Development. This is my
implementation of the assignments given to us.

## Support ##

This project is currently only confirmed to support Ubuntu 15.*.

## Requirements ##

  * CMake 2.8+
  * gcc 4.9.2+

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

Once the project is built a ./bin directory will be create containing the 
binary. To run it from the project root directory simply call **./bin/gdev**.

## See ##

[Standards Document](https://github
.com/invaderjon/gdev/blob/master/docs/standards.pdf)

## References ##

I used the following repo to help me figure out how to set up the GTest 
library and how to integrate it into travis ci:
[Here](https://github.com/bast/gtest-demo/blob/master/.travis.yml])

[Travis Badge]: https://travis-ci.org/invaderjon/gdev.svg?branch=master
[Travis Link]: https://travis-ci.org/invaderjon/gdev