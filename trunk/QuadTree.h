#ifndef __QUADTREE_H_
#define __QUADTREE_H_

#include "InitPlayfield.h"
#include "Ground.h"

const int LEAFSECTORSIZE           = 16;
const int NBLEAFSECTORPERROW       = (XWORLDSIZE-1) / LEAFSECTORSIZE;
const int NBLEAFSECTORPERCOL       = (ZWORLDSIZE-1) / LEAFSECTORSIZE;

const int NBVERTEXPERLEAFSECTOR    = LEAFSECTORSIZE+1;
const int ONESTRIPBUFFERSIZE       = 2*(NBVERTEXPERLEAFSECTOR)*(NBVERTEXPERLEAFSECTOR-1) + 2*(NBVERTEXPERLEAFSECTOR-2);

const int MESHRESOLUTIONQT         = 2;
const int NBVERTEXPERLEAFSECTORRES = (MESHRESOLUTIONQT==1) ? NBVERTEXPERLEAFSECTOR : (NBVERTEXPERLEAFSECTOR/MESHRESOLUTIONQT)+1;
const int ONESTRIPBUFFERSIZERES    = 2*(NBVERTEXPERLEAFSECTORRES)*(NBVERTEXPERLEAFSECTORRES-1) + 2*(NBVERTEXPERLEAFSECTORRES-2);
// TODO correct buffer size

const GLfloat ZOOMGROUNDQT      = 12.;
const GLfloat HIGHSCALEQT       = 8.;

/*---------------------------------------------------------------------------*/

class QuadTree;
class Sector;
class NodeSector;
class LeafSector;

/*---------------------------------------------------------------------------*/

class prnVertexArray
{
public:
       Vect3d *Varray;
       Vect3d *Narray;
       Vect2d *Tarray;
       GLuint *Iarray;

public:
       prnVertexArray() {}
       virtual ~prnVertexArray() {}
};

/*---------------------------------------------------------------------------*/

class Sector
{
public:
    QuadTree *root;
    bool     visible;

    Vect3d   box_min;
    Vect3d   box_max;
    Vect3d   box_center;

public:
    Sector(Vect3d *box_min, Vect3d *box_max);
    virtual ~Sector();

public:
    virtual void checkvisible() = 0;
    virtual void render()       = 0;
};

/*---------------------------------------------------------------------------*/

class NodeSector : public Sector
{
public:
       Sector *HG, *HD, *BG, *BD;

public:
       NodeSector(QuadTree *root, Vect3d *box_min, Vect3d *box_max);
       virtual ~NodeSector();

public:
       virtual void checkvisible();
       virtual void render();
};

/*---------------------------------------------------------------------------*/

class LeafSector : public Sector
{
public:
       prnVertexArray *array;
       int num;

public:
       LeafSector(QuadTree *root, Vect3d *box_min, Vect3d *box_max);
       virtual ~LeafSector();

public:
       virtual void checkvisible();
       virtual void render();
};

/*---------------------------------------------------------------------------*/

class QuadTree
{
public:
       Ground     *groundQT;

       LeafSector **leaves;                    // Quick access to the leaves
       prnVertexArray *arrayz;                 // Vertex (+etc) arrays
       GLfloat    frustum[6][4];

       // Temp buffer array
       Vect3d     **worldpointQT;
       Vect3d     **worldpointnormaleQT;
       Vect2d     **worldpointtexQT;

       bool       VBOsupported;

public:
       int        sizex;
       int        sizey;
       NodeSector *root;

public:
       QuadTree();
       virtual ~QuadTree();

public:
       void build(NodeSector *node, int level);
       void addLeaf(LeafSector *leaf);

       void initleavesarray();
       void inittemparrays();

       void getfrustum();
       bool pointinfrustum(Vect3d *point);
       bool cubeinfrustum(Sector *sector);

       void renderflyQT();
};


#endif
