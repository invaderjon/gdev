// game_utils.h
#ifndef INCLUDED_GAME_UTILS
#define INCLUDED_GAME_UTILS

namespace StevensDev
{

namespace sgdu
{

struct GameUtils
{
    static float calculateFPS( float elapsed, float previousFPS );
      // Calculates the FPS given the elapsed seconds, previous FPS, and
      // frame number.
};

inline
float GameUtils::calculateFPS( float elapsed, float previousFPS )
{
    float frameFPS = 1.0f / elapsed;
    return ( previousFPS * 0.1f + frameFPS * 0.9f );
}

} // End nspc sgdu

} // End nspc StevensDev

#endif
