#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "InitPlayField.h"

const GLdouble PI             = 3.1415926535897932384626433832795;
const GLdouble Deg2Rad        = 0.0174532925199432957692369076848861;

const GLfloat  SPEEDWALK      = 1.5;
const GLfloat  SPEEDROT       = 1.5;
const GLfloat  CAMERADISTANCE = 0.0;    // en z
const GLfloat  CAMERAHEIGHT   = 600.0;    // en y
const GLfloat  ZNEAR          = 1.0;
const GLfloat  ZFAR           = 5000.0;
const GLfloat  ANGLEDEVUE     = 48.0;

/*---------------------------------------------------------------------------*/

class Camera
{
public:   
        GLfloat xcamera, ycamera, zcamera;     
        GLfloat yrotcamera, xrotcamera, zrotcamera;         
        GLfloat speedwalk, speedrot;
        GLfloat Znear, Zfar;
        GLfloat angledevue;

public:
        Camera();
        ~Camera() {}

public:

        void Init3dView();
        void Walk(GLfloat speed);   
        void Yrotcamera(GLfloat speed);
        void Xrotcamera(GLfloat speed);
        void Zrotcamera(GLfloat speed);     
        void CameraTransformations();     
        void UpdateCameraPos();
        
        void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
   
};

#endif
