/**
 * @file Masterscript, this script is run first
 * @author Neal Nicdao (Capital_Asterisk)
 */

kdion.initialize(function() {
	
	OKO_Camera_ = OKO_Camera;
	
	kdion.log("Hello world");
	kdion.Bird();
	var f = new kdion.Bird();
	kdion.log("Bird: " + f);
	kdion.log("Bird is crushed: " + f.crushed);
	kdion.log("Bird integrity: " + f.integrity);
	kdion.log("Crushing bird...");
	f.crushed = true;
	f.integrity = 5.5;
	kdion.log("Bird is crushed: " + f.crushed);
	kdion.log("Bird is integrity: " + f.integrity);
	var c = new kdion.Chicken();
	kdion.log("Bird: " + c);
	kdion.log("Bird is crushed: " + c.crushed);
	kdion.log("Bird integrity: " + c.integrity);
	kdion.log("Crushing bird...");
	c.crushed = true;
	c.integrity = 5.5;
	kdion.log("Bird is crushed: " + c.crushed);
	kdion.log("Bird is integrity: " + c.integrity);
	kdion.log("World: " + kdion.World)
	
	var flor = new KCompnent("infplane");
	
	var camera = new OKO_Camera_();
	camera.setParent(kdion.World);

	var e = new KObj_Entity();
	e.addComponent(flor);
	e.translate(0.0, 0.5, 0.0);
	kdion.e = e;
	e.setName("Cube_Base");
	
	var left = new KObj_Entity();
	left.translate(-1.0, -0.5, 0.0);
	left.setName("Cube_Left");
	
	var top = new KObj_Entity();
	top.translate(0.5, 1.0, 0.0);
	top.setName("Cube_Top");
	
	
	camera.translate(0.0, 2.0, 6.0);
	//kdion.log(kdion.camera);
	
	
	e.setParent(kdion.World, "hey theretttt", 5, 5, 8);
	top.setParent(e);
	left.setParent(top);
	
	kdion.load({textures: ["tiles_diff", "tiles_norm"]});
	kdion.load({textures: 5});
	kdion.camera = camera;
	
	//kdion.log(mat4.identity);
	var tm = mat4.fromValues(1);
	mat4.identity(tm);
	kdion.log("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
	//kdion.log("[" + tm.join(", ") + "]");
	kdion.log("["
		+ tm[0] + ", " + tm[1] + ", " + tm[2] + ", " + tm[3] + "]\n["
		+ tm[4] + ", " + tm[5] + ", " + tm[6] + ", " + tm[7] + "]\n["
		+ tm[8] + ", " + tm[9] + ", " + tm[10] + ", " + tm[11] + "]\n["
		+ tm[12] + ", " + tm[13] + ", " + tm[14] + ", " + tm[15] + "]\n");
	flor.setMatrix(tm);
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
	var dist = vec3.distance(foo, bar);
	//kdion.log("foo: " + vec3.distance(foo, bar));
	if (dist > 6) {
		//kdion.camera.setPosition(vec3.add(bar, unit));
		// raise foo, doesn't actually raise the cube, only this vector.
		// this makes the camera move towards a point above the cube
		foo[1] += 2.6;
		// get a direction between the two objects
		var unit = vec3.sub(vec3.fromValues(0, 0, 0), bar, foo);
		vec3.normalize(unit, unit);
		// then scale it to 6
		vec3.scale(unit, unit, 6);
		// add so that foo is on the edge of the 6m radius, this makes the next
		// operation go smoothly
		vec3.add(foo, unit, foo);
		// Smoothly bar towards foo
		vec3.lerp(bar, foo, bar, 0.95);
		// self explanatory
		kdion.camera.setPosition(bar);
	}// else {
		//bar[1] -= (bar[1] - (foo[1] + 6));
	//}
	kdion.camera.pointAt(kdion.e);
	//kdion.camera.rotate([(Math.random() - 0.5) / 100, (Math.random() - 0.5) / 100 + 3, (Math.random() - 0.5) / 100]);
	kdion.e.translate(-kdion.input["MOVE_X"] * 0.3, 0.0, kdion.input["MOVE_Y"] * 0.3);
	//kdion.log(kdion.camera);
	//kdion.camera.translate(0.0, 0.01, kdion.input["MOVE_Y"] * 0.03);
	
});

