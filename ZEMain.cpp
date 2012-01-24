/*---------------------------------------------------------------------------*/
/*--------------                  3D DEMO                    ----------------*/
/*---------------------------------------------------------------------------*/

#include <allegro.h>
#include <alleggl.h>
#include "Affiche3d.h"
//#include <GL/glu.h>
//#include <GL/glext.h>	
//#include <GL/glaux.h>	

/*-------------------------------- Timer ------------------------------------*/

volatile int Time=0;
void TimerFunc()
{
 Time++;
}
END_OF_FUNCTION(TimerFunc);

/*--------------------------------- FPS -------------------------------------*/

volatile int TimeFps=0;
void FpsTimer()
{
 TimeFps++;
}
END_OF_FUNCTION(FpsTimer);

float TimeStart=0; 
float TimeEnd=0; 
float TimeLasted=200; 
int   NbFrame=0;

/*--------------------------------- MAIN ------------------------------------*/

int main(int argc,char* argv[]) 
{     
 if (argc>1)
  InitPlayfield::GFXOPENARG=!strcmp(argv[1],"fullscreen");
  
 Affiche3D *FW=new Affiche3D(InitPlayfield::GFXOPENARG, InitPlayfield::ScrW, InitPlayfield::ScrH);

 /*-------------------------------- TIMER -----------------------------------*/
 
 // Timer part
 LOCK_VARIABLE(Time);
 LOCK_FUNCTION(TimerFunc); 
 install_int_ex(TimerFunc, BPS_TO_TIMER(60));
 
 LOCK_VARIABLE(TimeFps);
 LOCK_FUNCTION(FpsTimer);
 install_int_ex(FpsTimer, BPS_TO_TIMER(1000));  
 
 /*-------------------------------- LOOP ------------------------------------*/
  
 while(! key[KEY_ESC] && !mouse_b&1)
 {
 
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
 /*-------------------------------- SYNC ------------------------------------*/
 
 while(Time > 0)
 {
  FW->UpdateCameraPos();
  FW->updatemodeskeys(); 

  Time--;   
 }
 
 /*------------------------------- RENDER -----------------------------------*/
 
 FW->updateglmodes();
 FW->followground();
 FW->CameraTransformations();
 FW->renderfly();    

 /*--------------------------------- FPS ------------------------------------*/
 
 if( FW->AfficheInfo )
 { 
  // Fps 
  if (NbFrame++>=20)									
  {
   NbFrame    = 0;		
   TimeEnd    = TimeFps;  	
   TimeLasted = TimeEnd - TimeStart;
   TimeStart  = TimeEnd;		
   sprintf(FW->buffer, "FPS: %i", int(float(20000.0)/TimeLasted));		
  }
  else
   sprintf(FW->buffer, "FPS: %i", int(float(20000.0)/TimeLasted));
  
  FW->myglprint(-1.0, 1.0, 0.0, makecol(255,255,255), FW->buffer); 

  // VBO
  if(FW->VBOSupported && FW->UseVBO)
   sprintf(FW->buffer, "VBO On");		
  else 
    sprintf(FW->buffer, "VBO Off");

  FW->myglprint(-0.75, 1.0, 0.0, makecol(255,255,255), FW->buffer); 
 
  // Position
  sprintf(FW->buffer, "xpos: %i", FW->xposmap);
  FW->myglprint(0.25, 1.0, 0.0, makecol(255,255,255), FW->buffer); 
  sprintf(FW->buffer, "zpos: %i", FW->zposmap);
  FW->myglprint(0.5, 1.0, 0.0, makecol(255,255,255), FW->buffer); 
  sprintf(FW->buffer, "ypos: %i", int(FW->ycamera));
  FW->myglprint(0.75, 1.0, 0.0, makecol(255,255,255), FW->buffer);    
  
  sprintf(FW->buffer, "%i", FW->TheQuadTree->root->HG->visible);
  FW->myglprint(0., 1.0, 0.0, makecol(255,255,255), FW->buffer);            
 }
 
 /*-------------------------------- BLIT IT ---------------------------------*/
                                                                                    
 glFlush();
 allegro_gl_flip(); 
}


 delete FW;
  return 0;
}
END_OF_MAIN();

/*---------------------------------------------------------------------------*/

/*
template<class T> Array2d {
Array2d(int width,int height): iWidth(width),iHeight(height),iArray(new 
T[width*height]) {}
~Array2d() { delete [] iArray; }
T& operator(int x,int y) { return iArray[y*iWidth+x] }
private:
   int iWidth;
   int iHeight;
   T*iArray;
}

que tu utilises comme ca:
Array2D<float> map;
map(0,0)=1.0;
*/
