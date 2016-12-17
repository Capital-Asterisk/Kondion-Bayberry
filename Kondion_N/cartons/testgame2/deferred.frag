#version 120

uniform int type;
uniform float fog;
uniform vec4 skyColor;

uniform sampler2D texture0; // Diffuse
uniform sampler2D texture1; // Depth
uniform sampler2D texture2; // Normals
uniform sampler2D texture3; // Brightness

varying vec3 normal;
varying vec4 texCoord;
varying vec4 viewPos;
varying vec4 worldPos;
varying mat4 cuteMatrix;

void main(){
	vec4 final = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 coord = texCoord.xy;
	//coord.x = mod((coord.x - 0.5) / 2 + 0.5 - sin(((1 - coord.x) - 0.5) * 3) / 4, 1.0);
	//coord.y = mod((coord.y - 0.5) / 2 + 0.5 - sin(((1 - coord.y) - 0.5) * 3) / 4, 1.0);
	
	//coord.x += 0.01;
	//coord.x = mod(coord.x, 1.0);
	//coord.x += 0.5;
	//final = mix(texture2D(texture0, texCoord.st), vec4(0.0, 0.0, 0.0, 1.0), clamp(vec4(pow(texture2D(texture1, texCoord.st).r, 900)), 0.0, 1.0));
	if (texture2D(texture1, coord).r == 1.0) {
		final = skyColor;
	} else {
		
		final = texture2D(texture0, coord) * texture2D(texture3, coord);
		//final = vec4(0, coord.x, 0.0, 1.0);
	}
	
	if (fog != 0.0) {
	    //final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), clamp(1.0 / exp(texture2D(texture1, texCoord.st).y * fog), 0.0, 1.0)), final.w);
	    //float fg = pow(texture2D(texture1, texCoord.st).x, 300);
		//final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), 1 - texture2D(texture1, texCoord.st).x ), final.w);
	   	//final += vec4(fg, fg, fg, 0.0);
	   	//final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), clamp(1.0 / exp(exp(exp(texture2D(texture1, texCoord.st).x)) * fog), 0.0, 1.0)), final.w);
    }
	
	//float v = 0;//(float(mod(coord.y * 80, 2) < 0.9)) * 0.2 + mod(coord.y * 80, 2) / 24;
	//v = pow(length(coord - vec2(0.5, 0.5)) * sqrt(2), 4) * 0.7;
	//v *= 60;
	//v *= abs(coord.x - 0.5) / 4;
	//v *= abs(coord.y - 0.5);
	//final -= vec4(v, v, v, 0.0);
    gl_FragData[0] = texture2D(texture0, texCoord.st);
    gl_FragData[1] = texture2D(texture2, texCoord.st);
    gl_FragData[2] = final;
    gl_FragData[3] = texture2D(texture3, texCoord.st);
}