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
	
	if (type == 0) {
		// Default render
		//float s = floor(texCoord.s * 200) / 200;
		//float t = floor(texCoord.t * 200) / 200;
		float b = 0;
		float eggs = dot(normalize(gl_ModelViewMatrix * vec4(normal, 0.0)), -vec4(0.0, -1.0, 0.0, 0.0));
		b += (eggs + 1) / 2;
		//b = exp(1 - length(vec3(worldPos.xyz)) / 40);
	    final = texture2D(texture0, texCoord.st) * vec4(b, b, b, 1.0);
	    //final = vec3(color.xyz) * vec3(b, b, b);
	    if (fog != 0.0) {
		    final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), clamp(1.0 / exp(length(viewPos) * fog), 0.0, 1.0)), final.w);
	    }
    } else if (type == 1) {
    	final = texture2D(texture0, texCoord.st) * color;
    	//final.w = 0.4;
    } else if (type == 3) {
    	final = vec4(((gl_ModelViewProjectionMatrix * vec4(normal, 1.0)).xyz + 0.5) / 2, 1.0);
    }
    
    if (type != 30) {
    	gl_FragData[0] = final;
    } else {
    	//vec2 coord = texCoord.xy;
		//coord.s = mod(coord.s + 0.2, 1.0);
    	gl_FragData[0] = texture2D(texture0, texCoord.xy) * color;
    	gl_FragData[1] = vec4((normalize(mat3(gl_ModelViewMatrix) * normal) + 1.0) / 2, 1.0);
    	gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
    	gl_FragData[3] = vec4(0.0, 0.0, 0.0, 1.0);
    }
}