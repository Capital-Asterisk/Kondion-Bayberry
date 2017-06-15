#version 120

uniform int type;
uniform float fog;
uniform vec4 color;

uniform sampler2D texture0;

varying vec3 normal;
varying vec4 texCoord;
varying vec4 viewPos;
varying vec4 worldPos;
varying mat4 cuteMatrix;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 5434);
}

void main(){
	vec4 final = vec4(0.0, 0.0, 0.0, 1.0);

	gl_FragData[0] = vec4(texCoord.xy, 1.0, 1.0);
	gl_FragData[1] = vec4((normalize(mat3(gl_ModelViewMatrix) * normal) + 1.0) / 2, 1.0);
}

