#version 120

// A cute vertex shader with many purposes

varying vec3 normal;
varying vec4 texCoord;
varying vec4 viewPos;
varying vec4 worldPos;
varying mat4 cuteMatrix;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 5434);
}

void main(){
	texCoord = gl_MultiTexCoord0;
	//texCoord.x += 0.9;
	normal = gl_Normal;
	viewPos = gl_ModelViewProjectionMatrix * gl_Vertex;
	worldPos = viewPos - (gl_ModelViewProjectionMatrix * vec4(0.0, 0.0, 0.0, 0.0));
    //gl_Position = gl_ProjectionMatrix * (gl_Vertex + vec4(normal * 3, 0.0));
    gl_Position = viewPos;
}