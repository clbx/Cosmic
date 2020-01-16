#ifndef _PGU_HPP_
#define _PGU_HPP_

#include <GL/gl3w.h>

class PGU
{
public:
  void init();
  void kill();
  void copy(GLubyte* mem);
  void show();
  void userCallBack();
protected:
  float width; 
  float heigth; 
  GLuint vramTexture=0;
  GLuint g_ShaderHandle = 0;
  GLuint g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
  GLuint g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0;
};

#endif
