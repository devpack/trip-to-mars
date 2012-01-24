#ifndef __INITPLAYFIELD_H_
#define __INITPLAYFIELD_H_

#include <allegro.h>
#include <alleggl.h>
#include "stdio.h"
#include "math.h"

/*---------------------------------------------------------------------------*/

struct Vect3d { GLfloat x; GLfloat y; GLfloat z; };
struct Vect2d { GLfloat x; GLfloat y; };

/*---------------------------------------------------------------------------*/

class InitPlayfield
{
public:
       static const int   ScrW;
       static const int   ScrH;
       static int         GFXOPENARG;
       
public:
       InitPlayfield(int GFXOPENARG, int ScrW, int ScrH);
       virtual ~InitPlayfield();
};



#endif


