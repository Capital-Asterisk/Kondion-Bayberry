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
	var tm = mat4.create();
	mat4.rotateX(tm, tm, Math.PI / 2);
	flat.setMatrix(tm);
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
	var foo = vec3.fromValues(0, 0, 0);
	var bar = vec3.fromValues(0, 0, 0);
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
	kdion.e.translate(-kdion.input["MOVE_X"] * kdion.delta * 12, 0.0, kdion.input["MOVE_Y"] * kdion.delta * 12);
	//kdion.log(kdion.camera);
	//kdion.camera.translate(0.0, 0.01, kdion.input["MOVE_Y"] * 0.03);
	
});

