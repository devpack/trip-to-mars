#include "QuadTree.h"

/*---------------------------------------------------------------------------*/

QuadTree::QuadTree()
{
int i;

 // The Ground
 groundQT = new Ground(XWORLDSIZE, ZWORLDSIZE, MAXHIGH, SOFTCOEF);

 // Root size
 sizex = NBLEAFSECTORPERROW;
 sizey = NBLEAFSECTORPERCOL; 

 // Vertices Arrays
 arrayz = new prnVertexArray[sizex*sizey];
 leaves = new LeafSector*[sizex*sizey];

 // Arrays size
 for (i=0; i<sizex*sizey; i++)
 {
  arrayz[i].Varray = new Vect3d [ONESTRIPBUFFERSIZE];
  arrayz[i].Narray = new Vect3d [ONESTRIPBUFFERSIZE];
  arrayz[i].Tarray = new Vect2d [ONESTRIPBUFFERSIZE];
  arrayz[i].Iarray = new GLuint [ONESTRIPBUFFERSIZE]; 
 }

 //Temp buffer array
 worldpointQT = new Vect3d* [XWORLDSIZE];
 for(i=0; i<XWORLDSIZE; i++)
  worldpointQT[i] = new Vect3d [ZWORLDSIZE];  
  
 worldpointnormaleQT = new Vect3d* [XWORLDSIZE];
 for(i=0; i<XWORLDSIZE; i++)
  worldpointnormaleQT[i] = new Vect3d [ZWORLDSIZE];    
  
 worldpointtexQT = new Vect2d* [XWORLDSIZE];
 for(i=0; i<XWORLDSIZE; i++)
  worldpointtexQT[i] = new Vect2d [ZWORLDSIZE]; 
  
 // INIT ARRAYS
 inittemparrays(); 
 initleavesarray(); 

 // Root Bounding Box      
 Vect3d box_min, box_max;
 box_min.x = 0;
 box_min.y = 0;
 box_min.z = 0;
 box_max.x = ZOOMGROUNDQT *(float)(XWORLDSIZE-1);
 box_max.y = HIGHSCALEQT * MAXHIGH;
 box_max.z = ZOOMGROUNDQT *(float)(ZWORLDSIZE-1);
    
 // The Root 
 root = new NodeSector(this, &box_min, &box_max); 
 build(root, 1);
  
 // VBO Check Extension
 VBOsupported = true; 
  if(!allegro_gl_is_extension_supported("GL_ARB_vertex_buffer_object"))
   VBOsupported = false;
    
}

QuadTree::~QuadTree()
{
int i;

 delete groundQT; 

 //Temp buffer array
  for(i=0; i<XWORLDSIZE; i++) 
   delete [] worldpointQT[i];   
  delete [] worldpointQT; 
  
  for(i=0; i<XWORLDSIZE; i++) 
   delete [] worldpointnormaleQT[i];   
  delete [] worldpointnormaleQT; 
  
  for(i=0; i<XWORLDSIZE; i++) 
   delete [] worldpointtexQT[i];   
  delete [] worldpointtexQT;   
  
  
  // delete the Vertices array
  for (i=0; i<sizex*sizey; i++)
  {
   delete [] arrayz[i].Varray;
   delete [] arrayz[i].Narray;
   delete [] arrayz[i].Tarray;
   delete [] arrayz[i].Iarray;
  }
  
  delete [] arrayz;

  delete [] leaves;
  delete root;   
}

/*---------------------------------------------------------------------------*/

