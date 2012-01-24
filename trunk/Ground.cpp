#include "Ground.h"

/*---------------------------------------------------------------------------*/

Ground::Ground(int Xground, int Zground, float MaxHigh, float SoftCoef)
{
int i,j;

 xground  = Xground;
 zground  = Zground;
 maxhigh  = MaxHigh;
 softcoef = SoftCoef;
 
 map = new float* [xground];
 for(i=0; i<xground; i++)
  map[i] = new float [zground]; 
   
 if( !USEFROMFILE )
 {
  for(i=0; i<xground; i++)
   for(j=0; j<zground; j++)
    map[i][j]=-1.0;

  map[0][0]=xrand(0.0, maxhigh);    // 1er rnd bug
  map[0][0]=xrand(0.0, maxhigh);
  map[xground-1][0]=xrand(0.0, maxhigh);
  map[0][zground-1]=xrand(0.0, maxhigh);
  map[xground-1][zground-1]=xrand(0.0, maxhigh);
 
  map[0][0]=0.;
  map[xground-1][0]=0.;
  map[0][zground-1]=0.;
  map[xground-1][zground-1]=0.; 
 
  buildground(); 
  lissage(LISSAGEDEGRE);
  HighCut(HIGHCUT); 
 }
 else
 {
  loadheightmap();
  lissage(LISSAGEDEGRE);  
 }
} 

/*---------------------------------------------------------------------------*/

Ground::~Ground()
{
  for(int i=0; i<xground; i++) 
   delete [] map[i];
   
  delete [] map; 
}

/*---------------------------------------------------------------------------*/

void  Ground::loadheightmap()
{
int i, j;
 
 set_color_depth(8);      	
 heightmap = load_bitmap("mountainheight513.bmp", NULL);                     
 if ( !heightmap ) { allegro_message("No Highmap Found \n"); exit(1); }
 
  for(j=0; j<zground; j++) 
   for(i=0; i<xground; i++)
    map[i][j]   = float( (unsigned char)getpixel(heightmap, i, j) );
   
 destroy_bitmap(heightmap);  
 
}

/*---------------------------------------------------------------------------*/

void  Ground::buildground()
{
 buildsubground(0, 0, xground-1, zground-1);
}

/*---------------------------------------------------------------------------*/

void  Ground::buildsubground(int x1, int z1, int x2, int z2)
{
int   xm = (x1+x2)/2;
int   zm = (z1+z2)/2;
int   dl = MIN(x2-x1,z2-z1);
int   variance;
float scaledrnd;

variance = random(dl);
variance = 2*variance - dl;

scaledrnd = float(variance) * softcoef;

 if((xm!=x1)||(zm!=z1))
 {
  // altitude new points
  map[x1][zm] = (map[x1][zm]==-1.0) ? ((map[x1][z1]+map[x1][z2])/float(2))+scaledrnd : map[x1][zm];
  map[xm][z1] = (map[xm][z1]==-1.0) ? ((map[x1][z1]+map[x2][z1])/float(2))+scaledrnd : map[xm][z1]; 
  map[xm][z2] = (map[xm][z2]==-1.0) ? ((map[x1][z2]+map[x2][z2])/float(2))+scaledrnd : map[xm][z2];
  map[x2][zm] = (map[x2][zm]==-1.0) ? ((map[x2][z1]+map[x2][z2])/float(2))+scaledrnd : map[x2][zm]; 
  
  map[xm][zm] = (map[xm][zm]==-1.0) ?((map[x1][z1]+map[x2][z1]+map[x1][z2]+map[x2][z2])/float(4))+scaledrnd :
                                       map[xm][zm];                                 

  // hauteur clipping
  map[x1][zm] = (map[x1][zm]<0.0)   ? 0.0   : map[x1][zm];
  map[xm][z1] = (map[xm][z1]<0.0)   ? 0.0   : map[xm][z1];
  map[xm][z2] = (map[xm][z2]<0.0)   ? 0.0   : map[xm][z2];
  map[x2][zm] = (map[x2][zm]<0.0)   ? 0.0   : map[x2][zm];
  map[xm][zm] = (map[xm][zm]<0.0)   ? 0.0   : map[xm][zm];

  map[x1][zm] = (map[x1][zm]>maxhigh) ? maxhigh : map[x1][zm];
  map[xm][z1] = (map[xm][z1]>maxhigh) ? maxhigh : map[xm][z1];
  map[xm][z2] = (map[xm][z2]>maxhigh) ? maxhigh : map[xm][z2];
  map[x2][zm] = (map[x2][zm]>maxhigh) ? maxhigh : map[x2][zm];
  map[xm][zm] = (map[xm][zm]>maxhigh) ? maxhigh : map[xm][zm];

  // rec (!ordre>)
  buildsubground(x1,z1,xm,zm);
  buildsubground(xm,z1,x2,zm);
  buildsubground(x1,zm,xm,z2);
  buildsubground(xm,zm,x2,z2);
 }

}

/*---------------------------------------------------------------------------*/

void Ground::lissage(int degre)
{
int i, j, k;

 for(k=0; k<degre; k++)
  for(i=1; i<xground-1; i++)
   for(j=1; j<zground-1; j++)
   {
	if(map[i][j]!=0)
    {
     map[i][j]=(map[i-1][j-1]+map[i-1][j]+map[i-1][j+1]+map[i][j-1]+
				map[i][j+1]+map[i+1][j-1]+map[i+1][j]+map[i+1][j+1])/8.;
	}
   }
}

/*---------------------------------------------------------------------------*/

void Ground::HighCut(int highcut)
{
int i, j;

  for(i=0; i<xground; i++)
   for(j=0; j<zground; j++)
    map[i][j] = (map[i][j]<(float)highcut) ? 0.0   : map[i][j];

}

/*---------------------------------------------------------------------------*/

float Ground::xrand(float xl, float xh)
{
#if defined(LINUX)
 return (xl + (xh - xl) *  drand48() ); 
#else
 return (xl + (xh - xl) *  rand() / 32767.0 );
#endif
}

int Ground::random(int rndmax)
{
 return ( rand() % (rndmax+1) );
}
