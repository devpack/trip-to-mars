#ifndef __AFFICHE3D_H_
#define __AFFICHE3D_H_

#include "InitGLmodes.h"
#include "Ground.h"
#include "Camera.h"
#include "QuadTree.h"

const GLfloat ZOOMGROUND     = 16.;
const GLfloat HIGHSCALE      = 8.;
const GLfloat STANDINGHEIGHT = 20.;

/*---------------------------------------------------------------------------*/

class Affiche3D : public InitGLmodes, public Camera
{
public:
       char    buffer[50];

       // Class Dependencies
       Ground   *TheGround;
       QuadTree *TheQuadTree;

       // Modes
       int     xposmap, zposmap;
       bool    UseVBO, FollowGround, AfficheInfo, AfficheMode, Fog, Shade, UseTex, light;
       bool    VBOSupported;

public:
       Affiche3D(int GFXOPENARG, int ScrW, int ScrH);
       virtual ~Affiche3D();

public:
       void  followground();
       void  renderfly();
       void  updatemodeskeys();
       void  updateglmodes();
};

#endif