void QuadTree::inittemparrays()
{ 
int i, j;

 // Transform map en coord
 for(j=0; j<ZWORLDSIZE; j++) 
  for(i=0; i<XWORLDSIZE; i++) 
  {
   worldpointQT[i][j].x = ZOOMGROUNDQT * float(i);
   worldpointQT[i][j].z = ZOOMGROUNDQT * float(j);    
   worldpointQT[i][j].y = HIGHSCALEQT * groundQT->map[i][j] ;    
  }
  
 // Normales
 for(j=0; j<ZWORLDSIZE-1; j++) 
  for(i=0; i<XWORLDSIZE-1; i++)  
  { 
   cross_product_f(worldpointQT[i][j+1].x-worldpointQT[i][j].x, worldpointQT[i][j+1].y-worldpointQT[i][j].y, worldpointQT[i][j+1].z-worldpointQT[i][j].z, 
                   worldpointQT[i+1][j].x-worldpointQT[i][j].x, worldpointQT[i+1][j].y-worldpointQT[i][j].y, worldpointQT[i+1][j].z-worldpointQT[i][j].z,                 
                   &worldpointnormaleQT[i][j].x, &worldpointnormaleQT[i][j].y, &worldpointnormaleQT[i][j].z);  
   normalize_vector_f(&worldpointnormaleQT[i][j].x, &worldpointnormaleQT[i][j].y, &worldpointnormaleQT[i][j].z);
   
   cross_product_f(worldpointQT[i+1][j].x-worldpointQT[i+1][j+1].x, worldpointQT[i+1][j].y-worldpointQT[i+1][j+1].y, worldpointQT[i+1][j].z-worldpointQT[i+1][j+1].z, 
                   worldpointQT[i][j+1].x-worldpointQT[i+1][j+1].x, worldpointQT[i][j+1].y-worldpointQT[i+1][j+1].y, worldpointQT[i][j+1].z-worldpointQT[i+1][j+1].z, 
                   &worldpointnormaleQT[i+1][j+1].x, &worldpointnormaleQT[i+1][j+1].y, &worldpointnormaleQT[i+1][j+1].z);  
   normalize_vector_f(&worldpointnormaleQT[i+1][j+1].x, &worldpointnormaleQT[i+1][j+1].y, &worldpointnormaleQT[i+1][j+1].z);   
  }  
  
 // Init Tex coord
 for(j=0; j<ZWORLDSIZE; j++) 
  for(i=0; i<XWORLDSIZE; i++) 
  {
   worldpointtexQT[i][j].x =  (float)i / (float)XWORLDSIZE;
   worldpointtexQT[i][j].y =  1 - (float)j / (float)ZWORLDSIZE;    
  }  
  
}

/*---------------------------------------------------------------------------*/

void QuadTree::initleavesarray()
{
int ia, ja, iaoffset, jaoffset, leavenum;
int i,j, kv,kn,kt;
 
 
 for(ja=0; ja<NBLEAFSECTORPERCOL; ja++)  
  for(ia=0; ia<NBLEAFSECTORPERROW; ia++) 
  {
   
   leavenum = ja*sizex + ia; 
   
   kv=0; kn=0; kt=0;
   for(j=0; j<NBVERTEXPERLEAFSECTOR-1; j+=MESHRESOLUTIONQT) 
    for(i=0; i<NBVERTEXPERLEAFSECTOR; i+=MESHRESOLUTIONQT) 
    {
     iaoffset = ia*LEAFSECTORSIZE;
     jaoffset = ja*LEAFSECTORSIZE;
   
     // Vertex
     if(j!=0 && i==0)
      arrayz[leavenum]. Varray[kv++]=worldpointQT[i+iaoffset][j+jaoffset];  
      
     arrayz[leavenum]. Varray[kv++]=worldpointQT[i+iaoffset][j+jaoffset];  
     arrayz[leavenum]. Varray[kv++]=worldpointQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];  
   
     if(j!=NBVERTEXPERLEAFSECTOR-2 && i==NBVERTEXPERLEAFSECTOR-1)
      arrayz[leavenum]. Varray[kv++]=worldpointQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];              
     
     // Normale
     if(j!=0 && i==0)
      arrayz[leavenum]. Narray[kn++]=worldpointnormaleQT[i+iaoffset][j+jaoffset];  
      
     arrayz[leavenum]. Narray[kn++]=worldpointnormaleQT[i+iaoffset][j+jaoffset];  
     arrayz[leavenum]. Narray[kn++]=worldpointnormaleQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];  
   
     if(j!=NBVERTEXPERLEAFSECTOR-2 && i==NBVERTEXPERLEAFSECTOR-1)
      arrayz[leavenum]. Narray[kn++]=worldpointnormaleQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];              
     
     // Strip Tex coord
     if(j!=0 && i==0)
      arrayz[leavenum]. Tarray[kt++]=worldpointtexQT[i+iaoffset][j+jaoffset];  
      
     arrayz[leavenum]. Tarray[kt++]=worldpointtexQT[i+iaoffset][j+jaoffset];  
     arrayz[leavenum]. Tarray[kt++]=worldpointtexQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];  
   
     if(j!=NBVERTEXPERLEAFSECTOR-2 && i==NBVERTEXPERLEAFSECTOR-1)
      arrayz[leavenum]. Tarray[kt++]=worldpointtexQT[i+iaoffset][j+MESHRESOLUTIONQT+jaoffset];   
   
     } // for for i j

   // Indice
   for(i=0; i<ONESTRIPBUFFERSIZERES; i++)    
    arrayz[leavenum]. Iarray[i]=i;
  
  } // for for ia ja
  
}

