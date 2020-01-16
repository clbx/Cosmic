#include "pgu.hpp"
#include "imgui.h"

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
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color * (vec4(1,1,1,1)+vec4(-1,-1,-1,0)*texture(Texture, Frag_UV.st));\n"
    "}\n";


void PGU::init()
{
  GLuint g_VertHandle = 0, g_FragHandle = 0;

	glGenTextures(1,&vramTexture);
	glBindTexture(GL_TEXTURE_2D,vramTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,64,64,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(g_VertHandle,1,&vertex_shader,NULL);
	glCompileShader(g_VertHandle);

	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_FragHandle,1,&fragment_shader,NULL);
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
}

void PGU::kill()
{
  glDeleteBuffers(1,&vramTexture);
  glDeleteProgram(g_ShaderHandle);
}

void PGU::copy(GLubyte* mem)
{
	glBindTexture(GL_TEXTURE_2D,vramTexture);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,64,64,GL_RGBA,GL_UNSIGNED_BYTE,mem);
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
	draw_list->AddCallback(_userCallBack,this);
	draw_list->AddImage((void*)(intptr_t)vramTexture,pos,ImVec2(pos.x+size.x,pos.y+size.y));
	draw_list->AddCallback(ImDrawCallback_ResetRenderState,NULL);
}
