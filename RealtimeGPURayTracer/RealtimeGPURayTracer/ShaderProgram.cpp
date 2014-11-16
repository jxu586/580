#include <cstring>
#include <cstdio>
#include <iostream>

#include "ShaderProgram.h"
#include "utility.h"
#include "error.h"

using namespace std;

CShaderProgram::CShaderProgram()
{
  shaderProgramHandle = 0; 
}

CShaderProgram::~CShaderProgram()
{
  glDeleteProgram(shaderProgramHandle);
}

int CShaderProgram::BuildShadersFromFiles(const char filenames[5][MAX_PATH])
{
  char *pCodes[5] = { NULL, NULL, NULL, NULL, NULL };
  const char *pCodesConst[5];
  for (int i = 0; i < 5; i++) {
    if (filenames[i] == NULL || filenames[i][0] == '\0') {
      pCodes[i] = NULL;
      continue;
    }

    pCodes[i] = new char[128 * 1024];

    if (ReadShader(filenames[i], pCodes[i], 128 * 1024) < 0) {
      cout << "shader " << filenames[i] << " file no found" << endl;
      return GET_ERROR_RETURN(ET_SHADER_FILE_NO_FOUND);
    }
  }

  for (int i = 0; i < 5; i++)
    pCodesConst[i] = pCodes[i];

  int r = BuildShadersFromStrings(pCodesConst);
  for (int i = 0; i < 5; i++) {
    SAFE_DELETE_ARRAY(pCodes[i]);
  }

  return r;
}

int CShaderProgram::BuildShadersFromStrings(const char **pCodes)
{
  shaderProgramHandle = glCreateProgram();
  if (shaderProgramHandle == 0) {
    cout << "shader initialize failed" << endl;
    return GET_ERROR_RETURN(ET_SHADER_CREATION_FAILURE);
  }

  GLuint hShaders[5] = { 0, 0, 0, 0, 0 };
  GLenum ShaderFlags[5] = { 
    GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER,
    GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER
  };

  char shaderName[5][128] = {
    "vertex shader", "fragment shader", "geometry_shader",
    "tess control shader", "tess evaluation shader"
  };


  for (int i = 0; i < 5; i++) {
    if (pCodes[i] == NULL)
      continue; 

    cout << "compiling " << shaderName[i] << "..." << endl;

    if (CompileShader(pCodes[i], ShaderFlags[i], hShaders[i]) < 0) {
      cout << "shader " << shaderName[i] << " compile error" << endl;
      return GET_ERROR_RETURN(ET_SHADER_COMPILE_ERROR);
    } else glAttachShader(shaderProgramHandle, hShaders[i]);
  }

  glLinkProgram(shaderProgramHandle);
  //
  int Status;
  glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &Status);
  if (Status == 0) 
  {
    GLchar Logs[512];
    glGetProgramInfoLog(shaderProgramHandle, 512, NULL, Logs);
    cout << "Errors:\n" << Logs << endl;
    return GET_ERROR_RETURN(ET_SHADER_LINK_ERROR);
  }

  for (int i = 0; i < 5; i++) 
  {
    glDeleteShader(hShaders[i]);
  }

  if (AssignVariable() < 0) 
  {
    return GET_ERROR_RETURN(ET_SHADER_VARIABLE_BINDING_ERROR);
  }

  return 0;
}

void CShaderProgram::BindShader()
{
  glUseProgram(shaderProgramHandle);
}

int CShaderProgram::ReadShader(const char *filename, char *code, int len) 
{
  char filePath[MAX_PATH];

  strcpy(filePath, ShaderBasePath);
  strcat(filePath, filename);

  FILE *pfile = fopen(filePath, "rb");
  if (pfile == NULL) 
  {
    return GET_ERROR_RETURN(ET_SHADER_FILE_NO_FOUND);
  }

  code[fread(code, 1, len, pfile)] = '\0';
  fclose(pfile);

  return 0;
}

int CShaderProgram::CompileShader(const char *pCode, GLenum ShaderType, GLuint &hShader)
{
  hShader = glCreateShader(ShaderType);

  if (hShader == 0) {
    cout << "shader creation failed" << endl;
    return GET_ERROR_RETURN(ET_SHADER_CREATION_FAILURE);
  }

  const GLchar *pCodes[] = { pCode };
  GLint codeLength[] = { (GLint)strlen(pCode)};

  glShaderSource(hShader, 1, pCodes, codeLength);
  glCompileShader(hShader);

  GLint status;
  glGetShaderiv(hShader, GL_COMPILE_STATUS, &status);
  if (status == 0) 
  {
    GLchar Info[512];
    glGetShaderInfoLog(hShader, 512, NULL, Info);
    cout << Info << endl;
    return -1;
  }

  return 0;
}

GLint CShaderProgram::GetVariableHandler(const char *pName, GLuint hsp)
{
  GLint handler = glGetUniformLocation(hsp, pName);

  if (handler == -1)
  {
    cout << "variable no found \'" << pName << '\'' << endl;
  }

  return handler;
}

int CShaderProgram::ScreenWidth = 0;
int CShaderProgram::ScreenHeight = 0;
char CShaderProgram::ShaderBasePath[MAX_PATH] = "";






