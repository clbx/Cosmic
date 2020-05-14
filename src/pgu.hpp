#ifndef _PGU_HPP_
#define _PGU_HPP_

#include <GL/glew.h>
#include "json.hpp"

struct Logger;

class PGU
{
public:
  void init();
  void kill();
  void copy(GLubyte* mem);
  void show();
  void swap(char* path,Logger* debugLog);
  void userCallBack();
protected:
  void makeTexture(unsigned int colorbits,unsigned int ybits);
  void compileProgram(const char* shader);
  unsigned int width=128;
  unsigned int heigth=128;
  GLuint vramTexture=0;
  GLuint g_VertHandle = 0, g_FragHandle = 0;
  GLuint g_ShaderHandle = 0;
  GLuint g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
  GLuint g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0;
};

#endif
