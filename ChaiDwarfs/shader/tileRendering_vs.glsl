#version 430

layout(location = 0) in vec2 pos;
layout(location = 1) in unsigned int tileType;

const float off = 0.17f;
/*const vec2 uv[6] = {
  vec2(0.f, 1.f),
  vec2(0.f, 1.f-off),
  vec2(off, 1.f-off),
  vec2(off, 1.f-off),
  vec2(off, 1.f),
  vec2(0.f, 1.f)
};*/

const vec2 uv[6] = {
  vec2(0.f, 0.f),
  vec2(1.f, 0.f),
  vec2(0.f, 1.f),
  vec2(0.f, 1.f),
  vec2(1.f, 0.f),
  vec2(1.f, 1.f)
};

out vec2 ex_uv;
flat out uint ex_tileType;

uniform mat4 mvp;

void main(void){
  gl_Position = mvp * vec4(pos, 0, 1);
  ex_tileType = tileType;
  ex_uv = uv[gl_VertexID%6];
}
