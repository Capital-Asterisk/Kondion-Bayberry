/**
 * @file Masterscript, this script is run first
 * @author Neal Nicdao (Capital_Asterisk)
 */
 
var matman, ripmat, flrmat;

OKO_Camera_ = OKO_Camera;

function Character() {
  var obj = new KObj_Entity();
  obj.health = 100;
  obj.isActive = false;
  obj.onSwitchedTo = undefined;
  obj.physLevel(1 | 4 | 16 | 64);
  kdion.characters.push(obj);
  return obj;
}

var quickMakeACube = function(name, pos, velo) {
  var ent = new Character();
  var woot = new KCompnent("sphere");
  ent.setName(name);
  ent.addComponent(woot);
  ent.setMaterial(ripmat);
  ent.translate(pos[0], pos[1], pos[2]);
  ent.setParent(kdion.World);
  ent.thrustN([0.0, 0.05, 0.0], velo);
  return ent;
}

var quickMakeAFan = function(name, pos, velo) {
  var ent = new Character(),
      collider = new KCompnent("cube"),
      main = new KCompnent("mesh"),
      head = new KCompnent("mesh"),
      blade = new KCompnent("mesh");

  var tm = mat4.create();

  mat4.fromTranslation(tm, [0.0, 3.02204, -0.34683]);
  head.setMatrix(tm);

  mat4.fromTranslation(tm, [0.0, 4.11466, -0.27625]);
  blade.setMatrix(tm);

  main.setData("FanMain");
  head.setData("FanHead");
  blade.setData("FanBlade");

  ent.addComponent(collider);
  ent.addComponent(main);
  ent.addComponent(head);
  ent.addComponent(blade);
  
  ent.translate(pos[0], pos[1], pos[2]);
  ent.setParent(kdion.World);
  ent.thrustN([0.0, 0.05, 0.0], velo);
  ent.setMaterial(flrmat);
  ent.collided = false;

  ent.onupdate = function() {
    if (this.isActive && this.collided) {
      if (kdion.input["DEBUGA"]) {
        this.thrustN([0.0, 0.2, 0.0], [0.0, 1.0, 0.0]);
      }

      var foo = vec3.create();
      var bar = vec3.create();

      this.moving = (Math.abs(kdion.input["MOVE_X"])
                      + Math.abs(kdion.input["MOVE_Y"])) > 0.1;

      if (this.moving) {
        kdion.camera.dirRt(foo);
        kdion.camera.dirFd(bar);
        foo[1] = 0;
        bar[1] = 0;
        vec3.normalize(foo, foo);
        vec3.normalize(bar, bar);
        vec3.scale(foo, foo, -kdion.input["MOVE_X"] * 30);
        vec3.scale(bar, bar, -kdion.input["MOVE_Y"] * 30);
        vec3.add(foo, foo, bar);
        this.setVelocity([foo[0], 0.0, foo[2]]);
      } else {
        // Decelerate
        this.getVelocity(foo);
        vec3.negate(foo, foo);
        //this.thrustN([0.0, 0.0, 0.0], foo);
        this.setVelocity([0.0, 0.0, 0.0]);
        kdion.debug.fooo = foo.toString();
        
      }
    }
    this.collided = false;
  }

  ent.oncollide = function(ci) {
    this.collided = true;
    kdion.debug.ci = ci.entA;
    if (this.isActive)
      kdion.debug.hhh = Math.random();
  }

  return ent;
}

kdion.initialize(function() {

  kdion.log("Hello world 2");

  var ground = new KObj_Entity();
  var flat = new KCompnent("infplane");
  var wall = new KCompnent("infplane");
  var celi = new KCompnent("infplane");
  var cubeA = new KCompnent("mesh");
  var cubeB = new KCompnent("cube");
  var cubeC = new KCompnent("cube");
  
  matman = new KMaterial("defmat");
  flrmat = new KMaterial("ground");
  ripmat = new KMaterial("test");
  nrmmat = new KMaterial("normals");
  skymat = new KMaterial("sky");
  
  kdion.characters = [];
  kdion.currentCharacter = 0;
  
  var tm = mat4.create();
  mat4.rotateY(tm, tm, Math.PI / 4 * 2);
  mat4.rotateX(tm, tm, Math.PI / 4 * 2.0);
  mat4.translate(tm, tm, vec3.fromValues(0.0, 0.0, -3.0))
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
  e.onupdate = function() {
    kdion.debug.thing = Math.random();
  }

  mat4.fromTranslation(tm, [0.0, 0.0, 0.0]);
  mat4.fromScaling(tm, vec3.fromValues(1.0, 1.0, 1.0));
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

  kdion.queueLoad({textures: ["tiles_diff", "tiles_norm", "FanMain"],
                   materials: ["test", "ground", "normals", "sky"],
                   meshes: ["thing", "FanMain", "FanHead", "FanBlade"]}, 2);
  kdion.camera = camera;

  var sky = new RKO_Sky();
  sky.setMaterial(skymat);
  sky.setParent(kdion.World);

  kdion.doLoad(2);

});

kdion.globalUpdate(function() {

  // CUBE SPAM!
  if (kdion.input["DEBUGB"]) {
    quickMakeAFan("asdf" + Math.random(), [0.0, 20.0 * Math.random(), 5.0], [
          Math.random() * 24.0 - 12.0,
          -Math.random() * 10.0,
          Math.random() * 24.0 - 12.0]);
  }

  kdion.debug.I = JSON.stringify(kdion.input);
  kdion.debug.F = kdion.characters[kdion.currentCharacter];
  if (kdion.characters[kdion.currentCharacter]) {
    kdion.camera.pointAt(kdion.characters[kdion.currentCharacter]);
    kdion.characters[kdion.currentCharacter].isActive = true;
  //  kdion.debug.F = kdion.characters[kdion.characters.current];
  }

  flrmat.setUniform(2, Math.floor(Math.random() * 3) * 0 + 2);
  
});

