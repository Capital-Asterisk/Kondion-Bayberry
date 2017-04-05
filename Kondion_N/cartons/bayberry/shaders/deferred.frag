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
	vec2 coord = vec2(1 - texCoord.x, 1 - texCoord.y);
	//coord.x = mod((coord.x - 0.5) / 2 + 0.5 - sin(((1 - coord.x) - 0.5) * 3) / 4, 1.0);
	//coord.y = mod((coord.y - 0.5) / 2 + 0.5 - sin(((1 - coord.y) - 0.5) * 3) / 4, 1.0);

	//coord.x += 0.01;
	//coord.x = mod(coord.x, 1.0);
	//coord.x += 0.5;
	//final = mix(texture2D(texture0, texCoord.st), vec4(0.0, 0.0, 0.0, 1.0), clamp(vec4(pow(texture2D(texture1, texCoord.st).r, 900)), 0.0, 1.0));
	if (texture2D(texture1, texCoord.st).r == 1.0) {
		// if depth is very deep, then it's sky'
		final = skyColor;
	} else {

		final = texture2D(texture0, texCoord.st);// * texture2D(texture3, coord);
		
		// Not anti aliasing
		//final = mix(final, texture2D(texture0, texCoord.st + vec2(1.0 / 800.0, 1.0 / 600.0)), 0.5);
		//final = vec4(0, coord.x, 0.0, 1.0);
	}

	if (fog != 0.0) {
		//final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), clamp(1.0 / exp(texture2D(texture1, texCoord.st).y * fog), 0.0, 1.0)), final.w);
		//float fg = pow(texture2D(texture1, texCoord.st).x * fog
		//	+ (texture2D(texture1, texCoord.st).x - fog), 100);
		float fg = pow(max(pow(texture2D(texture1, texCoord.st).x, 100) * (1 + fog) - fog, 0), 3);
		//0.3 + 0.3 / (1 / (1 - 0.3))
		//final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), 1 - texture2D(texture1, texCoord.st).x ), final.w);
		final += vec4(fg, fg, fg, 0.0);
		//final = vec4(mix(vec3(1.0, 1.0, 1.0), vec3(final.xyz), clamp(1.0 / exp(((texture2D(texture1, texCoord.st).x)) * fog), 0.0, 1.0)), final.w);
	}

	//float v = 0;//(float(mod(coord.y * 80, 2) < 0.9)) * 0.2 + mod(coord.y * 80, 2) / 24;
	//v = pow(length(coord - vec2(0.5, 0.5)) * sqrt(2), 4) * 0.7;
	//v *= 60;
	//v *= abs(coord.x - 0.5) / 4;
	//v *= abs(coord.y - 0.5);
	//final -= vec4(v, v, v, 0.0);
	
	// red/blue gradient for some reason
	final += vec4(texCoord.x, 0.0, texCoord.y, 1.0) / 6;

	gl_FragData[0] = texture2D(texture0, coord);
	gl_FragData[1] = texture2D(texture2, coord);
	gl_FragData[2] = final;
	gl_FragData[3] = texture2D(texture3, coord);
}

