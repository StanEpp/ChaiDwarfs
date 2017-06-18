#version 430

smooth in vec2 ex_uv;
flat in uint ex_tileType;

uniform sampler2DArray tileAtlas;

out vec4 color;

void main(void){
  color = texture(tileAtlas, vec3(ex_uv, ex_tileType));
}