/*---------------------------------------------------------------------------*/

void QuadTree::build(NodeSector *node, int level)
{
Vect3d box_min, box_max;
  
 // Build Node Untill the Leaves
 if ( (int)pow(2,level) < NBLEAFSECTORPERROW && (int)pow(2,level) < NBLEAFSECTORPERCOL )
 {
  box_min.x = node->box_min.x;
  box_min.y = node->box_min.y;
  box_min.z = (node->box_max.z+node->box_min.z)/2;
  box_max.x = (node->box_max.x+node->box_min.x)/2;
  box_max.y = node->box_max.y;
  box_max.z = node->box_max.z; 
   node->BG = new NodeSector(this, &box_min, &box_max);
   build((NodeSector*)node->BG, level+1);

  box_min.x = (node->box_max.x+node->box_min.x)/2;
  box_min.y = node->box_min.y;
  box_min.z = (node->box_max.z+node->box_min.z)/2;
  box_max.x = node->box_max.x;
  box_max.y = node->box_max.y;
  box_max.z = node->box_max.z;
   node->BD = new NodeSector(this, &box_min, &box_max);
   build((NodeSector*)node->BD, level+1);

  box_min.x = node->box_min.x;
  box_min.y = node->box_min.y;
  box_min.z = node->box_min.z;
  box_max.x = (node->box_max.x+node->box_min.x)/2;
  box_max.y = node->box_max.y;
  box_max.z = (node->box_max.z+node->box_min.z)/2;
   node->HG = new NodeSector(this, &box_min, &box_max);
   build((NodeSector*)node->HG, level+1);
        
  box_min.x = (node->box_max.x+node->box_min.x)/2;
  box_min.y = node->box_min.y;
  box_min.z = node->box_min.z;
  box_max.x = node->box_max.x;
  box_max.y = node->box_max.y;
  box_max.z = (node->box_max.z+node->box_min.z)/2;        
   node->HD = new NodeSector(this, &box_min, &box_max);
   build((NodeSector*)node->HD, level+1);
 
  return;
 }

 // The Leaves
 box_min.x = node->box_min.x;
 box_min.y = node->box_min.y;
 box_min.z = (node->box_max.z+node->box_min.z)/2;
 box_max.x = (node->box_max.x+node->box_min.x)/2;
 box_max.y = node->box_max.y;
 box_max.z = node->box_max.z; 
  node->BG = new LeafSector(this, &box_min, &box_max);
 
 box_min.x = (node->box_max.x+node->box_min.x)/2;
 box_min.y = node->box_min.y;
 box_min.z = (node->box_max.z+node->box_min.z)/2;
 box_max.x = node->box_max.x;
 box_max.y = node->box_max.y;
 box_max.z = node->box_max.z; 
  node->BD = new LeafSector(this, &box_min, &box_max);
 
 box_min.x = node->box_min.x;
 box_min.y = node->box_min.y;
 box_min.z = node->box_min.z;
 box_max.x = (node->box_max.x+node->box_min.x)/2;
 box_max.y = node->box_max.y;
 box_max.z = (node->box_max.z+node->box_min.z)/2; 
  node->HG = new LeafSector(this, &box_min, &box_max);
 
 box_min.x = (node->box_max.x+node->box_min.x)/2;
 box_min.y = node->box_min.y;
 box_min.z = node->box_min.z;
 box_max.x = node->box_max.x;
 box_max.y = node->box_max.y;
 box_max.z = (node->box_max.z+node->box_min.z)/2; 
  node->HD = new LeafSector(this, &box_min, &box_max);
   
}

