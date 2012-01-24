#include "InitPlayfield.h"

const int   InitPlayfield::ScrW = 1280;
const int   InitPlayfield::ScrH = 800;
int         InitPlayfield::GFXOPENARG = 0; // 1 = FULLSCREEN | 0 = WINDOWED;


/*---------------------------------------------------------------------------*/

InitPlayfield::InitPlayfield(int GFXOPENARG, int ScrW, int ScrH)
{
 allegro_init();
 //install_allegro(SYSTEM_AUTODETECT, 0, atexit); // avoid errno undefined

 install_allegro_gl();

 allegro_gl_set(AGL_DOUBLEBUFFER, true);
 allegro_gl_set(AGL_SUGGEST, AGL_DOUBLEBUFFER);

 allegro_gl_clear_settings();
 allegro_gl_set(AGL_Z_DEPTH, 24);
 allegro_gl_set(AGL_COLOR_DEPTH, 32);
 allegro_gl_set(AGL_RENDERMETHOD, 1);

 allegro_gl_set(AGL_FULLSCREEN, GFXOPENARG);
 allegro_gl_set(AGL_SUGGEST, AGL_FULLSCREEN | AGL_Z_DEPTH | AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER |
                             AGL_RENDERMETHOD);

 set_gfx_mode(GFX_OPENGL, ScrW, ScrH, 0, 0);

 install_keyboard();
 install_mouse();
 install_timer();
 srand(time(NULL));

}

InitPlayfield::~InitPlayfield()
{
 poll_mouse();
 poll_keyboard();
 allegro_exit();
}



