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
	kdion.log("Crushing bird...");
	f.crushed = true;
	kdion.log("Bird is crushed: " + f.crushed);
	
	
});