/*---------------------------------------------------------------------------*/

void QuadTree::addLeaf(LeafSector *leaf)
{
 leaf->num = ((int)leaf->box_min.x)/(LEAFSECTORSIZE*(int)ZOOMGROUNDQT) + (((int)leaf->box_min.z)/(LEAFSECTORSIZE*(int)ZOOMGROUNDQT))*sizex;
 leaves[leaf->num] = leaf;
}
  
/*---------------------------------------------------------------------------*/

LeafSector::LeafSector(QuadTree *root, Vect3d *box_min, Vect3d *box_max)
           :Sector(box_min, box_max)
{
int i;

 this->root = root; 
 root->addLeaf(this);
 array = & root-> arrayz[num];      
 
 // compute the y value of the bbox
 this->box_min.y = MAXHIGH * HIGHSCALEQT;
 this->box_max.y = 0;
    
 for(i=0; i<ONESTRIPBUFFERSIZERES; i++)
 {    
  this->box_min.y = MIN ( array-> Varray[i].y, this->box_min.y ); 
  this->box_max.y = MAX ( array-> Varray[i].y, this->box_max.y ); 
 } 
 
}

LeafSector::~LeafSector()
{     
}

void LeafSector::checkvisible()
{
 if(key[KEY_L])
 {
  visible = ( root->pointinfrustum( & array-> Varray[0] ) || 
              root->pointinfrustum( & array-> Varray[ONESTRIPBUFFERSIZERES-1] ) ||
              root->pointinfrustum( & array-> Varray[2*(NBVERTEXPERLEAFSECTORRES-1)] ) ||
              root->pointinfrustum( & array-> Varray[(ONESTRIPBUFFERSIZERES-1) - 2*(NBVERTEXPERLEAFSECTORRES-1)] )); 
 }
 else
 {
  visible = ( root->cubeinfrustum(this)!=0 );
  if (!visible) return;  
 }
 
} 

void LeafSector::render()
{
 if (visible)
 { 
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY ); 
  glEnableClientState( GL_TEXTURE_COORD_ARRAY ); 
 
  glVertexPointer  ( 3, GL_FLOAT, 0, array-> Varray );
  glNormalPointer  ( GL_FLOAT, 0, array-> Narray );  	
  glTexCoordPointer( 2, GL_FLOAT, 0, array-> Tarray );  

  glDrawElements( GL_TRIANGLE_STRIP, ONESTRIPBUFFERSIZERES, GL_UNSIGNED_INT, array-> Iarray );   

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );  
  glDisableClientState( GL_TEXTURE_COORD_ARRAY ); 
 
 }
      
}

/*---------------------------------------------------------------------------*/

NodeSector::NodeSector(QuadTree *root, Vect3d *box_min, Vect3d *box_max)
           :Sector(box_min, box_max)
{ 
 this->root = root; 
}

NodeSector::~NodeSector()
{
 delete HG;
 delete HD;
 delete BG;
 delete BD;        
}

void NodeSector::checkvisible()
{
 if(key[KEY_L])
  visible = 1;
 else 
  visible = ( root->cubeinfrustum(this)!=0 );
 
 HG->checkvisible();
 HD->checkvisible();
 BG->checkvisible();
 BD->checkvisible();      
}

void NodeSector::render()
{
 if (!visible)
  return;

 HG->render();
 HD->render();
 BG->render();
 BD->render();     
}
  
/*---------------------------------------------------------------------------*/

Sector::Sector(Vect3d *box_min, Vect3d *box_max)
{
 memcpy(&this->box_min, box_min, sizeof(Vect3d));
 memcpy(&this->box_max, box_max, sizeof(Vect3d));
 box_center.x = (box_min->x+box_max->x) / 2.0f;
 box_center.y = (box_min->y+box_max->y) / 2.0f;
 box_center.z = (box_min->z+box_max->z) / 2.0f;
}


Sector::~Sector()
{
}

/*---------------------------------------------------------------------------*/

void QuadTree::renderflyQT()
{
int i;

 getfrustum();
 
 root->checkvisible();
 root->render();
}




