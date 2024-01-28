/*
 * dinupa3@gmail.com
 *
 */

#ifndef _HODO_CONST__H_
#define _HODO_CONST__H_

#include "TString.h"

#include <vector>

// add hodoscope planes
std::vector<TString> hodo_planes = {"H1B", "H1T", "H1L", "H1R", "H2L", "H2R", "H2B", "H2T", "H3B", "H3T", "H4Y1L", "H4Y1R", "H4Y2L", "H4Y2R", "H4B", "H4T"};
std::vector<int> hodo_IDs = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46};


// tracking planes
std::vector<TString> tracking_planes = {"D0U", "D0Up", "D0X", "D0Xp", "D0V", "D0Vp", "D2V", "D2Vp", "D2X", "D2Xp", "D2U", "D2Up", "D3pVp", "D3pV", "D3pXp", "D3pX", "D3pUp", "D3pU", "D3mVp", "D3mV", "D3mXp", "D3mX", "D3mUp", "D3mU", "P1Y1", "P1Y2", "P1X1", "P1X2", "P2X1", "P2X2", "P2Y1", "P2Y2"};

#endif /*_HODO_CONST__H_*/
