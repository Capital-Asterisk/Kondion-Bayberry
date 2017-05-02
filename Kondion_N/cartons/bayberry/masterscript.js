kdion.log("Kondion Bayberry default JS carton");
kdion.log("Loading glmatrix...");
kdion.require("kdefault:glmatrix/gl-matrix-min");
var test = vec3.fromValues(5, 5, 8);
kdion.log("Vector test: " + test + " "
		+ test.constructor + " " + (test instanceof Array));
		

kdion.materialParser = function(code) {
	//kdion.log("EEEEEEEE" + code);
	
	var whitespace = /\s/;
	
	var stop = false;
	var i = 0;
	var row = 0;
	var column = 0;
	var mode = 0;
	var stack = "";
	var meta = {};
	
	while (i < code.length || stop) {
		
		if (whitespace.test(code[i])) {
			// if whitespace
			if (mode == 0) {
				// header
				kdion.log(stack);
				mode = 1;
				stack = "";
			}
		} else {
			stack += code[i];
		}
		i ++;
	}
	
}

