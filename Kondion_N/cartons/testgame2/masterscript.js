/**
 * @file Masterscript, this script is run first
 * @author Neal Nicdao (Capital_Asterisk)
 */

kdion.initialize(function() {
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
	var e = new KObj_Entity();
	kdion.e = e;
	e.setParent(kdion.World, "hey theretttt", 5, 5, 8);
	//kdion.load({textures: ["tiles_diff", "tiles_norm"]});
	kdion.load({textures: 5});
});

kdion.globalUpdate(function() {
	//kdion.log("");
	//kdion.log(kdion.input["MOUSE_X"]);
	if (kdion.input["MOVE_X"])
		kdion.e.setPosition([0, Math.random(), 0]);
	
});

