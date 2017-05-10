kdion.log("Kondion Bayberry default JS carton");
kdion.log("Loading glmatrix...");
kdion.require("kdefault:glmatrix/gl-matrix-min");

String.prototype.midCut = function(a, b) {
  // silly how JS has 3 functions to remove the edges of a string, but none
  // that could remove the middle.
  return this.slice(0, a) + this.substr(b);
}

kdion.materialParser = function(code) {
  //kdion.log("EEEEEEEE" + code);

  var whitespace = /\s/;

  var stop = false;
  var i = 0;
  var j = 0;
  var mode = 0;
  var name = "";
  var stack = "";
  var meta = {};
  
  // get headers and shit
  if (code.startsWith("TWMh")) {
    // oh yeah man
    if (!code.startsWith("TWMhvbay")) {
      kdion.log("Unsupported TWM version");
      return "";
    }
  } else {
    // TODO: check for TWM binary
    //kdion.err("");
    kdion.log("Not a TWMh file");
    return "";
  }
  
  // Maybe shorten everything down to 1 loop
  
  // Remove all comments
  j = -1;
  i = 0;
  while (i < code.length) {
    if (j == -1 && code[i] == "#") {
      //kdion.log("# on: " + i);
      j = i;
    } else if (j != -1 && code[i] == "\n") {
      code = code.midCut(j, i + 1);
      i = j - 1;
      j = -1;
    }
    i ++;
  }
  
  //code = code.replace("\n", "")
  // Now all comments are removed, parse time
  //kdion.log(code);
  
  // Metadata
  i = 8;
  j = -1; // location of first bracket
  mode = 0;
  name = "";
  stack = "";
  
  while (i < code.length && mode >= 0) {
    if (j == -1 && code[i] == "{") {
      j = i;
    } else if (mode == 0 && code[i] == "}") {
      mode = -1;
    } else if (mode <= 1 && !whitespace.test(code[i])) {
      // if normal mode and not whitespace
      // scanning name
      mode = 1;
      if (code[i] == ":") {
        // change to mode 2 on the colon
        mode = 2;
        // skip any whitespace after the colon
        while (whitespace.test(code[i + 1]))
          i ++;
      } else 
        name += code[i];
    } else if (mode == 2 || mode == 3) {
      if (code[i] == "\"" && code[i - 1] != "\\") {
        // a " without an escape was detected
        // toggle string mode and (not) allow whitespace
        // overcomplicated toggler
        mode = (1 ^ (mode - 2)) + 2
      }
      
      if (mode == 2 && ((whitespace.test(code[i]) || code[i] == ","))) {
        // if mode is 2 (not in a string) and the current character is
        // whitespace or comma,
        // move on to the next entry
        try {
          // add the entry to the metadeta object
          meta[name.trim()] = JSON.parse(stack);
          kdion.log("META: " + name.trim() + ":" + stack);
        } catch(e) {
          kdion.log("Error in parsing material metadata [" + stack + "]");
        }
        name = "";
        stack = "";
        mode = 0;
        // skip whitespace and commas 
        while (whitespace.test(code[i + 1]) || code[i + 1] == ",")
          i ++;
      } else {
        stack += code[i];
      }
    }
    i ++;
  }
  
  //kdion.log(meta);
  
  // clip out the headers and metadata
  code = code.substr(i);
  kdion.log(code);
  
  // the following would be strings, commas, then data
  
  /*
  while (i < code.length || stop) {
  
    if (code[i] == "#") {
      // Comment, keep going until 
      //kdion.log(code[i]);
      while (code[i + 1] != "\n") {
        //kdion.log(code[i]);
        i ++;
      }
    } else if (whitespace.test(code[i])) {
      // if whitespace
      switch (mode) {

        case 0:
          // header
          kdion.log(stack);
          if (stack.startsWith("TWMh")) {
            // It's a TWMh!
          } else {
            kdion.log("Input file is not a TWMh");
            return "";
          }
          mode = 1;
          stack = "";
          break;

        case 1:
          //kdion.log(stack);
          stack = "";
          break;
      }
      
      // skip more whitespace, if blank spaces are more than 2 characters
      while (whitespace.test(code[i + 1]))
        i ++;

    } else {
      stack += code[i];
    }
    i++;
  }
  */

}
