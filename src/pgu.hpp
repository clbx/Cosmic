#ifndef _PGU_HPP_
#define _PGU_HPP_

#include <GL/gl3w.h>
#include "json.hpp"

class PGU
{
public:
  void init();
  void kill();
  void copy(GLubyte* mem);
  void show();
  void menu();
  void userCallBack();
protected:
  void makeTexture(unsigned int colorbits,unsigned int ybits);
  float width=128;
  float heigth=128;
  GLuint vramTexture=0;
  GLuint g_ShaderHandle = 0;
  GLuint g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
  GLuint g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0;
  char** moduls_data;
  json_value** moduls_json;
  const char** moduls_name;
  unsigned int nmoduls;
};

#endif
