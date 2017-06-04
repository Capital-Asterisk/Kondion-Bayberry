#version 120

// This shader is now for debug only

uniform int type;
uniform float fog;
uniform vec4 skyColor;
uniform bool normalmode;

// 0 Depth
// 1 Materials
// 2 Coords
// 3 Normals
// 4 Mapped normals
// 5 Brightness
// 6 Specular

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

varying vec3 normal;
varying vec4 texCoord;
varying vec4 viewPos;
varying vec4 worldPos;
varying mat4 cuteMatrix;

void main(){
	vec4 final = vec4(0.0, 0.0, 0.0, 0.0);
	//vec2 coord = vec2(texCoord.x, texCoord.y);
	//coord.x = mod((coord.x - 0.5) / 2 + 0.5 - sin(((1 - coord.x) - 0.5) * 3) / 4, 1.0);
	//coord.y = mod((coord.y - 0.5) / 2 + 0.5 - sin(((1 - coord.y) - 0.5) * 3) / 4, 1.0);

	//coord.x += 0.01;
	//coord.x = mod(coord.x, 1.0);
	//coord.x += 0.5;
	//final = mix(texture2D(texture0,f texCoord.st), vec4(0.0, 0.0, 0.0, 1.0), clamp(vec4(pow(texture2D(texture1, texCoord.st).r, 900)), 0.0, 1.0));
	if (texture2D(texture0, texCoord.st).r == 1.0) {
		// if depth is very deep, then it's sky'
		final = skyColor;
	} else {

		final = texture2D(texture2, texCoord.st);
		
		// Not anti aliasing
		//final = mix(final, texture2D(texture0, texCoord.st + vec2(1.0 / 800.0, 1.0 / 600.0)), 0.5);
		//final = vec4(0, coord.x, 0.0, 1.0);
	}

	if (fog != 0.0) {
		float fg = pow(max(pow(texture2D(texture0, texCoord.st).x, 100) * (1 + fog) - fog, 0), 3);
		final += vec4(fg, fg, fg, 0.0);
	}
	
	// red/blue gradient for some reason
	final += vec4(texCoord.x, 0.0, texCoord.y, 1.0) / 6;

  final += texture2D(texture3, texCoord.st);

	gl_FragData[0] = final;
	//gl_FragData[1] = texture2D(texture2, coord);
	//gl_FragData[2] = final;
	//gl_FragData[3] = texture2D(texture3, coord);
}

