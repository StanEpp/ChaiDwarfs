/*
*  Copyright(c) 2016 - 2017 Stanislaw Eppinger
*  Scripting based game called ChaiDwarfs
*
*  This file is part of ChaiDwarfs.
*
*  ChaiDwarfs is free software : you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>
*/

#pragma once

#include <map>
#include <string>

#ifndef _GL3W_
#define _GL3W_
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#endif

namespace cdwarfs::render
{

class ShaderBaseModel
{
  friend class ShaderManager;

public:

  // Returns shader ID with the associated Key. Returns 0 if Key does not exist.
  GLuint getShaderID(const std::string& ShaderKey)
  {
    auto it = _shader.find(ShaderKey);
    if (it == _shader.end()) return 0;
    return it->second;
  }

  // Returns shader ID with the associated Key. Returns 0 if Key does not exist.
  GLuint getShaderProgramID(const std::string& ShaderProgramKey)
  {
    auto it = _shaderProgram.find(ShaderProgramKey);
    if (it == _shaderProgram.end()) return 0;
    return it->second;
  }

  // Returns false if element with the same key already exists, otherwise returns true.
  bool saveShader(const std::string& ShaderKey, GLuint ShaderID)
  {
    return _shader.insert(std::make_pair(ShaderKey, ShaderID)).second;
  }

  // Returns false if element with the same key already exists, otherwise returns true.
  bool saveShaderProgram(const std::string& ShaderProgramKey, GLuint ShaderProgramID)
  {
    return _shaderProgram.insert(std::make_pair(ShaderProgramKey, ShaderProgramID)).second;
  }

  void deleteShader(const std::string& ShaderKey)
  {
    _shader.erase(ShaderKey);
  }

  void deleteShaderProgram(const std::string& ShaderProgramKey)
  {
    _shaderProgram.erase(ShaderProgramKey);
  }

private:
  std::map<std::string, GLuint> _shader;
  std::map<std::string, GLuint> _shaderProgram;

  //May only be instantiated in ShaderManager
  ShaderBaseModel() {}
  ShaderBaseModel(ShaderBaseModel&) = delete;
  ShaderBaseModel(ShaderBaseModel&&) = delete;
  ShaderBaseModel& operator=(ShaderBaseModel&) = delete;
  ShaderBaseModel& operator=(ShaderBaseModel&&) = delete;
};

}
