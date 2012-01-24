#ifndef __GROUND_H_
#define __GROUND_H_

#include <allegro.h>

//#define LINUX

/*---------------------------------------------------------------------------*/

#define   MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define   MIN(x,y)     (((x) < (y)) ? (x) : (y))

/*---------------------------------------------------------------------------*/

const bool  USEFROMFILE  = 1;         // Data Source

const int   XWORLDSIZE   = 513;       // BE EQUAL ! And = image size if USEFROMFILE  = 1
const int   ZWORLDSIZE   = 513;       // BE EQUAL ! And = image size if USEFROMFILE  = 1

const float MAXHIGH      = 255.0;
const float SOFTCOEF     = 0.3;
const int   LISSAGEDEGRE = 8;
const int   HIGHCUT      = 5;

/*---------------------------------------------------------------------------*/

class Ground
{
public:
       float softcoef;
       int   xground;
       int   zground;
       float maxhigh;
       float **map;

       BITMAP *heightmap;

public:
       Ground(int Xground, int Zground, float MaxHigh, float SoftCoef);
       virtual ~Ground();

public:
       void  loadheightmap();

       void  buildground();
       void  buildsubground(int x1, int z1, int x2, int z2);
       void  lissage(int degre);
       void  HighCut(int highcut);

       float xrand(float xl, float xh);
       int   random(int rndmax);
};

#endif

