#include "pgu.hpp"
#include "imgui.h"
#include <cmath>
#include <cstdio>
#include <dirent.h>

const GLchar* vertex_shader =
    "#version 150\n"
    "uniform mat4 ProjMtx;\n"
    "in vec2 Position;\n"
    "in vec2 UV;\n"
    "in vec4 Color;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_UV = UV;\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

const GLchar* fragment_shader =
    "#version 150\n"
    "uniform usampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "vec4 render(vec2 pos);\n"
    "void main()\n"
    "{\n"
		"  Out_Color=Frag_Color;\n"
    "  Out_Color=render(Frag_UV);\n"
    "}\n"
    "uvec4 readRAM(ivec2 pos)\n"
    "{\n"
     "  return texelFetch(Texture,pos,0);\n"
    "}\n";

const GLchar* pixel_shader =
    "vec4 render(vec2 pos)\n"
    "{\n"
"if(any(lessThan(pos,vec2(0,0)))||any(greaterThanEqual(pos,vec2(128,128))))return vec4(0.2,0.1,0.0,1.0);\n"
    "    else return vec4(0.0,float(readRAM(ivec2(pos)).x)/255.,0.0,1.0);\n"
    "}\n";

void PGU::makeTexture(unsigned int colorbits,unsigned int ybits)
{
	glGenTextures(1,&vramTexture);
	glBindTexture(GL_TEXTURE_2D,vramTexture);
	switch(colorbits)
	{
	case 2:
	  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8UI,1<<(12-ybits),1<<ybits,0,GL_RGBA_INTEGER,GL_UNSIGNED_BYTE,0);
	  break;
	case 1:
	  glTexImage2D(GL_TEXTURE_2D,0,GL_RG8UI,1<<(13-ybits),1<<ybits,0,GL_RG,GL_UNSIGNED_BYTE,0);
	  break;
	default:
	  glTexImage2D(GL_TEXTURE_2D,0,GL_R8UI,1<<(14-ybits),1<<ybits,0,GL_RED_INTEGER,GL_UNSIGNED_BYTE,0);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

void PGU::init()
{
  nmoduls=0;
  GLuint g_VertHandle = 0, g_FragHandle = 0;

  makeTexture(0,7);

	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(g_VertHandle,1,&vertex_shader,NULL);
	glCompileShader(g_VertHandle);

	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* shaders[2]={fragment_shader,pixel_shader};
	glShaderSource(g_FragHandle,2,shaders,NULL);
	glCompileShader(g_FragHandle);

	g_ShaderHandle = glCreateProgram();
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	glDeleteShader(g_VertHandle);
	glDeleteShader(g_FragHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationVtxPos = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationVtxUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationVtxColor = glGetAttribLocation(g_ShaderHandle, "Color");

  width=128;
  heigth=128;

  DIR *dir;
  struct dirent *ent;
  if((dir = opendir ("pgu")) != NULL)
  {
    while((ent = readdir (dir)) != NULL)
    {
      if(ent->d_type==DT_REG)nmoduls++;
    }
    rewinddir(dir);
    moduls_data = new char*[nmoduls];
    moduls_json = new json_value*[nmoduls];
    moduls_name = new const char*[nmoduls];
    unsigned int i=0;
    char path[512]="pgu/";
    while((ent = readdir (dir)) != NULL)
    {
      if(ent->d_type==DT_REG)
      {
        strcpy(path+4,ent->d_name);
        FILE *f = fopen(path, "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        moduls_data[i] = new char[fsize];
        fread(moduls_data[i], 1, fsize, f);
        fclose(f);
        json_value* json=moduls_json[i]=json_parse(moduls_data[i],fsize);
        if(json)
        {
          moduls_name[i]="[unnamed]";
          for(unsigned int k=0;k<json->u.object.length;k++)
          {
            if(!strcmp(json->u.object.values[k].name,"name"))moduls_name[i]=json->u.object.values[k].value->u.string.ptr;
          }         
        }else moduls_name[i]="[defunct]";
        i++;
      }
    }
    closedir(dir);
  }else{
    perror("");
  }
}

void PGU::kill()
{
  glDeleteBuffers(1,&vramTexture);
  glDeleteProgram(g_ShaderHandle);
  while(nmoduls--)
  {
    delete moduls_data[nmoduls];
    json_value_free(moduls_json[nmoduls]);
  }
  delete moduls_data;
  delete moduls_json;
  delete moduls_name;
}

void PGU::copy(GLubyte* mem)
{
	glBindTexture(GL_TEXTURE_2D,vramTexture);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,128,128,GL_RED_INTEGER,GL_UNSIGNED_BYTE,mem);
}

void PGU::userCallBack()
{
ImDrawData* draw_data=ImGui::GetDrawData();
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho_projection[4][4] =
    {
        { 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
        { 0.0f,         0.0f,        -1.0f,   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

    glEnableVertexAttribArray(g_AttribLocationVtxPos);
    glEnableVertexAttribArray(g_AttribLocationVtxUV);
    glEnableVertexAttribArray(g_AttribLocationVtxColor);
    glVertexAttribPointer(g_AttribLocationVtxPos,   2, GL_FLOAT,         GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationVtxUV,    2, GL_FLOAT,         GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

void _userCallBack(const ImDrawList* parent_list, const ImDrawCmd* cmd){
	((PGU*)(cmd->UserCallbackData))->userCallBack();
}


void PGU::show()
{
  ImDrawList *draw_list=ImGui::GetWindowDrawList();
  ImVec2 pos=ImGui::GetCursorScreenPos();
  ImVec2 size=ImGui::GetContentRegionAvail();
  float scale=fmax(fmin(floor((size.x-24.)/width),floor((size.y-24.)/heigth)),1.);
  size.x=floor(size.x/scale);
  size.y=floor(size.y/scale);
  float ox=(size.x-width)/2;
  float oy=(size.y-heigth)/2;
	draw_list->AddCallback(_userCallBack,this);
	draw_list->AddImage((void*)(intptr_t)vramTexture,pos,ImVec2(pos.x+size.x*scale,pos.y+size.y*scale),ImVec2(-ox,-oy),ImVec2(size.x-ox-1,size.y-oy-1));
	draw_list->AddCallback(ImDrawCallback_ResetRenderState,NULL);
}

void PGU::menu()
{
  if (ImGui::BeginMenu("PGU"))
  {
    if (ImGui::MenuItem("remove"))
    {
    }
    if (ImGui::BeginMenu("insert"))
    {
      for(unsigned int i=0;i<nmoduls;i++)
      {
        if (ImGui::MenuItem(moduls_name[i]))
        {
          
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
}
