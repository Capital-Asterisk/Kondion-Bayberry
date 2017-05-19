kdion.error = kdion.log;

kdion.log("Kondion Bayberry default JS carton");
kdion.log("Loading glmatrix...");
kdion.require("kdefault:glmatrix/gl-matrix-min");

//console = {
//  log: kondion.log
//};

String.prototype.between = function(index, start, cregex) {
  var i = start , j = 0, k = 0;
  while (i < this.length) {
    if (cregex.test(this[i])) {
      if (k == index) {
        return {indexA: i, indexB: j, slice: this.slice(j, i)};
      } else {
        k ++;
        j = i + 1;
      }
    }
    i ++;
  }
  return undefined;
};

String.prototype.midCut = function(a, b) {
  // silly how JS has 3 functions to remove the edges of a string, but none
  // that could remove the middle.
  return this.slice(0, a) + this.substr(b);
};

Array.prototype.twoDimRegex = function(regex, limit) {
  var ret = [];
  for (var i = 0; i < this.length; i ++) {
    if (this[i].constructor == Array) {
      var l = (!limit) ? this[i].length : Math.min(limit, this[i].length);
      for (var j = 0; j < l; j ++) {
        if (regex.test("" + this[i][j])) {
          ret.push(i);
          ret.push(j);
        }
      }
    }
  }
  return ret;
};

kdion.materialParser = function(code) {
  //kdion.log("EEEEEEEE" + code);

  // TODO: redo this entire thing to be more like actual javascript
  //     : add new string functions to help with parsing
  // this code is really bad

  var whitespace = /\s/;

  var stop = false;
  var i = 0;
  var j = 0;
  var mode = 0;
  var name = "";
  var stack = "";
  
  var meta = {};
  var uniforms = [];
  // uniforms are
  
  // get headers and shit
  if (code.startsWith("TWMh")) {
    // oh yeah man
    if (!code.startsWith("TWMhvbay")) {
      //kdion.log("Unsupported TWM version");
      return "e: Unsupported version";
    }
  } else {
    // TODO: check for TWM binary
    //kdion.err("");
    //kdion.log("Not a TWMh file");
    return "e: Not a TWNh, or TWM binary not yet supported";
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
          //kdion.log("Error in parsing material metadata [" + stack + "]");
          return "e: Error in parsing material metadata [" + stack + "]";
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
  while (whitespace.test(code[0]))
    code = code.substr(1);
  
  kdion.log(code);
  
  // the following would be strings, commas, then data
  
  // Get uniforms
  
  i = 0; // current uniform, [0, 1, 2 ...]
  j = 0; // character index of uniform
  //stop = false;
  while ((j = code.indexOf("u" + i + ": ")) != -1) {
    kdion.log("U" + i + ": " + code.between(1, j, whitespace).slice
              + " " + code.between(2, j, whitespace).slice)
    uniforms[i] = [code.between(1, j, whitespace).slice, code.between(2, j, whitespace).slice];
    i ++;
  }
  
  var smallsize = 0;  
  // now actual node parsing
  // for every node, #x#:
  while ((j = code.search(/\d*x\d*:/)) != -1) {
    smallsize = code.indexOf(":", j);
    code = code.substr(smallsize + 2);
    var nodes = [];
    var k = 0;
    // For every row, divided by '| -'
    while ((k = code.search(/\|\s*?-/)) != -1) {
      //kdion.log(k);
      // set stack to the current row
      stack = code.substring(0, k);
      // remove the row from code
      code = code.substr(code.indexOf("-", k) + 1);
      // split the row by the '|' character into individual nodes. put array in
      // nodes.
      var l = nodes.push(stack.trim().split("|")) - 1;
      //kdion.log(nodes[l]);
      for (i = 0; i < nodes[l].length; i ++) {
        //kdion.log(nodes[l][i] = nodes[l][i].trim());
        nodes[l][i] = nodes[l][i].trim();
      }
      
    }
    // Step 1: Find the out node
    
    var r = nodes.twoDimRegex(/out/);
    //for (i = 0; i < r.length / 2; i ++) {
    //  kdion.log("EGGS: " + r[i * 2] + " " + r[i * 2 + 1])
    //}
    if (r.length != 2) {
      //kdion.log("E");
      return "e: multiple output nodes"
    }
    
    //var findOutputRegex = new RegExp("\\].*\\[.*eggs.*\\]$");
    
    var thisisarecursivefunction = function(y, x) {
      
      //var node = nodes[y][x];
      //kdion.log("node: " + node);
      
      var funcName = nodes[y][x].split(/\[|\]/);
      //var values = nodes[y][x].between(1, 0, /\[|\]/).slice.split(",");
      var ret = funcName[1];
      var values = ret.split(",");
      //kdion.log("values:" + funcName[1] + " " + funcName[2]);
      funcName = funcName[2];
      

      //kdion.log("Function name: " + nodes[y][x].between(2, 0, /\[|\]/).slice);
      // replace(/\[.*\]/g,"") regex to get function name is unknown
      // use non regex instead.
      //funcName = ()
      
      for (var l = 0; l < values.length; l ++) {
        if (values[l] != "") {
          var next = nodes.twoDimRegex(
              new RegExp("\\].*\\[.*" + values[l] + ".*\\]$"));
          // /[,\[].*\]$/
          if (next.length != 2) {
            if (next.length == 0)
              kdion.error("Unknown input: [" + values[l] + "]; for now, no system is in place to prevent these errors.")
            else
              kdion.error("Multiple inputs for [" + values[l] + "]; for now, no system is in place to prevent these errors.")
          } else {
            //kdion.log("next: " + nodes[next[0]][next[1]]);
            ret = ret.replace(values[l], thisisarecursivefunction(next[0], next[1]));
            //return nodes[y][x].replace(values[l], thisisarecursivefunction(next[0], next[1]));
            //
            
          }
        }
      }
      
      return funcName + "(" + ret + ")";
    }
    
    kdion.log(thisisarecursivefunction(r[0], r[1]));
    
    //kdion.log(code);
  }
};
