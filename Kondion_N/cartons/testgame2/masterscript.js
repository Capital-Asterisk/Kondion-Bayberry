/**
 * @file Masterscript, this script is run first
 * @author Neal Nicdao (Capital_Asterisk)
 */
 
var matman, ripmat, flrmat;

function quickMakeACube(name, pos, velo) {
  var ent = new KObj_Entity();
  var woot = new KCompnent("sphere");
  ent.setName(name);
  ent.addComponent(woot);
  ent.setMaterial(ripmat);
  ent.translate(pos[0], pos[1], pos[2]);
  ent.setParent(kdion.World);
  ent.thrustN([0.0, 0.05, 0.0], velo);
  return ent;
}

kdion.initialize(function() {
  
  OKO_Camera_ = OKO_Camera;
  
  kdion.log("Hello world");
  //kdion.Bird();
  
  var ground = new KObj_Entity();
  var flat = new KCompnent("infplane");
  var wall = new KCompnent("infplane");
  var celi = new KCompnent("infplane");
  var cubeA = new KCompnent("mesh");
  var cubeB = new KCompnent("sphere");
  var cubeC = new KCompnent("cube");
  
  matman = new KMaterial("defmat");
  flrmat = new KMaterial("ground");
  ripmat = new KMaterial("test");
  nrmmat = new KMaterial("normals");
  skymat = new KMaterial("sky");
  
  var tm = mat4.create();
  mat4.rotateY(tm, tm, Math.PI / 4 * 2);
  mat4.rotateX(tm, tm, Math.PI / 4 * 2.0);
  flat.setMatrix(tm);
  mat4.identity(tm);
  mat4.rotateY(tm, tm, Math.PI / 4 * 4);
  mat4.rotateX(tm, tm, Math.PI / 4 * 9.0);
  wall.setMatrix(tm);
  mat4.identity(tm);
  mat4.rotateX(tm, tm, -Math.PI / 2);
  mat4.rotateY(tm, tm, -Math.PI / 36);
  mat4.translate(tm, tm, [0, 0, 8]);
  celi.setMatrix(tm);
  //wall.setMatrix();
  //mat4.fromScaling(tm, [4, 4, 4]);
  ground.addComponent(flat);
  ground.addComponent(wall);
  //ground.addComponent(celi);
  ground.translate(0.0, -9.0, 0.0);
  ground.setName("Ground");
  ground.setParent(kdion.World);
  ground.physLevel(0);
  ground.setMaterial(flrmat);
  
  var camera = new OKO_Camera_();
  camera.translate(0.0, 2.0, 6.0);
  camera.setParent(kdion.World);

  // BUG: add children after adding it to the world
  
  var e = new KObj_Entity();
  e.translate(0.0, 1.0, 0.0);
  e.setName("Cube_Base");
  e.setParent(kdion.World);
  mat4.fromScaling(tm, vec3.fromValues(1.0, 1.0, 1.0));
  //mat4.translate(tm, tm, vec3.fromValues(0.0, -0.5, 0.0)); 
  cubeA.setMatrix(tm);
  cubeA.setData("thing"); // Connect it to the "thing" mesh
  e.addComponent(cubeA);
  kdion.e = e;
  
  //var left = new KObj_Entity();
  //left.translate(-1.0, 1.0, 0.0);
  //left.setName("Cube_Left");
  //left.setParent(e);
  mat4.fromTranslation(tm, [0.0, 0.0, 0.0]);
  mat4.fromScaling(tm, vec3.fromValues(2.0, 2.0, 2.0));
  cubeB.setMatrix(tm);
  e.addComponent(cubeB);
  
  var top = new KObj_Entity();
  top.translate(2.0, 3.0, 0.0);
  top.setName("Cube_Top");
  top.addComponent(cubeC);
  top.setParent(e);
  top.setMaterial(ripmat);
  
  mat4.fromScaling(tm, vec3.fromValues(0.2, 0.2, 0.2)); 
  
  var debugA = new KObj_Entity();
  var cubeD = new KCompnent("cube");
  cubeD.setMatrix(tm);
  debugA.setName("DebugA");
  debugA.addComponent(cubeD);
  debugA.setMaterial(ripmat);
  debugA.setParent(kdion.World);
  debugA.translate(-4, 5, 0);
  debugA.physLevel(0);
  
  var debugB = new KObj_Entity();
  var cubeE = new KCompnent("cube");
  cubeE.setMatrix(tm);
  debugB.setName("DebugB");
  debugB.addComponent(cubeE);
  debugB.setMaterial(ripmat);
  debugB.setParent(kdion.World);
  debugB.translate(-4, 5, 0);
  debugB.physLevel(0);
  
  kdion.queueLoad({textures: ["tiles_diff", "tiles_norm"],
                   materials: ["test", "ground", "normals", "sky"],
                   meshes: ["thing"]}, 2);
  kdion.camera = camera;
  
  var sky = new RKO_Sky();
  sky.setMaterial(skymat);
  sky.setParent(kdion.World);
  
  //kdion.log(mat4.identity);
  //var tm = mat4.fromValues(1);
  //mat4.identity(tm);
  //kdion.log("[" + tm.join(", ") + "]");
  //mat4.translate(tm, tm, [11, 22, 33]);
  //mat4.rotateX(tm, tm, Math.PI / 2);
  //kdion.log("["
  //  + tm[0] + ", " + tm[1] + ", " + tm[2] + ", " + tm[3] + "]\n["
  //  + tm[4] + ", " + tm[5] + ", " + tm[6] + ", " + tm[7] + "]\n["
  //  + tm[8] + ", " + tm[9] + ", " + tm[10] + ", " + tm[11] + "]\n["
  //  + tm[12] + ", " + tm[13] + ", " + tm[14] + ", " + tm[15] + "]\n");
  
  kdion.doLoad(2);
  
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
  
  var height = -kdion.input.MOUSE_Y / 100;
  
  if (dist > 12 || true) {
    //kdion.camera.setPosition(vec3.add(bar, unit));
    // raise foo, doesn't actually raise the cube, only this vector.
    // this makes the camera move towards a point above the cube
    foo[1] += height;
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
    // Rotate feature
    var cross = vec3.cross(vec3.fromValues(0, 0, 0), vec3.fromValues(0, 1, 0), unit);
    vec3.normalize(cross, cross);
    vec3.scale(cross, cross, kdion.inputd.MOUSE_X - kdion.input.MOUSE_X);
    vec3.scale(cross, cross, 0.04);
    vec3.add(bar, bar, cross);
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
  vec3.scale(foo, foo, -kdion.input["MOVE_X"] * 30);
  vec3.scale(bar, bar, -kdion.input["MOVE_Y"] * 30);
  //kdion.log(kdion.input["DEBUGA"]);
  // Press K to spaz
  if (kdion.input["DEBUGA"])
    kdion.e.thrustN([
          10.0,
          0.0,
          0.0],
          [0.0, 10.0, 0.0]);
  // Space to jump
  if (kdion.input["DEBUGC"])
    kdion.e.thrustN([
          0.0,
          0.0,
          0.0],
          [0.0, 1.0, 0.0]);
  // add foo and bar, put result in foo
  vec3.add(foo, foo, bar);
  //kdion.log(foo[0] + " " + foo[1] + " " + foo[2]);
  
  // Then translate, change this to something else soon
  kdion.e.thrustN([0.0, 4.0, 0.0], [foo[0] / 50.0, 0.0, foo[2] / 50.0]);

  kdion.e.getVelocity(foo)
  kdion.debug.MOUSE_XD = kdion.inputd.MOUSE_X - kdion.input.MOUSE_X;
  kdion.debug.MOUSE_X = kdion.inputd.MOUSE_X - kdion.input.MOUSE_X;
  kdion.debug["+ACCELERATION"] = (kdion.input["DEBUGA"]) ? "SPIN FAST!" : "OFF";

  // CUBE SPAM!
  if (kdion.input["DEBUGB"]) {
    quickMakeACube("asdf" + Math.random(), [0.0, 20.0 * Math.random(), 5.0], [
          Math.random() * 24.0 - 12.0,
          -Math.random() * 10.0,
          Math.random() * 24.0 - 12.0]);
  }
  
  flrmat.setUniform(2, Math.floor(Math.random() * 3) * 0 + 2);
  
});

