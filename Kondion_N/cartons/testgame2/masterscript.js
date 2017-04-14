/**
 * @file Masterscript, this script is run first
 * @author Neal Nicdao (Capital_Asterisk)
 */

kdion.initialize(function() {
	
	OKO_Camera_ = OKO_Camera;
	
	kdion.log("Hello world");
	//kdion.Bird();
	
	var ground = new KObj_Entity();
	var flat = new KCompnent("infplane");
	var cube = new KCompnent("cube");
	var tm = mat4.create();
	mat4.rotateX(tm, tm, Math.PI / 2 + Math.PI / 12 * 0);
	flat.setMatrix(tm);
	mat4.fromScaling(tm, [4, 4, 4]);
	cube.setMatrix(tm);
	ground.addComponent(flat);
	ground.translate(0.0, -9.0, 0.0);
	ground.setName("Ground");
	ground.setParent(kdion.World);
	ground.physLevel(0);
	
	var camera = new OKO_Camera_();
	camera.translate(0.0, 2.0, 6.0);
	camera.setParent(kdion.World);

	// BUG: add children after adding it to the world
	
	var e = new KObj_Entity();
	e.translate(0.0, 1.0, 0.0);
	e.setName("Cube_Base");
	e.setParent(kdion.World);
	e.addComponent(cube);
	kdion.e = e;
	
	var left = new KObj_Entity();
	left.translate(-1.0, 1.0, 0.0);
	left.setName("Cube_Left");
	left.setParent(e);
	
	var top = new KObj_Entity();
	top.translate(1.0, 2.0, 0.0);
	top.setName("Cube_Top");
	top.setParent(e);
	
	kdion.load({textures: ["tiles_diff", "tiles_norm"]});
	kdion.load({textures: 5});
	kdion.camera = camera;
	
	//kdion.log(mat4.identity);
	//var tm = mat4.fromValues(1);
	//mat4.identity(tm);
	kdion.log("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	//kdion.log("[" + tm.join(", ") + "]");
	//mat4.translate(tm, tm, [11, 22, 33]);
	//mat4.rotateX(tm, tm, Math.PI / 2);
	//kdion.log("["
	//	+ tm[0] + ", " + tm[1] + ", " + tm[2] + ", " + tm[3] + "]\n["
	//	+ tm[4] + ", " + tm[5] + ", " + tm[6] + ", " + tm[7] + "]\n["
	//	+ tm[8] + ", " + tm[9] + ", " + tm[10] + ", " + tm[11] + "]\n["
	//	+ tm[12] + ", " + tm[13] + ", " + tm[14] + ", " + tm[15] + "]\n");
});

kdion.globalUpdate(function() {
	//kdion.log("");
	//kdion.log(kdion.input["MOUSE_X"]);
	//kdion.log(new Date().getTime());
	// Make some empty vectors (inefficient)
	var foo = vec3.create();
	var bar = vec3.create();
	// Get the positions and put it in the vectors
	kdion.e.getPosition(foo);
	kdion.camera.getPosition(bar);
	// Get the distance between the camera and the cube
	var dist = vec2.distance(vec2.fromValues(foo[0], foo[2]),
			vec2.fromValues(bar[0], bar[2]));
	//kdion.log("foo: " + dist);
	
	if (dist > 12) {
		//kdion.camera.setPosition(vec3.add(bar, unit));
		// raise foo, doesn't actually raise the cube, only this vector.
		// this makes the camera move towards a point above the cube
		foo[1] += 2.6;
		// get a direction between the two objects
		var unit = vec3.sub(vec3.fromValues(0, 0, 0), bar, foo);
		unit[1] = 0;
		vec3.normalize(unit, unit);
		// then scale it to 6
		vec3.scale(unit, unit, 12);
		// add so that foo is on the edge of the 6m radius, this makes the next
		// operation go smoothly
		vec3.add(foo, unit, foo);
		// Smoothly bar towards foo
		vec3.lerp(bar, foo, bar, 0.96);
		// self explanatory
		kdion.camera.setPosition(bar);
	}// else {
		//bar[1] -= (bar[1] - (foo[1] + 6));
	//}
	kdion.camera.pointAt(kdion.e);
	//kdion.camera.rotate([(Math.random() - 0.5) / 100, (Math.random() - 0.5) / 100 + 3, (Math.random() - 0.5) / 100]);
	
	// Cube movement
	// reuse foo and bar
	// get camera's left and forward direction
	kdion.camera.dirRt(foo);
	kdion.camera.dirFd(bar);
	// clear the y component, this is for horizontal movement
	foo[1] = 0;
	bar[1] = 0;
	// now normalize just in case
	vec3.normalize(foo, foo);
	vec3.normalize(bar, bar);
	// Now multiply by input and speed
	vec3.scale(foo, foo, -kdion.input["MOVE_X"] * 12);
	vec3.scale(bar, bar, -kdion.input["MOVE_Y"] * 12);
	
	// add foo and bar, put result in foo
	vec3.add(foo, foo, bar);
	//kdion.log(foo[0] + " " + foo[1] + " " + foo[2]);
	
	// Then translate, change this to something else soon
	kdion.e.accel([foo[0], 0.0, foo[2]]);
	//kdion.log(kdion.camera);
	//kdion.camera.translate(0.0, 0.01, kdion.input["MOVE_Y"] * 0.03);
	
});

