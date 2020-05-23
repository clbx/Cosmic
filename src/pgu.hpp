#ifndef _PGU_HPP_
#define _PGU_HPP_

#include <GL/glew.h>

struct Logger;

class PGU
{
public:
  void init(Logger* debugLog);
  void kill();
  void copy(GLubyte* mem);
  void show();
  void swap(char* path,Logger* debugLog);
  void userCallBack();
protected:
  void makeTexture();
  void compileProgram(const char* shader,Logger* debugLog);
  unsigned int colorbits=0;
  unsigned int ybits=7;
  GLfloat width=128;
  GLfloat heigth=128;
  GLuint vramTexture=0;
  GLuint g_VertHandle = 0, g_FragHandle = 0;
  GLuint g_ShaderHandle = 0;
  GLuint g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
  GLuint g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0;
};

#endif
