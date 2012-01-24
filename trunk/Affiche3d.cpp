#include "Affiche3d.h"  
  	
/*---------------------------------------------------------------------------*/

Affiche3D::Affiche3D(int GFXOPENARG, int ScrW, int ScrH)
          :InitGLmodes(GFXOPENARG, ScrW, ScrH)
{ 
 // Class Dependencies
 TheGround   = new Ground(XWORLDSIZE, ZWORLDSIZE, MAXHIGH, SOFTCOEF);
 TheQuadTree = new QuadTree();

 // GL Modes
 AfficheInfo=1; AfficheMode=0; FollowGround=0; Fog=1; UseVBO=0; Shade=0; UseTex=1; light = 1;

 // Check Extension
 VBOSupported = TRUE; 
  if(!allegro_gl_is_extension_supported("GL_ARB_vertex_buffer_object"))
   VBOSupported = FALSE;
}

Affiche3D::~Affiche3D()
{
 delete TheGround;
 delete TheQuadTree;
}
  
/*---------------------------------------------------------------------------*/

void Affiche3D::renderfly()
{   
int i, j;
      
 glPushAttrib ( GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT);         

 /***************************************/
      
 if( AfficheMode )
 {
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D); 	
  glColor4f(0.0, 0.0, 0.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 }
 else
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
    
 /***************************************/  
 
  TheQuadTree->renderflyQT();

  glPopAttrib();
}

/*---------------------------------------------------------------------------*/

void Affiche3D::followground()
{
  xposmap = (int) ((xcamera / ZOOMGROUND) ); 
  zposmap = (int) ((zcamera / ZOOMGROUND) ); 

  if(FollowGround==1)
   if ((xposmap < XWORLDSIZE) && (xposmap >= 0) && (zposmap < ZWORLDSIZE) && (zposmap >= 0))
    ycamera = (int) ((TheGround->map[xposmap][zposmap] * HIGHSCALE) + STANDINGHEIGHT); 
}

/*---------------------------------------------------------------------------*/

void Affiche3D::updatemodeskeys()
{
static int funckeys[] = { 0,0,0,0,0,0,0,0 };
static bool* allbooloptions[] = { &AfficheInfo, &AfficheMode, &FollowGround,
                                  &Fog, &UseVBO, &Shade, &UseTex , &light};

 for(int i=0; i<sizeof(funckeys)/sizeof(int); i++)
 {
  if (key[KEY_F1+i]&&!funckeys[i])
   *(allbooloptions[i])=!*(allbooloptions[i]);
  
  funckeys[i]=key[KEY_F1+i];
 }
}

/*---------------------------------------------------------------------------*/

void Affiche3D::updateglmodes()
{
 if( Fog )
  glEnable(GL_FOG);
 else
  glDisable(GL_FOG);
  
 if( Shade )   
  glShadeModel(GL_FLAT);
 else
  glShadeModel(GL_SMOOTH);    

 if( !light )
  glDisable(GL_LIGHTING);  
 else
  glEnable(GL_LIGHTING);    

 if( UseTex )
  glEnable(GL_TEXTURE_2D); 
}

