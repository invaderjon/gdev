// game.m.cpp
#include "build.h"
#include <iostream>

int main( int argc, char *argv[] )
{
    using namespace std;

    int i;

    // print out debug information
    cout << "CS 585 Intro to Game Development" << endl;
    cout << "Game" << endl;
    cout << "Version: " << GAME_VERSION << endl;

    cout << "Arguments: ";
    for ( i = 1; i < argc; ++i )
    {
        cout << argv[i] << " ";
    }
    cout << endl;

    cout << "Finished. Exiting..." << endl;

    return 0;
}