/*
The MIT License (MIT)

Copyright (c) 2014 Mitsuaki KAWAMURA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**@file
 @brief Main routine
*/
/**@mainpage FermiSurfer Main Page

Fermisurfer displays Fermi surfaces 
with a color-plot of the arbitrary matrix element

@section Notation
 
- @f$\varepsilon_{n k}@f$ : Energy
- @f$\Delta_{n k}@f$ : Any @f$(n, k)@f$-dependent value for the color-plot.
- @f$N_b@f$ : Number of bands
 
@section sec_routine Important routines
- Main routine: main()
- Right click menu : FS_CreateMenu(), FS_ModifyMenu(let status)

@section sec_file Important files
- Main routine : fermisurfer.cpp
- Global variable : variable.hpp

@section sec_flow Flow

- main() :
  - read_file() : Read .frmsf file
  - compute_patch_segment() : Compute patch and segment
    - fermi_patch() : Compute patches constructing Fermi surface
  - display() : Display figures with OpenGL

*/
/*
 Input variables
*/
let ng0 = [0, 0, 0];         //!< @f$k@f$-point grid in the input file
let shiftk = [0, 0, 0];      //!< Whether @f$k@f$-grid is shifted or not
let nb = 0;             //!< The number of Bands
let avec = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; //!< Direct lattice vector
let bvec = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; //!< Reciprocal lattice vector
let eig0 = [];   //!< Eigenvalues @f$\varepsilon_{n k}@f$[::nb][::ng0[0]][::ng0[1]][::ng0[2]]
let mat0 = [];   //!< Matrix element [::nb][::ng0[0]][::ng0[1]][::ng0[2]][3]
/*
 Interpolation
*/
let ng = [0, 0, 0];        //!< @b Interpolated @f$k@f$-grids
let eig = [];  //!< Eigenvalues @f$\varepsilon_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]]
let mat = []; //!< Matrix element @f$\delta_{n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
let vf = [];  //!< Matrix element @f$\{\bf v}_{{\rm f} n k}@f$ [::nb][::ng[0]][::ng[1]][::ng[2]][3]
let interpol = 1;     //!< Ratio of interpolation
/*
 Switch for some modes
*/
let color_scale = 1; //!< Switch for full color scale mode
let fbz = 1;         //!< Switch for 1st Brillouin zone mode
let nodeline = 0;    //!< Switch for node lines
let lcolorbar = 1;   //!< Switch for colorbar
let lstereo = 1;     //!< Switch for the stereogram
let lmouse = 1;      //!< Switch for the mouse function
let lsection = 0;    //!< Switch for the 2D Fermi lines
let lequator = 0;    //!< Switch for equator
let BZ_number = [1, 1, 1];
/*
 Variables for Brillouin zone boundaries
*/
let nbzl = 1;      //!< The number of Lines of 1st Brillouin zone
let bzl = []; //!< Lines of 1st BZ [nbzl(max:26*26=676)][2][3]
let bragg = [];   //!< Bragg plane vectors
let brnrm = [];      //!< Norms of Bragg plane vectors
let brnrm_min = 0.0;     //!< Minimum scale of the reciplocal space
let nbragg = 0;             //!< Number of Bragg plane og 1st BZ
/*
 Variables for patches
*/
let ntri = [];          //!< The number of triangle patch [::nb]
let draw_band = [];     //!< Switch for drawn bands [::nb]
let nmlp = [];    //!< Normal vector of patchs [::nb][::ntri][3][3]
let kvp = [];    //!< @f$k@f$-vectors of points [::nb][::ntri][3][3]
let arw = [];
let nmlp_rot = []; //!< Normal vector of patchs [::nb][::ntri*3*3]
let kvp_rot = [];  //!< @f$k@f$-vectors of points [::nb][::ntri*3*3]
let arw_rot = [];
let matp = [];    //!< Matrix elements of points [::nb][::ntri][3][3]
let clr = [];      //!< Colors of points [::nb][::ntri*3*4]
let itet = 0;           //!< Counter for tetrahedron
let side = 1.0;       //!< Which side is lighted
let patch_max = 0.0;  //!< Max value across patch
let patch_min = 0.0;  //!< Max value across patch
/*
  Variables for nodeline
*/
let nnl = [];             //!< The number of nodeline
let kvnl = [];     //!< @f$k@f$-vector of nodeline [::nb][::nnl][2][3]
let kvnl_rot = []; //!< @f$k@f$-vector of nodeline [::nb][::nnl*2*3]
/*
 2D Fermi line
*/
let secvec = [0.0, 0.0, 0.0];         //!< @f$k@f$-vector to define section
let secvec_fr = [0.0, 0.0, 0.0];         //!< @f$k@f$-vector to define section
let secscale = 0.0;          //!< 0.0 (across @f$\Gamma@f$) or 1.0
let axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];      //!< @f$k@f$-vector to define section
let n2d = [];                  //!< Number of line segment
let kv2d = [];          //!< @f$k@f$-vector for 2D plot [::nb][::n2d*2*3]
let clr2d = [];         //!< Matrix element for 2D plot [::nb][::n2d*2*4]
let nbzl2d = 0;                //!< The number of Lines of 1st Brillouin zone
let bzl2d = [];      //!< Lines of 1st BZ [::nbzl2d (max:26)][3]
let bzl2d_proj = []; //!< Lines of 1st BZ [::nbzl2d (max:26)][3], projected into 2D plane
/*
 Equator
*/
let eqvec = [0.0, 0.0, 0.0]; //!<  @f$k@f$-vector for equator
let eqvec_fr = [0.0, 0.0, 0.0]; //!<  @f$k@f$-vector for equator
let nequator = [];             //!< The number of equator
let kveq = [];     //!< @f$k@f$-vector of equator [::nb][::nequator][2][3]
let kveq_rot = []; //!< @f$k@f$-vector of equator [::nb][::nequator*2*3]
/*
  Variables for mouse  & cursorkey
*/
let sx = 0.0;        //!< Scale of mouse movement
let sy = 0.0;        //!< Scale of mouse movement
let cx = 0.0;            //!< Starting point of drug
let cy = 0.0;            //!< Starting point of drug
let scl = 1.0;       //!< Initial scale
let trans = [0.0, 0.0, 0.0];  //!< Translation
let rot = [[1.0, 0.0, 0.0], [0.0, 1.0, 0.0], [0.0, 0.0, 1.0]]; //!< Rotation matrix
let thetax = 0.0;    //!< Rotation angle
let thetay = 0.0;    //!< Rotation angle
let thetaz = 0.0;    //!< Rotation angle
let linewidth = 3.0; //!< BZ/nodal-line/Fermiline width
/*
 Colors
*/
let black = [0.0, 0.0, 0.0, 1.0]; //!< Black color code
let gray = [0.5, 0.5, 0.5, 1.0]; //!< Gray color code
let wgray = [0.9, 0.9, 0.9, 1.0]; //!< Gray color code
let bgray = [0.1, 0.1, 0.1, 1.0]; //!< Gray color code
let cyan = [0.0, 1.0, 1.0, 1.0]; //!< Cyan color code
let magenta = [1.0, 0.0, 1.0, 1.0]; //!< Magenta color code
let yellow = [1.0, 1.0, 0.0, 1.0]; //!< Yellow color code
let red = [1.0, 0.0, 0.0, 1.0]; //!< Red color code
let green = [0.0, 1.0, 0.0, 1.0]; //!< Green color code
let blue = [0.0, 0.0, 1.0, 1.0]; //!< Blue color code
let BackGroundColor = [0.0, 0.0, 0.0, 1.0];//!< BackGround color code
let LineColor = [1.0, 1.0, 1.0, 1.0];//!< Line color code
let BarColor = [[0.0, 0.0, 1.0, 1.0], [0.0, 1.0, 1.0, 1.0],
[0.0, 1.0, 0.0, 1.0], [1.0, 1.0, 0.0, 1.0], [1.0, 0.0, 0.0, 1.0]];
/*
 Others
*/
let corner = [[0.0, 0.0, 0.0, 0.0],
[0.0, 0.0, 0.0, 0.0],
[0.0, 0.0, 0.0, 0.0],
[0.0, 0.0, 0.0, 0.0],
[0.0, 0.0, 0.0, 0.0],
[0.0, 0.0, 0.0, 0.0]]; //!< Corners of tetrahedron
let EF = 0.0;       //!< Fermi energy
/*
Batch mode
*/
let refresh_interpol = 0;
let refresh_patch = 1;
let refresh_color = 1;
let refresh_nodeline = 1;
let refresh_equator = 1;
let refresh_section = 1;
let skip_minmax = 0;
let windowx = 1100;
let windowy = 850;

let gl;
let programInfo;
//
// Start here
//
function main() {
  let canvas = document.querySelector('#glcanvas');

  gl = canvas.getContext('webgl') || canvas.getContext('experimental-webgl');

  // If we don't have a GL context, give up now

  if (!gl) {
    alert('Unable to initialize WebGL. Your browser or machine may not support it.');
    return;
  }

  // Vertex shader program

  let vsSource = `
    attribute vec4 aVertexPosition;
    attribute vec3 aVertexNormal;
    attribute vec4 aVertexColor;

    uniform mat4 uNormalMatrix;
    uniform mat4 uModelViewMatrix;
    uniform mat4 uProjectionMatrix;

    varying lowp vec4 vColor;
    varying highp vec3 vLighting;

    void main(void) {
      gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
      vColor = aVertexColor;

      // Apply lighting effect

      highp vec3 ambientLight = vec3(0.5, 0.5, 0.5);
      highp vec3 directionalLightColor = vec3(1, 1, 1);
      highp vec3 directionalVector = normalize(vec3(-1, -1, -1));

      highp vec4 transformedNormal = uNormalMatrix * vec4(aVertexNormal, 1.0);

      highp float directional = abs(dot(transformedNormal.xyz, directionalVector));
      vLighting = ambientLight + (directionalLightColor * directional);
    }
  `;

  // Fragment shader program

  let fsSource = `
    varying lowp vec4 vColor;
    varying highp vec3 vLighting;

    void main(void) {
      gl_FragColor = vec4(vColor.rgb * vLighting, vColor.a);
    }
  `;

  // Initialize a shader program; this is where all the lighting
  // for the vertices and so forth is established.
  let shaderProgram = initShaderProgram(gl, vsSource, fsSource);

  // Collect all the info needed to use the shader program.
  // Look up which attributes our shader program is using
  // for aVertexPosition, aVevrtexColor and also
  // look up uniform locations.
  programInfo = {
    program: shaderProgram,
    attribLocations: {
      vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
      vertexNormal: gl.getAttribLocation(shaderProgram, 'aVertexNormal'),
      vertexColor: gl.getAttribLocation(shaderProgram, 'aVertexColor'),
    },
    uniformLocations: {
      projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
      modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
      normalMatrix: gl.getUniformLocation(shaderProgram, 'uNormalMatrix'),
    }
  };

  // Here's where we call the routine that builds all the
  // objects we'll be drawing.

  drawScene();

  var el = document.getElementById("glcanvas");
  el.addEventListener("touchstart", touch_start, false);
  el.addEventListener("touchmove", touch_move, false);
  el.addEventListener("mousedown", mouse_down, false);
  el.addEventListener("mousemove", mouse_move, false);
  el.addEventListener("mouseup", mouse_up, false);
  el.addEventListener("wheel", zoom, false);
}
//
// Draw the scene.
//
function drawScene() {
  let ib = 0, ii = 0, jj = 0, icount = 0, itri = 0;

  gl.clearColor(BackGroundColor[0], BackGroundColor[1], BackGroundColor[2], BackGroundColor[3]);  // Clear to black, fully opaque
  gl.clearDepth(1.0);                 // Clear everything
  gl.enable(gl.DEPTH_TEST);           // Enable depth testing
  gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

  // Clear the canvas before we start drawing on it.

  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  // Create a perspective matrix, a special matrix that is
  // used to simulate the distortion of perspective in a camera.
  // Our field of view is 45 degrees, with a width/height
  // ratio that matches the display size of the canvas
  // and we only want to see objects between 0.1 units
  // and 100 units away from the camera.

  let fieldOfView = 45 * Math.PI / 180;   // in radians
  let aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
  let zNear = 0.1;
  let zFar = 100.0;
  let projectionMatrix = mat4.create();

  // note: glmatrix.js always has the first argument
  // as the destination to receive the result.
  mat4.perspective(projectionMatrix,
    fieldOfView,
    aspect,
    zNear,
    zFar);

  // Set the drawing position to the "identity" point, which is
  // the center of the scene.
  let modelViewMatrix = mat4.create();

  // Now move the drawing position a bit to where we want to
  // start drawing the square.

  mat4.translate(modelViewMatrix,     // destination matrix
    modelViewMatrix,     // matrix to translate
    [-0.0, 0.0, -6.0]);  // amount to translate
  mat4.rotate(modelViewMatrix,  // destination matrix
    modelViewMatrix,  // matrix to rotate
    0.0,     // amount to rotate in radians
    [0, 0, 1]);       // axis to rotate around (Z)
  mat4.rotate(modelViewMatrix,  // destination matrix
    modelViewMatrix,  // matrix to rotate
    0.0,// amount to rotate in radians
    [0, 1, 0]);       // axis to rotate around (X)
  let normalMatrix = mat4.create();
  mat4.invert(normalMatrix, modelViewMatrix);
  mat4.transpose(normalMatrix, normalMatrix);

  gl.lineWidth(linewidth);

  {
    let vertexCount = 0;
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] === 1) vertexCount += ntri[ib] * 3;
    }

    let nkvp = vertexCount * 3;
    let positions = new Float32Array(nkvp);
    icount = 0;
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] === 1) {
        for (itri = 0; itri < ntri[ib]; itri++) {
          for (ii = 0; ii < 3; ii++) {
            for (jj = 0; jj < 3; jj++) {
              positions[icount] = (
                      rot[jj][0] * kvp[ib][itri][ii][0]
                      + rot[jj][1] * kvp[ib][itri][ii][1]
                      + rot[jj][2] * kvp[ib][itri][ii][2]) * scl
                  + trans[jj];
              icount += 1;
            }
          }
        }
      }
    }

    let nnmlp = vertexCount * 3;
    let vertexNormals = new Float32Array(nnmlp);
    icount = 0;
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] === 1) {
        for (itri = 0; itri < ntri[ib]; itri++) {
          for (ii = 0; ii < 3; ii++) {
            for (jj = 0; jj < 3; jj++) {
              vertexNormals[icount] = rot[jj][0] * nmlp[ib][itri][ii][0]
                  + rot[jj][1] * nmlp[ib][itri][ii][1]
                  + rot[jj][2] * nmlp[ib][itri][ii][2];
              icount += 1;
            }
          }
        }
      }
    }

    let nclr = vertexCount * 4;
    let colors = new Float32Array(nclr);
    icount = 0;
    for (ib = 0; ib < nb; ib++) {
      if (draw_band[ib] === 1) {
        for (itri = 0; itri < ntri[ib]; itri++) {
          for (ii = 0; ii < 3; ii++) {
            for (jj = 0; jj < 4; jj++) {
              colors[icount] = clr[ib][jj + 4 * ii + 12 * itri];
              icount += 1;
            }
          }
        }
      }
    }

    draw2(projectionMatrix, modelViewMatrix, normalMatrix,
      vertexCount, gl.TRIANGLES,
      positions, colors, vertexNormals);
  }

  {
    let ibzl = 0, i = 0, j = 0, icount = 0;

    let vertexCount = 2*nbzl;
    let nkvp = vertexCount * 3;
    let nnmlp = vertexCount * 3;
    let nclr = vertexCount * 4;
    let positions = new Float32Array(nkvp);
    let colors = new Float32Array(nclr);
    let vertexNormals = new Float32Array(nnmlp);

    //
    //First Brillouin zone mode
    //
    icount = 0;
    for (ibzl = 0; ibzl < nbzl; ++ibzl) {
      for (i = 0; i < 2; ++i) {
        vertexNormals[icount] = -1.0;
        vertexNormals[icount + 1] = -1.0;
        vertexNormals[icount + 2] = -1.0;
        for (j = 0; j < 3; ++j) {
          positions[icount] = (rot[j][0] * bzl[ibzl][i][0]
            + rot[j][1] * bzl[ibzl][i][1]
            + rot[j][2] * bzl[ibzl][i][2])*scl
            + trans[j];
          icount += 1;
        }
      }//for (i = 0; i< 2; ++i)
    }//for (ibzl = 0; ibzl < nbzl; ++ibzl)
    icount = 0;
    for (ibzl = 0; ibzl < nbzl; ++ibzl) {
      for (i = 0; i < 2; ++i) {
        for (j = 0; j < 4; ++j) {
          colors[icount] = LineColor[j];
          icount += 1;
        }
      }
    }
    draw2(projectionMatrix, modelViewMatrix, normalMatrix,
      vertexCount, gl.LINES,
      new Float32Array(positions), new Float32Array(colors), new Float32Array(vertexNormals));
  }
}
function draw2(projectionMatrix, modelViewMatrix, normalMatrix,
  vertexCount, drawtype,
  positions, colors, vertexNormals) {

  let positionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, positions, gl.STATIC_DRAW);

  let colorBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, colors, gl.STATIC_DRAW);

  let normalBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, vertexNormals, gl.STATIC_DRAW);

  // Tell WebGL how to pull out the positions from the position
  // buffer into the vertexPosition attribute
  {
    let numComponents = 3;
    let type = gl.FLOAT;
    let normalize = false;
    let stride = 0;
    let offset = 0;
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    gl.vertexAttribPointer(
      programInfo.attribLocations.vertexPosition,
      numComponents,
      type,
      normalize,
      stride,
      offset);
    gl.enableVertexAttribArray(
      programInfo.attribLocations.vertexPosition);
  }

  // Tell WebGL how to pull out the colors from the color buffer
  // into the vertexColor attribute.
  {
    let numComponents = 4;
    let type = gl.FLOAT;
    let normalize = false;
    let stride = 0;
    let offset = 0;
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.vertexAttribPointer(
      programInfo.attribLocations.vertexColor,
      numComponents,
      type,
      normalize,
      stride,
      offset);
    gl.enableVertexAttribArray(
      programInfo.attribLocations.vertexColor);
  }

  // Tell WebGL how to pull out the normals from
  // the normal buffer into the vertexNormal attribute.
  {
    let numComponents = 3;
    let type = gl.FLOAT;
    let normalize = false;
    let stride = 0;
    let offset = 0;
    gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
    gl.vertexAttribPointer(
      programInfo.attribLocations.vertexNormal,
      numComponents,
      type,
      normalize,
      stride,
      offset);
    gl.enableVertexAttribArray(
      programInfo.attribLocations.vertexNormal);
  }

  // Tell WebGL to use our program when drawing

  gl.useProgram(programInfo.program);

  // Set the shader uniforms

  gl.uniformMatrix4fv(
    programInfo.uniformLocations.projectionMatrix,
    false,
    projectionMatrix);
  gl.uniformMatrix4fv(
    programInfo.uniformLocations.modelViewMatrix,
    false,
    modelViewMatrix);
  gl.uniformMatrix4fv(
    programInfo.uniformLocations.normalMatrix,
    false,
    normalMatrix);

  {
    let offset = 0;
    gl.drawArrays(drawtype, offset, vertexCount);
  }
}
//
// Initialize a shader program, so WebGL knows how to draw our data
//
function initShaderProgram(gl, vsSource, fsSource) {
  let vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
  let fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

  // Create the shader program

  let shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  // If creating the shader program failed, alert

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
    return null;
  }

  return shaderProgram;
}
//
// creates a shader of the given type, uploads the source and
// compiles it.
//
function loadShader(gl, type, source) {
  let shader = gl.createShader(type);

  // Send the source to the shader object

  gl.shaderSource(shader, source);

  // Compile the shader program

  gl.compileShader(shader);

  // See if it compiled successfully

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}

let touch0x = 0.0;
let touch0y = 0.0;
let touch1x = 0.0;
let touch1y = 0.0;

function touch_start(evt) {
  evt.preventDefault();
  var touches = evt.changedTouches;

  for (i = 0; i < touches.length; i++) {
    if (touches[i].identifier === 0) {
      touch0x = touches[i].clientX;
      touch0y = touches[i].clientY;
    }
    else if (touches[i].identifier === 1) {
      touch1x = touches[i].clientX;
      touch1y = touches[i].clientY;
    }
  }
}

function touch_move(evt) {
  evt.preventDefault();
  var touches = evt.changedTouches;
  let dx = 0.0, dy = 0.0, dold = 0.0, dnew = 0.;

  if (touches.length === 1) {
    if (touches[0].identifier === 0) {
      dx = 0.001 * (touches[0].clientX - touch0x);
      dy = 0.001 * (touches[0].clientY - touch0y);
      mouserotation(dx, dy)
      drawScene();
      touch0x = touches[0].clientX;
      touch0y = touches[0].clientY;
    }
  }
  else if (touches.length === 2) {
    dold = Math.sqrt((touch1x - touch0x) * (touch1x - touch0x) + (touch1y - touch0y) * (touch1y - touch0y));
    touch0x = touches[0].clientX;
    touch0y = touches[0].clientY;
    touch1x = touches[1].clientX;
    touch1y = touches[1].clientY;
    dnew = Math.sqrt((touch1x - touch0x) * (touch1x - touch0x) + (touch1y - touch0y) * (touch1y - touch0y));
    scl += 0.001 * (dnew - dold);
    document.getElementById("scale").value = String(scl);
    drawScene();
  }
}

let isDrawing = false;

function mouse_down(evt){
  touch0x = evt.offsetX;
  touch0y = evt.offsetY;
  isDrawing = true;
}

function mouse_move(evt){
  if (isDrawing === true) {
    let dx = evt.offsetX - touch0x;
    let dy = evt.offsetY - touch0y;
    dx *= 0.001;
    dy *= 0.001;
    mouserotation(dx, dy)
    drawScene();
    touch0x = evt.offsetX;
    touch0y = evt.offsetY;
  }
}

function mouse_up(evt){
  if (isDrawing === true) {
    let dx = evt.offsetX - touch0x;
    let dy = evt.offsetY - touch0y;
    dx *= 0.001;
    dy *= 0.001;
    mouserotation(dx, dy)
    drawScene();
    touch0x = 0;
    touch0y = 0;
    isDrawing = false;
  }
}

function zoom(evt) {
  scl -= evt.deltaY*0.001;
  document.getElementById("scale").value = String(scl);
  drawScene();
}

function mouserotation(dx, dy) {
  let i = 0, j = 0;
  let a = Math.sqrt(dx * dx + dy * dy);
  let rot0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];
  let rot1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  if (a !== 0.0) {
    //
    // Compute rotational matrix from translation of mousepointer
    //
    let ax = -dy;
    let ay = dx;
    //
    a = a * 10.0;
    //
    rot0[0][0] = (ax * ax + ay * ay * Math.cos(a)) / (ax * ax + ay * ay);
    rot0[0][1] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
    rot0[0][2] = ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
    rot0[1][0] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
    rot0[1][1] = (ax * ax * Math.cos(a) + ay * ay) / (ax * ax + ay * ay);
    rot0[1][2] = ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
    rot0[2][0] = -ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
    rot0[2][1] = -ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
    rot0[2][2] = Math.cos(a);
    //
    for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) rot1[i][j] = rot[i][j];
    //
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
        rot[i][j] = rot0[i][0] * rot1[0][j]
                  + rot0[i][1] * rot1[1][j]
                  + rot0[i][2] * rot1[2][j];
      }
    }
    //
    //Print angle to text Box
    //
    thetay = Math.asin(rot[0][2]);
    if (Math.cos(thetay) != 0.0) {
      if (-rot[1][2] / Math.cos(thetay) >= 0.0) thetax = Math.acos(rot[2][2] / Math.cos(thetay));
      else thetax = 2.0 * Math.PI - Math.acos(rot[2][2] / Math.cos(thetay));
      if (-rot[0][1] / Math.cos(thetay) >= 0.0) thetaz = Math.acos(rot[0][0] / Math.cos(thetay));
      else thetaz = 2.0 * Math.PI - Math.acos(rot[0][0] / Math.cos(thetay));
    }
    else {
      thetax = 0.0;
      if (rot[1][0] >= 0.0) thetaz = Math.acos(rot[1][1]);
      else thetaz = 2.0 * Math.PI - Math.acos(rot[1][1]);
    }
    thetax *= 180.0 / Math.PI;
    thetay *= 180.0 / Math.PI;
    thetaz *= 180.0 / Math.PI;
    document.getElementById("rotatex").value = String(thetax);
    document.getElementById("rotatey").value = String(thetay);
    document.getElementById("rotatez").value = String(thetaz);
  }
}

function terminal(msg) {
  var p = document.getElementById('log');
  p.innerHTML += msg;
}
/**@file
@brief Mathematical operations used in various step
*/
/**
 @brief Work as Modulo function of fortran
 @return Modulated value
*/
function modulo(
  i = 0, //!< [in]
  n = 0 //!< [in]
) {
  let j = 0;
  j = (i + 100 * n) % n;
  return j;
};/*modulo(let i, let n)*/
/**
 @brief Solve linear system
 @return Determinant
*/
function solve3(
  a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!< [in] Matix
  b = [0.0, 0.0, 0.0] //!< [in,out] Right hand side vector
)
{
  let i = 0;
  let det = 0.0, c = [0.0, 0.0, 0.0];
  /**/
  det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
      + a[0][1] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
      + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
  /**/
  c[0] = b[0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1])
       + b[1] * (a[0][2] * a[2][1] - a[0][1] * a[2][2])
       + b[2] * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);
  /**/
  c[1] = b[0] * (a[1][2] * a[2][0] - a[1][0] * a[2][2])
       + b[1] * (a[0][0] * a[2][2] - a[0][2] * a[2][0])
       + b[2] * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);
  /**/
  c[2] = b[0] * (a[1][0] * a[2][1] - a[1][1] * a[2][0])
       + b[1] * (a[0][1] * a[2][0] - a[0][0] * a[2][1])
       + b[2] * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
  /**/
  for (i = 0; i<3; ++i) b[i] = c[i] / det;
  return det;
}/*let solve3*/
/**
 @brief Simple sort
*/
function eigsort(
  n = 0, //!< [in] the number of components
  key = [0.0], //!< [in] Variables to be sorted [n].
  swap = [0] //!< [out] Order of index (sorted)
)
{
  let i = 0, j = 0, k = 0;

  for (i = 0; i < n; ++i) swap[i] = i;

  for (i = 0; i < n - 1; ++i) {
    for (j = i + 1; j < n; ++j) {
      if (key[swap[j]] < key[swap[i]]) {
        /*
         Swap
        */
        k = swap[j];
        swap[j] = swap[i];
        swap[i] = k;
      }/*if (sortee[j][0] < sortee[i][0])*/
    }/*for (j = i + 1; j < n; ++j)*/
  }/*for (i = 0; i < n - 1; ++i)*/
}/*eigsort*/
/**
 @brief Calculate normal vector from corners of triangle
*/
function normal_vec(
  in1 = [0.0, 0.0, 0.0], //!< [in] Corner 1
  in2 = [0.0, 0.0, 0.0], //!< [in] Corner 2
  in3 = [0.0, 0.0, 0.0], //!< [in] Corner 3
  out = [0.0, 0.0, 0.0] //!< [out] The normal vector
)
{
  let i = 0;
  let norm = 0.0;
  out[0] = in1[1] * in2[2] - in1[2] * in2[1]
         + in2[1] * in3[2] - in2[2] * in3[1]
         + in3[1] * in1[2] - in3[2] * in1[1];
  out[1] = in1[2] * in2[0] - in1[0] * in2[2]
         + in2[2] * in3[0] - in2[0] * in3[2]
         + in3[2] * in1[0] - in3[0] * in1[2];
  out[2] = in1[0] * in2[1] - in1[1] * in2[0]
         + in2[0] * in3[1] - in2[1] * in3[0]
         + in3[0] * in1[1] - in3[1] * in1[0];
  norm = Math.sqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
  for (i = 0; i<3; i++) out[i] = out[i] / norm;
} /* normal_vec */
/**
 @brief Judge wheser this line is the edge of 1st BZ
*/
function bragg_vert(
  bragg = [[0.0]],
  brnrm = [0.0],
  ibr = 0, //!< [in] Index of a Bragg plane
  jbr = 0, //!< [in] Index of a Bragg plane
  nbr = 0, //!< [in]
  vert = [0.0], //!< [in] start point of line
  vert2 = [0.0]//!< [in] end point of line
)
{
  let kbr = 0, i = 0, lbr = 0, nbr0 = 0;
  let bmat = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], rhs = [0.0, 0.0, 0.0], prod, thr, det;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < 26; ++kbr) {
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = bragg[ibr][i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = brnrm[ibr];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = Math.sqrt(rhs[0] * rhs[1] * rhs[2]) * 0.001;
    /*
     if Bragg planes do not cross, roop next kbr
    */
    det = solve3(bmat, rhs);
    if (Math.abs(det) < thr) continue;
    /*
     if vert0 = vert1, roop next kbr
    */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
         + (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
         + (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if (prod < thr) continue;
    /*
     is corner really in 1st BZ ?
    */
    i = 0;
    for (lbr = 0; lbr < 26; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }
  /*
   this line is not a BZ boundary
  */
  return 0;
}/* bragg_vert */
/**
 @brief Compute real number of Bragg plane at 1st BZ
*/
function check_bragg()
{
  let ibr = 0, ibzl = 0, ibzc = 0;
  let ii = 0, kk = 0, bzflag = 0, nbzcorner = 0, nn = 0;
  let thr = 0.0001, prod = 0.0, bzc = []; //[676][3];
  /*
  First, compute real number of corners of 1st BZ
  */
  nbzcorner = 0;
  for (ibzl = 0; ibzl < nbzl; ibzl++) {
    for (ii = 0; ii < 2; ii++) {
      bzflag = 0;
      
      for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
        prod = 0.0;
        for (kk = 0; kk < 3; kk++) prod += (bzl[ibzl][ii][kk] - bzc[ibzc][kk]) * (bzl[ibzl][ii][kk] - bzc[ibzc][kk]);
        if (prod < thr) bzflag = 1;
      }

      if (bzflag == 0) {
        bzc.push([0.0, 0.0, 0.0]);
        for (kk = 0; kk < 3; kk++) bzc[nbzcorner][kk] = bzl[ibzl][ii][kk];
        nbzcorner += 1;
      }

    }/*for (ii = 0; ii < 2; ii++)*/
  }/*for (ibzl = 0; ibzl < nbzl; ibzl++)*/
  terminal("    Number of corners of 1st BZ : " + String(nbzcorner) + "\n");
  /**@brief
  Then, compute real number Bragg plane of 1st BZ (::nbragg), 
  Re-order ::bragg and ::brnrm
  */
  nbragg = 0;
  for (ibr = 0; ibr < 26; ibr++) {
    nn = 0;

    for (ibzc = 0; ibzc < nbzcorner; ibzc++) {
      prod = bragg[ibr][0] * bzc[ibzc][0] + bragg[ibr][1] * bzc[ibzc][1] + bragg[ibr][2] * bzc[ibzc][2];
      if (Math.abs(prod - brnrm[ibr]) < thr) nn += 1;
    }
    if (nn >= 3) {
      for (kk = 0; kk < 3; kk++) bragg[nbragg][kk] = bragg[ibr][kk];
      brnrm[nbragg] = brnrm[ibr];
      nbragg += 1;
    }
  }
  terminal("    Number of plane of 1st BZ : " + String(nbragg) + "\n");
}/*function check_bragg*/
/**
 @brief Compute Brillouin zone boundariy lines

 Modify : ::nbzl, ::bzl
*/
function bz_lines()
{
  /**/
  let ibr = 0, jbr = 0, nbr = 0, i = 0, j = 0, lvert = 0;
  let vert = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];
  /**/
  nbzl = 0;
  bzl = [];
  /**/
  for (ibr = 0; ibr < 26; ++ibr) {
    for (jbr = 0; jbr < 26; ++jbr) {
      /**/
      for (i = 0; i<3; ++i) vert[1][i] = 0.0;
      nbr = 0;
      lvert = bragg_vert(bragg, brnrm, ibr, jbr, nbr, vert[0], vert[1]);
      if (lvert == 0) continue;
      nbr = lvert;
      /**/
      lvert = bragg_vert(bragg, brnrm, ibr, jbr, nbr, vert[1], vert[0]);
      if (lvert == 0) continue;
      /**/
      bzl.push([]);
      for (i = 0; i < 2; ++i) {
        bzl[nbzl].push([]);
        for (j = 0; j < 3; ++j) bzl[nbzl][i].push(vert[i][j]);
      }
      nbzl += 1;

    }/*for (jbr = 0; jbr < 26; ++jbr)*/
  }/*for (ibr = 0; ibr < 26; ++ibr)*/

  check_bragg();

}/*bz_lines*/
/**
@brief Compute node-line where \f$\Delta_{n k} = 0\f$

 Modify : ::nnl, ::kvnl, ::kvnl_rot

*/
function calc_nodeline()
{
  let ib = 0, itri = 0, i = 0, j = 0;
  let sw = [0, 0, 0];
  let a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], matps = [0.0, 0.0, 0.0];
  let kvnl0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]; // std::vector<std::vector<let> > 

  terminal("    band   # of nodeline\n");
  for (ib = 0; ib < nb; ib++) {

    kvnl.push([]);

    for (itri = 0; itri < ntri[ib]; ++itri) {


      for (i = 0; i < 3; ++i) matps[i] = matp[ib][itri][i][0];
      eigsort(3, matps, sw);

      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (0.0 - matp[ib][itri][sw[j]][0]) 
            / (matp[ib][itri][sw[i]][0] - matp[ib][itri][sw[j]][0]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/

      if ((matp[ib][itri][sw[0]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[1]][0])
        || (matp[ib][itri][sw[0]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[1]][0])) {
        for (i = 0; i < 3; ++i) {
          kvnl0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
          kvnl0[1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        kvnl[ib].push(kvnl0);
      }/*else if (matp[ib][itri][sw[0]] < 0.0 && 0.0 <= matp[ib][itri][sw[1]])*/
      else if ((matp[ib][itri][sw[1]][0] < 0.0 && 0.0 <= matp[ib][itri][sw[2]][0])
        || (matp[ib][itri][sw[1]][0] <= 0.0 && 0.0 < matp[ib][itri][sw[2]][0])) {
        for (i = 0; i < 3; ++i) {
          kvnl0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          kvnl0[1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
        }/*for (i = 0; i < 3; ++i)*/
        kvnl[ib].push(kvnl0);
      }/*else if (matp[ib][itri][sw[1]] < 0.0 && 0.0 <= matp[ib][itri][sw[2]])*/
    }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    /*
     Allocation of nodeline
     */
    nnl[ib] = kvnl[ib].length;
    terminal("    " + String(ib + 1) + "       " + String(nnl[ib]) + "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function calc_nodeline()*/
/**
 @brief Draw Fermi surfaces

 Modify : ::nmlp_rot, ::kvp_rot, ::kvnl_rot

 First, rotate ::nmlp and ::kvp with OpenMP then draw them.
 Also draw nodeline in the same way.
*/
function draw_fermi() {
  let ib = 0, a0 = 0, a1 = 0, a2 = 0, ia = 0;
  let kshift = [0.0, 0.0, 0.0];
  let i = 0, j = 0, l = 0, itri = 0;
  /*
   First, rotate k-vector and normal vector
  */
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;

        for (ib = 0; ib < nb; ib++) {
          if (draw_band[ib] == 1) {
            for (itri = 0; itri < ntri[ib]; ++itri) {
              for (i = 0; i < 3; ++i) {
                for (j = 0; j < 3; ++j) {
                  kvp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * (kvp[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvp[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvp[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  nmlp_rot[ib][j + 3 * i + 9 * itri]
                      = rot[j][0] * nmlp[ib][itri][i][0]
                      + rot[j][1] * nmlp[ib][itri][i][1]
                      + rot[j][2] * nmlp[ib][itri][i][2];
                  nmlp_rot[ib][j + 3 * i + 9 * itri] *= side;
                  for (l = 0; l < 2; ++l) {
                      arw_rot[ib][j + 3 * l + 6 * i + 18 * itri]
                        = rot[j][0] * (arw[ib][itri][i][l][0] + kshift[0])
                        + rot[j][1] * (arw[ib][itri][i][l][1] + kshift[1])
                        + rot[j][2] * (arw[ib][itri][i][l][2] + kshift[2])
                        + trans[j];
                  }
                }
              }/*for (i = 0; i < 3; ++i)*/
            }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
          }/*if (draw_band[ib] == 1)*/
        }/*for (ib = 0; ib < nb; ib++)*/
        /*
          Second, draw each triangle
        */
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        for (ib = 0; ib < nb; ib++) {
          if (draw_band[ib] == 1) {
            glVertexPointer(3, GL_FLOAT, 0, kvp_rot[ib]);
            glNormalPointer(GL_FLOAT, 0, nmlp_rot[ib]);
            glColorPointer(4, GL_FLOAT, 0, clr[ib]);
            glDrawArrays(GL_TRIANGLES, 0, ntri[ib] * 3);
          }/*if (draw_band[ib] == 1)*/
        }/*for (ib = 0; ib < nb; ib++)*/
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        /*
        Arrow for 3D value
        */
        glNormal3f(0.0, 0.0, 1.0);
        if (color_scale == 3) {
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glColor3f(1.0 - clr[ib][0], 1.0 - clr[ib][1], 1.0 - clr[ib][2]);
              glVertexPointer(3, GL_FLOAT, 0, arw_rot[ib]);
              glDrawArrays(GL_LINES, 0, ntri[ib] * 3 * 2);
            }
          }
        }
        /*
         Nodeline
        */
        if (nodeline == 1) {
          /*
           First, rotate k-vector
          */
          let i, j, itri;
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              for (itri = 0; itri < nnl[ib]; ++itri) {
                for (i = 0; i < 2; ++i) {
                  for (j = 0; j < 3; ++j)
                    kvnl_rot[ib][j + 3 * i + 6 * itri]
                      = rot[j][0] * (kvnl[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kvnl[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kvnl[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  kvnl_rot[ib][2 + 3 * i + 6 * itri] += 0.001;
                }/*for (i = 0; i < 2; ++i)*/
              }/*for (itri = 0; itri < nnl[ib]; ++itri)*/
            }/*if (draw_band[ib] == 1)*/
          }/*for (ib = 0; ib < nb; ib++)*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0, 0.0, 1.0);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kvnl_rot[ib]);
              glDrawArrays(GL_LINES, 0, 2 * nnl[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
        /*
        Equator
        */
        if (lequator == 1) {
          /*
          First, rotate k-vector
          */
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              for (itri = 0; itri < nequator[ib]; ++itri) {
                for (i = 0; i < 2; ++i) {
                  for (j = 0; j < 3; ++j)
                    kveq_rot[ib][j + 3 * i + 6 * itri]
                      = rot[j][0] * (kveq[ib][itri][i][0] + kshift[0])
                      + rot[j][1] * (kveq[ib][itri][i][1] + kshift[1])
                      + rot[j][2] * (kveq[ib][itri][i][2] + kshift[2])
                      + trans[j];
                  kveq_rot[ib][2 + 3 * i + 6 * itri] += 0.001;
                }/*for (i = 0; i < 2; ++i)*/
              }/*for (itri = 0; itri < nequator[ib]; ++itri)*/
            }/*if (draw_band[ib] == 1)*/
          }/*for (ib = 0; ib < nb; ib++)*/
          /*
          Second, draw each lines
          */
          glColor3fv(black);
          glNormal3f(0.0, 0.0, 1.0);
          for (ib = 0; ib < nb; ib++) {
            if (draw_band[ib] == 1) {
              glVertexPointer(3, GL_FLOAT, 0, kveq_rot[ib]);
              glDrawArrays(GL_LINES, 0, 2 * nequator[ib]);
            }/*if (draw_band[ib] == 1)*/
          }/* for (ib = 0; ib < nb; ib++)*/
        }/*if (nodeline == 1)*/
      }
    }
  }
}/*function draw_fermi*/
/**
 @brief Draw lines of BZ boundaries
*/
function draw_bz_lines() {
  let ibzl = 0, i = 0, j = 0, a0 = 0, a1 = 0, a2 = 0, ia = 0;
  let bzl2 = [0.0, 0.0, 0.0], bvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    linecolor = [0.0, 0.0, 0.0, 0.0], secvec2 = [0.0, 0.0, 0.0], kshift = [0.0, 0.0, 0.0];
  let vertices = [];
  /*
   Line color is oposit of BG color
  */
  for (i = 0; i < 4; i++) linecolor[i] = LineColor[i];
  /**/
  glLineWidth(linewidth);
  for (a0 = 0; a0 < BZ_number[0]; a0++) {
    for (a1 = 0; a1 < BZ_number[1]; a1++) {
      for (a2 = 0; a2 < BZ_number[2]; a2++) {
        for (ia = 0; ia < 3; ia++) kshift[ia] = bvec[0][ia] * a0 + bvec[1][ia] * a1 + bvec[2][ia] * a2;
        /*
        First Brillouin zone mode
        */
        if (fbz == 1) {
          for (ibzl = 0; ibzl < nbzl; ++ibzl) {
            for (i = 0; i < 2; ++i) {
              for (j = 0; j < 3; ++j)
                bzl2[j] = rot[j][0] * (bzl[ibzl][i][0] + kshift[0])
                        + rot[j][1] * (bzl[ibzl][i][1] + kshift[1])
                        + rot[j][2] * (bzl[ibzl][i][2] + kshift[2])
                        + trans[j];
              for (j = 0; j < 3; j++) vertices.push(bzl2[j]);
            }/*for (i = 0; i< 2; ++i)*/
            glColor3fv(linecolor);
            glNormal3f(0.0, 0.0, 1.0);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_LINES, 0, 2);
          }/*for (ibzl = 0; ibzl < nbzl; ++ibzl)*/
        }/*if (fbz == 1)*/
        else {
          /*
           Premitive BZ mode
          */
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
              bvec2[i][j] = rot[j][0] * bvec[i][0]
                          + rot[j][1] * bvec[i][1]
                          + rot[j][2] * bvec[i][2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i] + bvec2[2][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[1][i]);
          for (i = 0; i < 3; ++i) vertices.push(trans[i] + bvec2[0][i] + bvec2[1][i]);
          for (i = 0; i < 17; ++i) {
            for (j = 0; j < 3; ++j) {
              vertices[j + 3 * i] = vertices[j + 3 * i]
                + rot[j][0] * kshift[0]
                + rot[j][1] * kshift[1]
                + rot[j][2] * kshift[2];
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
          glColor3fv(linecolor);
          glNormal3f(0.0, 0.0, 1.0);
          glVertexPointer(3, GL_FLOAT, 0, vertices);
          glDrawArrays(GL_LINE_STRIP, 0, 17);
        }/*if (fbz != 1)*/
      }
    }
  }
  /*
  Section for the 2D Fermi line
  */
  if (lsection == 1 && fbz == 1) {
    for (j = 0; j < 3; ++j)
      secvec2[j] = rot[j][0] * secvec[0]
                 + rot[j][1] * secvec[1]
                 + rot[j][2] * secvec[2];
    for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
      for (j = 0; j < 3; ++j)
        bzl2[j] = rot[j][0] * bzl2d[ibzl][0]
                + rot[j][1] * bzl2d[ibzl][1]
                + rot[j][2] * bzl2d[ibzl][2]
                + trans[j];
      for (j = 0; j < 3; j++)vertices[j + 3 * ibzl] = bzl2[j];
    }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
    glColor3fv(gray);
    glNormal3fv(secvec2);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nbzl2d);
  }/*if (lsection == 1)*/
}/*draw bz_lines */
/**
 @brief Draw color-bar or colr-circle (periodic) 
        as a color scale
*/
function draw_colorbar()
{
  let i = 0, j = 0, k = 0;
  let mat2 = 0.0, vertices = [], colors = [], vector = [], vector_color = [],
    norm = 0.0;

  glEnableClientState(GL_COLOR_ARRAY);
  /*
  Reciplocal lattice vectors
  */
  for (i = 0; i < 6; i++) {
    vector[3 * i] = -1.2;
    vector[3 * i + 1] = -1.05;
    vector[3 * i + 2] = 0.0;
  }
  for (i = 0; i < 3; i++) {
    norm = Math.sqrt(bvec[i][0] * bvec[i][0]+ bvec[i][1] * bvec[i][1]+ bvec[i][2] * bvec[i][2]);
    for (j = 0; j < 3; j++) {
      vector[j + 6 * i] 
        += (rot[j][0] * bvec[i][0]
        + rot[j][1] * bvec[i][1]
        + rot[j][2] * bvec[i][2]) * 0.2 / norm;
    }
    for (j = 0; j < 4; j++) {
      vector_color[j + 8 * i] = BarColor[i * 2][j];
      vector_color[j + 8 * i + 4] = BarColor[i * 2][j];
    }
  }
  glNormal3f(0.0, 0.0, 1.0);
  glVertexPointer(3, GL_FLOAT, 0, vector);
  glColorPointer(4, GL_FLOAT, 0, vector_color);
  glDrawArrays(GL_LINES, 0, 6);
  /*
   Color bar/circle/cube
  */
  if (color_scale == 1 || color_scale == 4) {
    for (i = 0; i < 5; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0 + 0.5*i;
        vertices[1 + j * 3 + i * 6] = -1.0 - 0.1*j;
        vertices[2 + j * 3 + i * 6] = 0.0;
        for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = BarColor[i][k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
  }/*if (color_scale == 1 || color_scale == 4)*/
  else if (color_scale == 2) {
    /*
     Periodic color scale
    */
    vertices[0] = 0.0;
    vertices[1] = -1.0;
    vertices[2] = 0.0;
    for (j = 0; j < 4; j++) colors[j] = 1.0 - BackGroundColor[j];
    /**/
    for (i = 0; i <= 60; i++) {
      /**/
      mat2 = i / 60.0 * 6.0;
      /**/
      if (mat2 <= 1.0) {
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = yellow[j] * mat2 + red[j] * (1.0 - mat2);
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = green[j] * mat2 + yellow[j] * (1.0 - mat2);
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = cyan[j] * mat2 + green[j] * (1.0 - mat2);
      }
      else if (mat2 <= 4.0) {
        mat2 = mat2 - 3.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = blue[j] * mat2 + cyan[j] * (1.0 - mat2);
      }
      else if (mat2 <= 5.0) {
        mat2 = mat2 - 4.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = magenta[j] * mat2 + blue[j] * (1.0 - mat2);
      }
      else {
        mat2 = mat2 - 5.0;
        for (j = 0; j<4; ++j) colors[j + 4 * (i + 1)] = red[j] * mat2 + magenta[j] * (1.0 - mat2);
      }
      /**/
      vertices[0 + 3 * (i + 1)] = 0.2 * Math.cos(i / 60.0 * 6.283185307);
      vertices[1 + 3 * (i + 1)] = 0.2 * Math.sin(i / 60.0 * 6.283185307) - 1.0;
      vertices[2 + 3 * (i + 1)] = 0.0;
    }/*for (i = 0; i <= 60; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 62);
  }/*else if (color_scale == 2)*/
  else  if (color_scale == 6 || color_scale == 7) {
    for (i = 0; i < 2; i++) {
      for (j = 0; j < 2; j++) {
        vertices[0 + j * 3 + i * 6] = -1.0 + 2.0*i;
        vertices[1 + j * 3 + i * 6] = -1.0 - 0.1*j;
        vertices[2 + j * 3 + i * 6] = 0.0;
        if (i == 0) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = bgray[k];
        else if (i == 1) for (k = 0; k < 4; k++) colors[k + 4 * j + 8 * i] = wgray[k];
      }
    }/*for (i = 0; i < 10; i++)*/
    glNormal3f(0.0, 0.0, 1.0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }/*if (color_scale == 6 || color_scale == 7)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*function draw_colorbar*/
/**
 @brief Draw eye-points for the stereogram
*/
function draw_circles(
  dx2d = 0.0 //!< [in] Translation used for the section-mode
) {
  let i = 0, j = 0;
  let r = 0.0, vertices = [];
  /**/
  r = 0.05;
  /**/
  vertices[0] = 0.7 - dx2d;
  vertices[1] = scl;
  vertices[2] = 0.0;
  for (i = 0; i <= 20; i++) {
    vertices[0 + (i + 1) * 3] = r * Math.cos(i / 20.0 * 6.283185307) + 0.7 - dx2d;
    vertices[1 + (i + 1) * 3] = r * Math.sin(i / 20.0 * 6.283185307) + scl;
    vertices[2 + (i + 1) * 3] = 0.0;
  }/*for (i = 0; i <= 20; i++)*/
  glNormal3f(0.0, 0.0, 1.0);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
  /**/
  for (i = 0; i < 22; i++) vertices[3 * i] += -1.4;
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}/*function draw_circles*/
/**
 @brief Draw 2D Fermi lines
*/
function draw_fermi_line() {
  let i = 0, ib = 0, ibzl = 0;
  let vertices = [];
  /*
   Draw 2D BZ lines
  */
  glLineWidth(linewidth);
  for (ibzl = 0; ibzl < nbzl2d; ++ibzl) {
    for (i = 0; i < 3; i++) vertices[i + 3 * ibzl] = bzl2d_proj[ibzl][i];
  }/*for (ibzl = 0; ibzl < nbzl2d; ++ibzl)*/
  glNormal3f(0.0, 0.0, 1.0);
  glColor3fv(LineColor);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_LINE_LOOP, 0, nbzl2d);
  /*
   Draw Fermi lines
  */
  glLineWidth(linewidth);
  glEnableClientState(GL_COLOR_ARRAY);
  glNormal3f(0.0, 0.0, 1.0);
  for (ib = 0; ib < nb; ib++) {
    if (draw_band[ib] == 1) {
      glVertexPointer(3, GL_FLOAT, 0, kv2d[ib]);
      glColorPointer(4, GL_FLOAT, 0, clr2d[ib]);
      glDrawArrays(GL_LINES, 0, 2 * n2d[ib]);
    }/*if (draw_band[ib] == 1)*/
  }/* for (ib = 0; ib < nb; ib++)*/
  glDisableClientState(GL_COLOR_ARRAY);
}/*function draw_fermi_line*/
/**
 @brief Glut Display function
 called by glutDisplayFunc
*/
function draw()
{
  let pos = [1.0, 1.0, 1.0, 0.0];
  let amb = [0.2, 0.2, 0.2, 0.0];
  let dx = 0.0, dx2d = 0.0, theta = 0.0, posz = 0.0, phi = 0.0;
  let pos1 = [0.0, 0.0, 0.0, 0.0], pos2 = [0.0, 0.0, 0.0, 0.0];

  if (draw_band == NULL) return;

  if (lstereo == 2) {
    /*
     Parallel eyes
    */
    theta = 3.14159265359 / 180.0 * 2.5;
    posz = 5.0;
    dx = 0.7;
    phi = Math.atan(posz / dx) - theta;
    theta = (3.1416 * 0.5 - phi) / 3.1416 * 180.0;
    /**/
    pos1[0] = posz * Math.cos(phi) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * Math.sin(phi);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * Math.cos(phi) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * Math.sin(phi);
    pos2[3] = 1.0;
  }/*if (lstereo == 2)*/
  else if (lstereo == 3) {
    /*
     Cross eyes
    */
    theta = -3.1416 / 180.0 * 2.0;
    posz = 5.0;
    dx = -0.7;
    /**/
    pos1[0] = posz * Math.sin(theta) - dx;
    pos1[1] = 0.0;
    pos1[2] = posz * Math.cos(theta);
    pos1[3] = 1.0;
    /**/
    pos2[0] = -posz * Math.sin(theta) + dx;
    pos2[1] = 0.0;
    pos2[2] = posz * Math.cos(theta);
    pos2[3] = 1.0;
  }/*if (lstereo == 3)*/
  else {
    theta = 0.0;
    dx = 0.0;
  }/*lstero == 1*/
  if (lsection == 1 && fbz == 1) dx2d = 0.7;
  else dx2d = 0.0;
  /*
   Initialize
  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /*
   Set view point & view line
  */
  glTranslatef(0.0, 0.0, -5.0);
  //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /*
   Set position of light
  */
  if (lstereo == 1) {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glTranslatef(-dx2d, 0.0, 0.0);
    /*
     Draw color scale
    */
    if (lcolorbar == 1) draw_colorbar();
  }
  else {
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    draw_circles(dx2d);
    glTranslatef(-dx-dx2d, 0.0, 0.0);
    glRotatef(theta, 0.0, 1.0, 0.0);
  }
  glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
  /*
   Rotation & Zoom
  */
  glScalef(scl, scl, scl);
  /*
   Draw Brillouin zone boundaries
  */
  draw_bz_lines();
  /*
   Draw Fermi surfaces
  */
  draw_fermi();
  /*
   Draw the second object for stereogram
  */
  if (lstereo != 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos2);
    /**/
    glTranslatef(dx-dx2d, 0.0, 0.0);
    glRotatef(-theta, 0.0, 1.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_bz_lines();
    draw_fermi();
    /**/
    glPopMatrix();
  }
  /*
   Draw 2D Fermi line
  */
  if (lsection == 1 && fbz == 1) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    //gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    /**/
    if (lstereo == 1) glTranslatef(dx2d, 0.0, 0.0);
    else glTranslatef(2.0 * dx2d, 0.0, 0.0);
    /**/
    glScalef(scl, scl, scl);
    draw_fermi_line();
    /**/
    glPopMatrix();
  }/*if (lsection == 1)*/
  glFlush(); // Not really necessary: buffer swapping below implies glFlush()
  SwapBuffers();
}/*function display*/
/**
@brief Compute equator \f$\{\bf v}_{n k} \cdot {\bf k} = 0\f$

 Modify : ::nequator, ::kveq, ::kveq_rot
*/
function equator() {
  let ib = 0, itri = 0, i = 0, j = 0;
  let sw = [0,0,0];
  let a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], prod = [0.0, 0.0, 0.0];
  let kveq_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  kveq = [];
  nequator = [];
  terminal("    band   # of equator\n");
  for (ib = 0; ib < nb; ib++) {
    kveq.push([]);

    for (itri = 0; itri < ntri[ib]; ++itri) {
      for (i = 0; i < 3; ++i) {
        prod[i] = 0.0;
        for (j = 0; j < 3; ++j) prod[i] += eqvec[j] * nmlp[ib][itri][i][j];
      }/*for (i = 0; i < 3; ++i)*/

      eigsort(3, prod, sw);
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            a[i][j] = (0.0 - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
          }/*for (j = 0; j < 3; ++j)*/
        }/*for (i = 0; i < 3; ++i)*/

        if ((prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])
          || (prod[sw[0]] <= 0.0 && 0.0 < prod[sw[1]])) {
          for (i = 0; i < 3; ++i) {
            kveq_0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][1] + kvp[ib][itri][sw[1]][i] * a[1][0];
            kveq_0[1][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
          }/*for (i = 0; i < 3; ++i)*/
          kveq[ib].push(kveq_0);
        }/*else if (prod[sw[0]] < 0.0 && 0.0 <= prod[sw[1]])*/
        else if ((prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])
          || (prod[sw[1]] <= 0.0 && 0.0 < prod[sw[2]])) {
           for (i = 0; i < 3; ++i) {
             kveq_0[0][i] = kvp[ib][itri][sw[0]][i] * a[0][2] + kvp[ib][itri][sw[2]][i] * a[2][0];
             kveq_0[1][i] = kvp[ib][itri][sw[1]][i] * a[1][2] + kvp[ib][itri][sw[2]][i] * a[2][1];
           }/*for (i = 0; i < 3; ++i)*/
          kveq[ib].push(kveq_0);
        }/*else if (prod[sw[1]] < 0.0 && 0.0 <= prod[sw[2]])*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    /*
     Sum node-lines in all threads
    */
    nequator.push(kveq[ib].length);
    terminal("    " + String(ib + 1) + "       " + String(nequator[ib])+ "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function equator()*/
/**
 @brief Store triangle patch

 Modify : ::matp, ::kvp

 For the 1st BZ  mode, this routine cuts triangle recursivly at the
 BZ boundary (Bragg plane).

 - DO @f${\bf l}@f$ in Bragg vector
   - @f${p_i = {\bf l}\cdot{\bf k}}@f$
   - Sort : @f$p_0<p_1<p_2@f$
   - @f[
       a_{i j} \equiv \frac{-p_j}{p_i - p_j}
     @f]
   - if (@f$|{\bf l}| < p_0@f$)
     - This patch is not in the 1st BZ
   - if (@f$p_0 < |{\bf l}| < p_1@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_1 < |{\bf l}| < p_2@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
     - and
     - @f${\bf k}'_0 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - if (@f$p_2 < |{\bf l}| < p_3@f$)
     - @f${\bf k}'_0 = {\bf k}_0@f$
     - @f${\bf k}'_1 = {\bf k}_1@f$
     - @f${\bf k}'_2 = {\bf k}_2@f$
 - END DO
*/
function triangle(
  nbr = 1, //!<[in] Bragg plane
  mat1 = [[0.0,0.0,0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] The matrix element
  kvec1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] @f$k@f$-vector of corners
  vf1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], //!<[in] @f$v_f@f$-vector of corners
  kvp_v =[],
  matp_v = [],
  nmlp_v = []
  )
{
  let ibr = 0, i = 0, j = 0, sw = [0, 0, 0];
  let prod = [0.0, 0.0, 0.0], thr = 0.0, thr2 = 0.001,
    mat2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    kvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    bshift = 0.0,
    vfave = [0.0, 0.0, 0.0], norm = [0.0, 0.0, 0.0],
    kvp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], 
    matp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], 
    nmlp_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  /*
   If the area is nearly 0, it is ignored.
  */
  for (i = 0; i < 3; i++)norm[i] = 0.0;
  for (i = 0; i < 3; i++) {
    norm[0] += (kvec1[1][i] - kvec1[2][i])*(kvec1[1][i] - kvec1[2][i]);
    norm[1] += (kvec1[2][i] - kvec1[0][i])*(kvec1[2][i] - kvec1[0][i]);
    norm[2] += (kvec1[0][i] - kvec1[1][i])*(kvec1[0][i] - kvec1[1][i]);
  }
  for (i = 0; i < 3; i++) {
    if (norm[i] < 1.0e-10*brnrm_min) return;
  }
  /*
   For 1st BZ, it is cut at the BZ boundary.
  */
  if (fbz == 1) {
    /**/
    for (ibr = 0; ibr < nbragg; ++ibr) {

      thr = brnrm[ibr] * 0.001;
      /**/
      for (i = 0; i < 3; ++i) 
        prod[i] = bragg[ibr][0] * kvec1[i][0]
                + bragg[ibr][1] * kvec1[i][1]
                + bragg[ibr][2] * kvec1[i][2];
      eigsort(3, prod, sw);
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (brnrm[ibr] - prod[sw[j]]) / (prod[sw[i]] - prod[sw[j]]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/
      i = Math.floor(0.5 * ((prod[sw[2]] / brnrm[ibr]) + 1.0));
      bshift = -2.0 *i;

      if (brnrm[ibr] + thr > prod[sw[2]]) continue;

      if (brnrm[ibr] < prod[sw[0]]) {
        /*
         All corners are outside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          for (j = 0; j < 3; ++j) {
            kvec2[i][j] = kvec1[sw[i]][j] + bshift * bragg[ibr][j];
            mat2[i][j] = mat1[sw[i]][j];
            vf2[i][j] = vf1[sw[i]][j];
          }
        }
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else if (brnrm[ibr] < prod[sw[1]]) {
        /*
         Single corner (#0) is inside of the Bragg plane
        */
       for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          
          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][1] + kvec1[sw[1]][i] * a[1][0];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i] * a[0][1] + mat1[sw[1]][i] * a[1][0];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i] * a[0][1] + vf1[sw[1]][i] * a[1][0];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[2]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[2]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[2]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else if (brnrm[ibr] < prod[sw[2]]) {
        /*
        Two corners (#0, #1) are inside of the Bragg plane
        */
        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];
          kvec2[1][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[2][i] = kvec1[sw[2]][i];

          mat2[0][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];
          mat2[1][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[2][i] = mat1[sw[2]][i];

          vf2[0][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
          vf2[1][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[2][i] = vf1[sw[2]][i];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j)
          kvec2[i][j] += bshift * bragg[ibr][j];
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[0]][i];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[0]][i];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[0]][i];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);

        for (i = 0; i < 3; ++i) {
          kvec2[0][i] = kvec1[sw[1]][i] * a[1][2] + kvec1[sw[2]][i] * a[2][1];
          kvec2[1][i] = kvec1[sw[1]][i];
          kvec2[2][i] = kvec1[sw[0]][i] * a[0][2] + kvec1[sw[2]][i] * a[2][0];

          mat2[0][i] = mat1[sw[1]][i] * a[1][2] + mat1[sw[2]][i] * a[2][1];
          mat2[1][i] = mat1[sw[1]][i];
          mat2[2][i] = mat1[sw[0]][i] * a[0][2] + mat1[sw[2]][i] * a[2][0];

          vf2[0][i] = vf1[sw[1]][i] * a[1][2] + vf1[sw[2]][i] * a[2][1];
          vf2[1][i] = vf1[sw[1]][i];
          vf2[2][i] = vf1[sw[0]][i] * a[0][2] + vf1[sw[2]][i] * a[2][0];
        }/*for (i = 0; i < 3; ++i)*/
        triangle(ibr + 1, mat2, kvec2, vf2, kvp_v, matp_v, nmlp_v);
        return;
      }
      else {
        /*
        All corners are inside of the Bragg plane
        */
      } /* brnrm[ibr] + thr < prod */
    } /* for ibr = 1; ibr < nbragg*/
  } /* if fbz == 1 */
  /*
   Store patch
  */
  normal_vec(kvec1[0], kvec1[1], kvec1[2], norm);
  for (i = 0; i < 3; ++i) {
    vfave[i] = 0.0;
    for (j = 0; j < 3; ++j) vfave[i] += vf1[j][i];
  }
  prod[0] = 0.0;
  for (i = 0; i < 3; ++i) prod[0] += vfave[i] * norm[i];

  if (prod[0] < 0.0) {
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        kvp_0[i][j] = kvec1[2 - i][j];
        matp_0[i][j] = mat1[2 - i][j];
        nmlp_0[i][j] = vf1[2 - i][j];
      }
    }/*for (i = 0; i < 3; ++i)*/
  }
  else {
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        kvp_0[i][j] = kvec1[i][j];
        matp_0[i][j] = mat1[i][j];
        nmlp_0[i][j] = vf1[i][j];
      }
    }/*for (i = 0; i < 3; ++i)*/
  }
  kvp_v.push(kvp_0);
  matp_v.push(matp_0);
  nmlp_v.push(nmlp_0);
}/* triangle */
/**
@brief Cut triangle patch with the tetrahedron method.

 - Sort : @f$\varepsilon_0<\varepsilon_1<\varepsilon_2<\varepsilon_3@f$
 - @f[
     a_{i j} \equiv \frac{-\varepsilon_j}{\varepsilon_i - \varepsilon_j}
   @f]
 - if (@f$\varepsilon_0 < 0 < \varepsilon_1@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 1} + {\bf k}_1 a_{1 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_2 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
 - if (@f$\varepsilon_1 < 0 < \varepsilon_2@f$)
   - @f${\bf k}'_0 = {\bf k}_0 a_{0 2} + {\bf k}_2 a_{2 0}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
   - and
   - @f${\bf k}'_0 = {\bf k}_1 a_{1 3} + {\bf k}_3 a_{3 1}@f$
   - @f${\bf k}'_1 = {\bf k}_0 a_{0 3} + {\bf k}_3 a_{3 0}@f$
   - @f${\bf k}'_2 = {\bf k}_1 a_{1 2} + {\bf k}_2 a_{2 1}@f$
 - if (@f$\varepsilon_2 < 0 < \varepsilon_3@f$)
   - @f${\bf k}'_0 = {\bf k}_3 a_{3 0} + {\bf k}_0 a_{0 3}@f$
   - @f${\bf k}'_1 = {\bf k}_3 a_{3 1} + {\bf k}_1 a_{1 3}@f$
   - @f${\bf k}'_2 = {\bf k}_3 a_{3 2} + {\bf k}_2 a_{2 3}@f$
*/
function tetrahedron(
  eig1 = [], //!< [in] Orbital energies @f$\varepsilon_{n k}@f$
  mat1 = [], //!< [in] Matrix elements @f$\Delta_{n k}@f$
  kvec1 = [], //!< [in] @f$k@f$-vectors
  vf1 = [], //!< [in] @f$v_f@f$-vectors
  kvp_v = [],
  matp_v = [],
  nmlp_v = []
)
{
  let it = 0, i = 0, j = 0, sw = [0, 0, 0, 0];
  let eig2 = [0.0, 0.0, 0.0, 0.0],
    mat2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    kvec2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf2 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    a = [[0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0]],
    kvec3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    mat3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vf3 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    vol = 0.0, thr = 0.000;

  for (it = 0; it < 6; ++it) {
    /*
     Define corners of the tetrahedron
    */
    for (i = 0; i < 4; ++i) {
      eig2[i] = eig1[corner[it][i]];
      for (j = 0; j < 3; ++j) {
        mat2[i][j] = mat1[corner[it][i]][j];
        vf2[i][j] = vf1[corner[it][i]][j];
      }
      /*
       Fractional -> Cartecian
      */
      for (j = 0; j < 3; ++j) 
        kvec2[i][j] = bvec[0][j] * kvec1[corner[it][i]][0]
                    + bvec[1][j] * kvec1[corner[it][i]][1]
                    + bvec[2][j] * kvec1[corner[it][i]][2];
    }/*for (i = 0; i < 4; ++i)*/
    eigsort(4, eig2, sw);

    for (i = 0; i < 4; ++i) {
      for (j = 0; j < 4; ++j) {
        a[i][j] = (0.0 - eig2[sw[j]]) / (eig2[sw[i]] - eig2[sw[j]]);
      }/*for (j = 0; j < 4; ++j)*/
    }/*for (i = 0; i < 4; ++i)*/
    /*
     Draw triangle in each cases
    */
    if (eig2[sw[0]] <= 0.0 && 0.0 < eig2[sw[1]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][1] + kvec2[sw[1]][i] * a[1][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[2][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];

        mat3[0][i] = mat2[sw[0]][i] * a[0][1] + mat2[sw[1]][i] * a[1][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[2][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];

        vf3[0][i] = vf2[sw[0]][i] * a[0][1] + vf2[sw[1]][i] * a[1][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[2][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
      }
      
      vol = a[1][0] * a[2][0] * a[3][0];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else if (eig2[sw[1]] <= 0.0 && 0.0 < eig2[sw[2]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[0]][i] * a[0][2] + kvec2[sw[2]][i] * a[2][0];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[0]][i] * a[0][2] + mat2[sw[2]][i] * a[2][0];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[0]][i] * a[0][2] + vf2[sw[2]][i] * a[2][0];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }
      
      vol = a[1][2] * a[2][0] * a[3][0];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
      /**/
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[1]][i] * a[1][3] + kvec2[sw[3]][i] * a[3][1];
        kvec3[1][i] = kvec2[sw[0]][i] * a[0][3] + kvec2[sw[3]][i] * a[3][0];
        kvec3[2][i] = kvec2[sw[1]][i] * a[1][2] + kvec2[sw[2]][i] * a[2][1];

        mat3[0][i] = mat2[sw[1]][i] * a[1][3] + mat2[sw[3]][i] * a[3][1];
        mat3[1][i] = mat2[sw[0]][i] * a[0][3] + mat2[sw[3]][i] * a[3][0];
        mat3[2][i] = mat2[sw[1]][i] * a[1][2] + mat2[sw[2]][i] * a[2][1];

        vf3[0][i] = vf2[sw[1]][i] * a[1][3] + vf2[sw[3]][i] * a[3][1];
        vf3[1][i] = vf2[sw[0]][i] * a[0][3] + vf2[sw[3]][i] * a[3][0];
        vf3[2][i] = vf2[sw[1]][i] * a[1][2] + vf2[sw[2]][i] * a[2][1];
      }

      vol = a[1][3] * a[3][0] * a[2][1];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else if (eig2[sw[2]] <= 0.0 && 0.0 < eig2[sw[3]]) {
      for (i = 0; i < 3; ++i) {
        kvec3[0][i] = kvec2[sw[3]][i] * a[3][0] + kvec2[sw[0]][i] * a[0][3];
        kvec3[1][i] = kvec2[sw[3]][i] * a[3][1] + kvec2[sw[1]][i] * a[1][3];
        kvec3[2][i] = kvec2[sw[3]][i] * a[3][2] + kvec2[sw[2]][i] * a[2][3];

        mat3[0][i] = mat2[sw[3]][i] * a[3][0] + mat2[sw[0]][i] * a[0][3];
        mat3[1][i] = mat2[sw[3]][i] * a[3][1] + mat2[sw[1]][i] * a[1][3];
        mat3[2][i] = mat2[sw[3]][i] * a[3][2] + mat2[sw[2]][i] * a[2][3];

        vf3[0][i] = vf2[sw[3]][i] * a[3][0] + vf2[sw[0]][i] * a[0][3];
        vf3[1][i] = vf2[sw[3]][i] * a[3][1] + vf2[sw[1]][i] * a[1][3];
        vf3[2][i] = vf2[sw[3]][i] * a[3][2] + vf2[sw[2]][i] * a[2][3];
      }

      vol = a[0][3] * a[1][3] * a[2][3];
      triangle(0, mat3, kvec3, vf3, kvp_v, matp_v, nmlp_v);
    }
    else {
    }
  }/*for (it = 0; it < 6; ++it)*/
}/* tetrahedron */
/**
 @brief Compute patches for Fermi surfaces

 Modify : ::ntri, nmlp, ::matp, ::kvp, ::clr, ::nmlp_rot, ::kvp_rot
*/
function fermi_patch()
{
  let ib = 0, i0 = 0, i1 = 0, j0 = 0, start = [0, 0, 0], last = [0, 0, 0];
  let i = 0, j = 0, i2 = 0, j1 = 0, j2 = 0, ii0 = 0, ii1 = 0, ii2 = 0;
  let kvec1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
  [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    mat1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
    eig1 = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
    vf1 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  if (fbz == 1) {
    terminal("\n");
    terminal("  ##  First Brillouin zone mode  #######\n");
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = Math.floor(ng[i0] / 2) - ng[i0];
      last[i0] = Math.floor(ng[i0] / 2);
    }
  }
  else {
    terminal("\n");
    terminal("  ##  Premitive Brillouin zone mode  #######\n");
    for (i0 = 0; i0 < 3; ++i0) {
      start[i0] = 0;
      last[i0] = ng[i0];
    }
  }
  terminal("    Computing patch ...\n");
  terminal("      band   # of patchs\n");
/**/
  kvp = [];
  matp = [];
  nmlp = [];
  ntri = [];
  for (ib = 0; ib < nb; ++ib) {
    kvp.push([]);
    matp.push([]);
    nmlp.push([]);
    ntri.push([]);
    for (j0 = start[0]; j0 < last[0]; ++j0) {
      for (j1 = start[1]; j1 < last[1]; ++j1) {
        for (j2 = start[2]; j2 < last[2]; ++j2) {

          i0 = j0;
          i1 = j1;
          i2 = j2;
          ii0 = j0 + 1;
          ii1 = j1 + 1;
          ii2 = j2 + 1;
          /**/
          kvec1[0][0] = i0 / ng[0];
          kvec1[1][0] = i0 / ng[0];
          kvec1[2][0] = i0 / ng[0];
          kvec1[3][0] = i0 / ng[0];
          kvec1[4][0] = ii0 / ng[0];
          kvec1[5][0] = ii0 / ng[0];
          kvec1[6][0] = ii0 / ng[0];
          kvec1[7][0] = ii0 / ng[0];
          /**/
          kvec1[0][1] = i1 / ng[1];
          kvec1[1][1] = i1 / ng[1];
          kvec1[2][1] = ii1 / ng[1];
          kvec1[3][1] = ii1 / ng[1];
          kvec1[4][1] = i1 / ng[1];
          kvec1[5][1] = i1 / ng[1];
          kvec1[6][1] = ii1 / ng[1];
          kvec1[7][1] = ii1 / ng[1];
          /**/
          kvec1[0][2] = i2 / ng[2];
          kvec1[1][2] = ii2 / ng[2];
          kvec1[2][2] = i2 / ng[2];
          kvec1[3][2] = ii2 / ng[2];
          kvec1[4][2] = i2 / ng[2];
          kvec1[5][2] = ii2 / ng[2];
          kvec1[6][2] = i2 / ng[2];
          kvec1[7][2] = ii2 / ng[2];
          /**/
          for (i = 0; i < 8; i++)
            for (j = 0; j < 3; j++)
              kvec1[i][j] = kvec1[i][j] + shiftk[j] / (2.0 * ng0[j]);
          /**/
          i0 = modulo(i0, ng[0]);
          i1 = modulo(i1, ng[1]);
          i2 = modulo(i2, ng[2]);
          ii0 = modulo(ii0, ng[0]);
          ii1 = modulo(ii1, ng[1]);
          ii2 = modulo(ii2, ng[2]);
          /**/
          eig1[0] = eig[ib][i0][i1][i2] - EF;
          eig1[1] = eig[ib][i0][i1][ii2] - EF;
          eig1[2] = eig[ib][i0][ii1][i2] - EF;
          eig1[3] = eig[ib][i0][ii1][ii2] - EF;
          eig1[4] = eig[ib][ii0][i1][i2] - EF;
          eig1[5] = eig[ib][ii0][i1][ii2] - EF;
          eig1[6] = eig[ib][ii0][ii1][i2] - EF;
          eig1[7] = eig[ib][ii0][ii1][ii2] - EF;
          /**/
          for (j = 0; j < 3; j++) {
            mat1[0][j] = mat[ib][i0][i1][i2][j];
            mat1[1][j] = mat[ib][i0][i1][ii2][j];
            mat1[2][j] = mat[ib][i0][ii1][i2][j];
            mat1[3][j] = mat[ib][i0][ii1][ii2][j];
            mat1[4][j] = mat[ib][ii0][i1][i2][j];
            mat1[5][j] = mat[ib][ii0][i1][ii2][j];
            mat1[6][j] = mat[ib][ii0][ii1][i2][j];
            mat1[7][j] = mat[ib][ii0][ii1][ii2][j];
            /**/
            vf1[0][j] = vf[ib][i0][i1][i2][j];
            vf1[1][j] = vf[ib][i0][i1][ii2][j];
            vf1[2][j] = vf[ib][i0][ii1][i2][j];
            vf1[3][j] = vf[ib][i0][ii1][ii2][j];
            vf1[4][j] = vf[ib][ii0][i1][i2][j];
            vf1[5][j] = vf[ib][ii0][i1][ii2][j];
            vf1[6][j] = vf[ib][ii0][ii1][i2][j];
            vf1[7][j] = vf[ib][ii0][ii1][ii2][j];
          }/*for (j = 0; j < 3; j++)*/
          /**/
          tetrahedron(eig1, mat1, kvec1, vf1, 
            kvp[ib], matp[ib], nmlp[ib]);
        }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
      }/*for (j1 = start[1]; j1 < ng[1]; ++j1)*/
    }/*for (j0 = start[0]; j0 < ng[0]; ++j0)*/
    ntri[ib] = kvp[ib].length;
    terminal("      " + String(ib + 1) + "       " + String(ntri[ib]) + "\n");
  }/*for (ib = 0; ib < nb; ++ib)*/
  terminal("    ... Done\n");
} /* fermi_patch */
/**
 @brief Free variables for patch before new patch is computed

 Free : ::nmlp, ::matp, ::clr, ::kvp, ::nmlp_rot, ::kvp_rot,
        ::kvnl, ::kvnl_rot, ::kv2d, ::clr2d
*/
function free_patch()
{
  let ib, i0, i1, i2;
  /*
   Fermi patch
  */
  if (refresh_patch == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < ntri[ib]; ++i0) {
        for (i1 = 0; i1 < 3; ++i1) {
          for (i2 = 0; i2 < 2; ++i2)
            arw[ib][i0][i1][i2] = [];
          nmlp[ib][i0][i1] = [];
          matp[ib][i0][i1] = [];
          kvp[ib][i0][i1] = [];
          arw[ib][i0][i1] = [];
        }
        nmlp[ib][i0] = [];
        matp[ib][i0] = [];
        kvp[ib][i0] = [];
        arw[ib][i0] = [];
      }
      nmlp[ib] = [];
      matp[ib] = [];
      clr[ib] = [];
      kvp[ib] = [];
      arw[ib] = [];
      nmlp_rot[ib] = [];
      kvp_rot[ib] = [];
      arw_rot[ib] = [];
    }
    nmlp = [];
    matp = [];
    clr = [];
    kvp = [];
    arw = [];
    nmlp_rot = [];
    kvp_rot = [];
    arw_rot = [];
  }/*if (refresh_patch == 1)*/
  /*
   Nodal line
  */
  if (refresh_nodeline == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < nnl[ib]; ++i0) {
        for (i1 = 0; i1 < 2; ++i1) {
          kvnl[ib][i0][i1] = [];
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        kvnl[ib][i0] = [];
      }/*for (i0 = 0; i0 < nnl[ib]; ++i0)*/
      kvnl[ib] = [];
      kvnl_rot[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kvnl = [];
    kvnl_rot = [];
  }/*if (refresh_nodeline == 1)*/
  /*
   2D Fermi line
  */
  if (refresh_section == 1) {
    for (ib = 0; ib < nb; ++ib) {
      kv2d[ib] = [];
      clr2d[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kv2d = [];
    clr2d = [];
  }/*if (refresh_section == 1)*/
  /*
  equator
  */
  if (refresh_equator == 1) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < nequator[ib]; ++i0) {
        for (i1 = 0; i1 < 2; ++i1) {
          kveq[ib][i0][i1] = [];
        }/*for (i1 = 0; i1 < 2; ++i1)*/
        kveq[ib][i0] = [];
      }/*for (i0 = 0; i0 < nequator[ib]; ++i0)*/
      kveq[ib] = [];
      kveq_rot[ib] = [];
    }/*for (ib = 0; ib < nb; ++ib)*/
    kveq = [];
    kveq_rot = [];
  }/*if (refresh_equator == 1)*/
}/*function free_patch()*/
/**
 @brief Compute Max. & Min. of matrix elements.
 Compute color of each patch 

 Modify : ::clr
*/
function max_and_min() 
{
  let itri = 0;
  let i = 0, ib = 0;
  let abs = 0.0;

  terminal("\n");
  if (color_scale == 1) terminal("  ##  Color Scale as Input Quantity (Real) #############\n");
  else if (color_scale == 2) terminal("  ##  Color Scale as Input Quantity (Complex) #############\n");
  else if (color_scale == 3) terminal("  ##  Color Scale as Fermi Velocity #############\n");
  else if (color_scale == 4) terminal("  ##  Color Scale as Band Index #############\n");
  else if (color_scale == 5) terminal("  ##  Gray Scale as Input Quantity (Real) #############\n");
  else if (color_scale == 6) terminal("  ##  Gray Scale as Fermi Velocity #############\n");
  terminal("\n");
  /*
   Search max and min.
  */
  if (color_scale == 1 || color_scale == 6) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          if (matp[ib][itri][i][0] > patch_max) patch_max = matp[ib][itri][i][0];
          if (matp[ib][itri][i][0] < patch_min) patch_min = matp[ib][itri][i][0];
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 0 || color_scale == 4)*/
  else if (color_scale == 2) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                        + matp[ib][itri][i][1] * matp[ib][itri][i][1]);
          if (abs > patch_max) patch_max = abs;
          if (abs < patch_min) patch_min = abs;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
    }/*if (color_scale == 2)*/
  else   if (color_scale == 3) {

    patch_min = 1.0e10;
    patch_max = -1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                        + matp[ib][itri][i][1] * matp[ib][itri][i][1]
                        + matp[ib][itri][i][2] * matp[ib][itri][i][2]);
          if (abs > patch_max) patch_max = abs;
          if (abs < patch_min) patch_min = abs;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 3)*/
  else if (color_scale == 4 || color_scale == 7) {

    patch_max = -1.0e10;
    patch_min = 1.0e10;

    for (ib = 0; ib < nb; ib++) {
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          norm = 0.0;
          for (j = 0; j < 3; ++j) norm += nmlp[ib][itri][i][j]*nmlp[ib][itri][i][j];
          norm = Math.sqrt(norm);

          if (norm > patch_max) patch_max = norm;
          if (norm < patch_min) patch_min = norm;
        }
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 5 || color_scale == 6)*/

  document.getElementById("scalemin").value = String(patch_min);
  document.getElementById("scalemax").value = String(patch_max);
}/* max_and_min */
 /**
 @brief Compute Max. & Min. of matrix elements.
 Compute color of each patch

 Modify : ::clr
 */
function paint()
{
  let itri = 0, j = 0;
  let origin = [0.0, 0.0, 0.0, 0.0];
  let i = 0, ib = 0;
  let mat2 = 0.0;
  let theta = 0.0, abs = 0.0, theta2 = 0.0;

  clr = [];
  if (color_scale == 1) {
    for (ib = 0; ib < nb; ib++) {
      clr.push([]);
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = (matp[ib][itri][i][0] - patch_min) / (patch_max - patch_min);
          mat2 = mat2 * 4.0;
          /**/
          if (mat2 <= 1.0) {
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2));
          }
          else if (mat2 <= 2.0) {
            mat2 = mat2 - 1.0;
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2));
          }
          else if (mat2 <= 3.0) {
            mat2 = mat2 - 2.0;
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2));
          }
          else {
            mat2 = mat2 - 3.0;
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2));
          }
        }/*for (i = 0; i < 3; ++i)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 1 || color_scale == 2)*/
  else if (color_scale == 2) {
    for (j = 0; j < 4; ++j) origin[j] = 1.0 - BackGroundColor[j];
    for (ib = 0; ib < nb; ib++) {
      clr.push([]);
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          abs = Math.sqrt(matp[ib][itri][i][0] * matp[ib][itri][i][0]
                        + matp[ib][itri][i][1] * matp[ib][itri][i][1]);
          if (abs / patch_max < 0.00001) theta = 0.0;
          else if (matp[ib][itri][i][1] > 0.0) theta = Math.acos(matp[ib][itri][i][0] / abs);
          else theta = -Math.acos(matp[ib][itri][i][0] / abs);
          abs /= patch_max;
          theta = 3.0 * theta / Math.acos(-1.0);
          /**/
          if (-3.0 <= theta && theta < -2.0) {
            theta2 = theta + 3.0;
            for (j = 0; j < 4; ++j)
              clr[ib].push(blue[j] * theta2 + cyan[j] * (1.0 - theta2));
          }
          else if (-2.0 <= theta && theta < -1.0) {
            theta2 = theta + 2.0;
            for (j = 0; j < 4; ++j) 
              clr[ib].push(magenta[j] * theta2 + blue[j] * (1.0 - theta2));
          }
          else if (-1.0 <= theta && theta < 0.0) {
            theta2 = theta + 1.0;
            for (j = 0; j < 4; ++j) 
              clr[ib].push(red[j] * theta2 + magenta[j] * (1.0 - theta2));
          }
          else if (0.0 <= theta && theta < 1.0) {
            theta2 = theta;
            for (j = 0; j < 4; ++j)
              clr[ib].push(yellow[j] * theta2 + red[j] * (1.0 - theta2));
          }
          else if (1.0 <= theta && theta < 2.0) {
            theta2 = theta - 1.0;
            for (j = 0; j < 4; ++j)
              clr[ib].push(green[j] * theta2 + yellow[j] * (1.0 - theta2));
          }
          else {
            theta2 = theta - 2.0;
            for (j = 0; j < 4; ++j) 
              clr[ib].push(cyan[j] * theta2 + green[j] * (1.0 - theta2));
          }
          clr[ib][j + 4 * i + 12 * itri] = clr[ib][j + 4 * i + 12 * itri] * abs + origin[j] * (1.0 - abs);
        }/*for (i = 0; i < 3; ++i)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 2)*/
  else if (color_scale == 4) {
      for (ib = 0; ib < nb; ib++) {
        clr.push([]);
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            /**/
            mat2 = 0.0;
            for (j = 0; j < 3; ++j) mat2 += nmlp[ib][itri][i][j] * nmlp[ib][itri][i][j];
            mat2 = Math.sqrt(mat2);
            mat2 = (mat2 - patch_min) / (patch_max - patch_min);
            mat2 = mat2 * 4.0;
            /**/
            if (mat2 <= 1.0) {
              for (j = 0; j < 4; ++j) clr[ib].push(BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2));
            }
            else if (mat2 <= 2.0) {
              mat2 = mat2 - 1.0;
              for (j = 0; j < 4; ++j) clr[ib].push(BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2));
            }
            else if (mat2 <= 3.0) {
              mat2 = mat2 - 2.0;
              for (j = 0; j < 4; ++j) clr[ib].push(BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2));
            }
            else {
              mat2 = mat2 - 3.0;
              for (j = 0; j < 4; ++j) clr[ib].push(BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2));
            }
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 4)*/
  else if (color_scale == 3 || color_scale == 5) {
    for (ib = 0; ib < nb; ib++) {
      clr.push([]);
      /**/
      if (nb == 1) mat2 = 0.5;
      else mat2 = 1.0 / (nb - 1) * ib;
      mat2 *= 4.0;
      /**/
      if (mat2 <= 1.0) {
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[1][j] * mat2 + BarColor[0][j] * (1.0 - mat2));
          }
        }
      }
      else if (mat2 <= 2.0) {
        mat2 = mat2 - 1.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[2][j] * mat2 + BarColor[1][j] * (1.0 - mat2));
          }
        }
      }
      else if (mat2 <= 3.0) {
        mat2 = mat2 - 2.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[3][j] * mat2 + BarColor[2][j] * (1.0 - mat2));
          }
        }
      }
      else {
        mat2 = mat2 - 3.0;
        for (itri = 0; itri < ntri[ib]; ++itri) {
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 4; ++j) clr[ib].push(BarColor[4][j] * mat2 + BarColor[3][j] * (1.0 - mat2));
          }
        }
      }
    }/*for (ib = 0; ib < nb; ib++*/
    if (color_scale == 3) {
      for (ib = 0; ib < nb; ib++) {
        clr.push([]);
        for (itri = 0; itri < ntri[ib]; ++itri){
          for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
              arw[ib][itri][i][0][j] = kvp[ib][itri][i][j]
                              - 0.5 * matp[ib][itri][i][j] / patch_max;
              arw[ib][itri][i][1][j] = kvp[ib][itri][i][j]
                              + 0.5 * matp[ib][itri][i][j] / patch_max;
            }/*for (j = 0; j < 3; ++j)*/
          }/*for (i = 0; i < 3; ++i)*/
        }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
      }/*for (ib = 0; ib < nb; ib++)*/
    }/*if (color_scale == 3)*/
  }/*if (color_scale == 5)*/
  else if (color_scale == 6) {
    for (ib = 0; ib < nb; ib++) {
      clr.push([]);
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = (matp[ib][itri][i][0] - patch_min) / (patch_max - patch_min);
          /**/
          for (j = 0; j < 4; ++j) clr[ib].push(wgray[j] * mat2 + bgray[j] * (1.0 - mat2));
        }/*for (i = 0; i < 3; ++i)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 6)*/
  else if (color_scale == 7) {
    for (ib = 0; ib < nb; ib++) {
      clr.push([]);
      for (itri = 0; itri < ntri[ib]; ++itri) {
        for (i = 0; i < 3; ++i) {
          /**/
          mat2 = 0.0;
          for (j = 0; j < 3; ++j) mat2 += nmlp[ib][itri][i][j] * nmlp[ib][itri][i][j];
          mat2 = Math.sqrt(mat2);
          mat2 = (mat2 - patch_min) / (patch_max - patch_min);
          /**/
          for (j = 0; j < 4; ++j) clr[ib].push(wgray[j] * mat2 + bgray[j] * (1.0 - mat2));
        }/*for (i = 0; i < 3; ++i)*/
      }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    }/*for (ib = 0; ib < nb; ib++)*/
  }/*if (color_scale == 7)*/
}/* paint */
/**
 @brief Specify corners of tetrahedron

 Modify : ::corner
*/
function init_corner()
{
  let i = 0, j = 0;
  let corner1 = [
    /*
    [0] min = 0-7
    */
    [ [ 0, 1, 3, 7 ],
    [ 0, 1, 5, 7 ],
    [ 0, 2, 3, 7 ],
    [ 0, 2, 6, 7 ],
    [ 0, 4, 5, 7 ],
    [ 0, 4, 6, 7 ] ],
    /*
    [1] min = 1-6
    */
    [ [ 1, 0, 2, 6 ],
    [ 1, 0, 4, 6 ],
    [ 1, 3, 2, 6 ],
    [ 1, 3, 7, 6 ],
    [ 1, 5, 4, 6 ],
    [ 1, 5, 7, 6 ] ],
    /*
    [2] min = 2-5
    */
    [ [ 2, 0, 1, 5 ],
    [ 2, 0, 4, 5 ],
    [ 2, 3, 1, 5 ],
    [ 2, 3, 7, 5 ],
    [ 2, 6, 4, 5 ],
    [ 2, 6, 7, 5 ] ],
    /*
    [3] min = 3-4
    */
    [ [ 4, 0, 1, 3 ],
    [ 4, 0, 2, 3 ],
    [ 4, 5, 1, 3 ],
    [ 4, 5, 7, 3 ],
    [ 4, 6, 2, 3 ],
    [ 4, 6, 7, 3 ] ],
    /*
    [4] min = 0-7, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 0, 7, 2, 6 ],
    [ 0, 7, 1, 5 ],
    [ 0, 7, 1, 2 ],
    [ 0, 7, 6, 5 ] ],
    /*
    [5] min = 1-6, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 1, 6, 5, 7 ],
    [ 1, 6, 7, 2 ],
    [ 1, 6, 2, 0 ],
    [ 1, 6, 0, 5 ] ],
    /*
    [6] min = 2-5, max = 3-4
    */
    [ [ 0, 4, 5, 6 ],
    [ 1, 2, 3, 7 ],
    [ 2, 5, 7, 6 ],
    [ 2, 5, 6, 0 ],
    [ 2, 5, 0, 1 ],
    [ 2, 5, 1, 7 ] ],
    /*
    [7] min = 3-4, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 3, 4, 1, 5 ],
    [ 3, 4, 5, 6 ],
    [ 3, 4, 6, 2 ],
    [ 3, 4, 2, 1 ] ],
    /*
    [8] min = 2-5, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 2, 5, 1, 3 ],
    [ 2, 5, 3, 6 ],
    [ 2, 5, 6, 4 ],
    [ 2, 5, 4, 1 ] ],
    /*
    [9] min = 1-6, max = 0-7
    */
    [ [ 0, 1, 2, 4 ],
    [ 7, 3, 5, 6 ],
    [ 1, 6, 2, 3 ],
    [ 1, 6, 3, 5 ],
    [ 1, 6, 5, 4 ],
    [ 1, 6, 4, 2 ] ],
    /*
    [10] min = 0-7, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 0, 7, 2, 3 ],
    [ 0, 7, 3, 5 ],
    [ 0, 7, 5, 4 ],
    [ 0, 7, 4, 2 ] ],
    /*
    [11] min = 3-4, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 3, 4, 0, 2 ],
    [ 3, 4, 2, 7 ],
    [ 3, 4, 7, 5 ],
    [ 3, 4, 5, 0 ] ],
    /*
    [12] min = 2-5, max = 1-6
    */
    [ [ 1, 0, 3, 5 ],
    [ 6, 2, 4, 7 ],
    [ 2, 5, 0, 3 ],
    [ 2, 5, 3, 7 ],
    [ 2, 5, 7, 4 ],
    [ 2, 5, 4, 0 ] ],
    /*
    [13] min = 0-7, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 0, 7, 1, 3 ],
    [ 0, 7, 3, 6 ],
    [ 0, 7, 6, 4 ],
    [ 0, 7, 4, 1 ] ],
    /*
    [14] min = 1-6, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 1, 6, 0, 3 ],
    [ 1, 6, 3, 7 ],
    [ 1, 6, 7, 4 ],
    [ 1, 6, 4, 0 ] ],
    /*
    [15] min = 3-4, max = 2-5
    */
    [ [ 2, 0, 3, 6 ],
    [ 5, 1, 4, 7 ],
    [ 3, 4, 0, 1 ],
    [ 3, 4, 1, 7 ],
    [ 3, 4, 7, 6 ],
    [ 3, 4, 6, 0 ] ],
  ];
  /**/
  for (i = 0; i < 6; ++i) {
    for (j = 0; j < 4; ++j) {
      corner[i][j] = corner1[itet][i][j];
    }
  }
}
/**
 @brief Compute Bragg vector

 Modify : ::bragg, ::brnrm
*/
function bragg_vector()
{
  let i0, i1, i2, i, ibr, sw = [0, 0, 0, 0];
  let ldiag = [0.0, 0.0, 0.0, 0.0], bdiag = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];
  /**/
  ibr = 0;
  //
  bragg = [];
  brnrm = [];
  for (i0 = -1; i0 <= 1; ++i0) {
    for (i1 = -1; i1 <= 1; ++i1) {
      for (i2 = -1; i2 <= 1; ++i2) {
        /*
         Except Gamma points
        */
        if (i0 == 0 && i1 == 0 && i2 == 0) continue;
        /*
         Fractional -> Cartesian
        */
        bragg.push([]);
        for (i = 0; i < 3; ++i)
          bragg[ibr].push((i0 * bvec[0][i]
                        +  i1 * bvec[1][i]
                        +  i2 * bvec[2][i]) * 0.5);
        /*
         And its norm
        */
        brnrm.push(bragg[ibr][0] * bragg[ibr][0]
                 + bragg[ibr][1] * bragg[ibr][1]
                 + bragg[ibr][2] * bragg[ibr][2]);
        /**/
        ibr += 1;
      }/*for (i2 = -1; i2 <= 1; ++i2)*/
    }/*for (i1 = -1; i1 <= 1; ++i1)*/
  }/*for (i0 = -1; i0 <= 1; ++i0)*/
  /*
   Search min. of brnrm
  */
  brnrm_min = brnrm[0];
  for (ibr = 1; ibr < 26; ibr++) {
    if (brnrm_min > brnrm[ibr]) brnrm_min = brnrm[ibr];
  }
  terminal("    Minimum Bragg norm : " + String(brnrm_min) + "\n");
  //
  // Search the shortest diagonal line
  //
  for (i = 0; i < 3; i++) {
      bdiag[0][i] =  bvec[0][i] / ng0[0] + bvec[1][i] / ng0[1] + bvec[2][i] / ng0[2];
      bdiag[1][i] =  bvec[0][i] / ng0[0] + bvec[1][i] / ng0[1] - bvec[2][i] / ng0[2];
      bdiag[2][i] =  bvec[0][i] / ng0[0] - bvec[1][i] / ng0[1] + bvec[2][i] / ng0[2];
      bdiag[3][i] = -bvec[0][i] / ng0[0] + bvec[1][i] / ng0[1] + bvec[2][i] / ng0[2];
    }
    for (i = 0; i < 4; i++)
      ldiag[i] = bdiag[i][0] * bdiag[i][0] + bdiag[i][1] * bdiag[i][1] + bdiag[i][2] * bdiag[i][2];
    eigsort(4, ldiag, sw);
    itet = sw[0];
    document.getElementById("tetrahedron").tetrahedron.value = String(itet);
}/* bragg_vector */
/**
 @brief Print max and minimum @f$\varepsilon_{n k}, \Delta_{n k}@f$
        in the whole Brillouine zone
*/
function max_and_min_bz()
{
  let ib, i0, i1, i2;
  let eigmin, eigmax, matmin, matmax;
  /**/
  terminal("\n");
  terminal("  ##  Max. and Min. of each bands  #######################\n");
  terminal("\n");
  terminal("    Band   Eig_Min.      Eig_Max      Mat_Min      Mat_Max\n");
  for (ib = 0; ib < nb; ib++) {
    eigmax = eig0[0][0][0][0];
    eigmin = eig0[0][0][0][0];
    matmax = mat0[0][0][0][0][0];
    matmin = mat0[0][0][0][0][0];
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (eig0[ib][i0][i1][i2] > eigmax) eigmax = eig0[ib][i0][i1][i2];
          if (eig0[ib][i0][i1][i2] < eigmin) eigmin = eig0[ib][i0][i1][i2];
          if (mat0[ib][i0][i1][i2][0] > matmax) matmax = mat0[ib][i0][i1][i2][0];
          if (mat0[ib][i0][i1][i2][0] < matmin) matmin = mat0[ib][i0][i1][i2][0];
        }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
      }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
    }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    terminal("    " + String(ib + 1) + "     " + String(eigmin) + "     " +
      String(eigmax) + "     " + String(matmin) + "     " + String(matmax) + "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/* max_and_min_bz */
/**
 @brief Compute coefficient for the French-curve (Kumo) interpolation
 @f[
  A^{\rm intp} = \sum_{i = 1}^4 C_i A_i^{\rm orig}
 @f]
*/
function kumo_coef(
  j = 0, //!< [in] Interpolated grid index
  coef = [0.0, 0.0, 0.0, 0.0],//!< [out] Coefficient of interpolation @f$C_i@f$
  interpol = 0
) {
  let x = 0.0, mx = 0.0;
  x = j / interpol;
  mx = 1.0 - x;
  coef[0] = -0.5 * x * mx * mx;
  coef[1] = mx * (mx*mx + 3.0* x*mx + 0.5* x* x);
  coef[2] =  x * ( x* x + 3.0*mx* x + 0.5*mx*mx);
  coef[3] = -0.5 * x *  x * mx;
}
/**
 @brief Interpolation of energy and matrix 
        with the French-curve (Kumo) interpolation.

 Modify : ::eig, ::mat
*/
function interpol_energy() 
{
  let ib = 0, i0 = 0, i1 = 0, i2 = 0, ii = 0;
  let j0 = 0, j1 = 0, j2 = 0, jj;
  let coef = [0.0, 0.0, 0.0, 0.0],
    mat1 = [], mat2 = [], mat3 = [], eig1 = [], eig2 = [], eig3 = [];
  let i0p = 0, i0m = 0, i1p = 0, i1m = 0, i2p = 0, i2m = 0;
  let de = [0.0, 0.0, 0.0];

  for (i0 = 0; i0 < 4; i0++) {
    mat1.push([]);
    mat2.push([]);
    mat3.push([0.0, 0.0, 0.0]);
    eig1.push([]);
    eig2.push([]);
    eig3.push(0.0);
    for (i1 = 0; i1 < 4; i1++) {
      mat1[i0].push([]);
      mat2[i0].push([0.0, 0.0, 0.0]);
      eig1[i0].push([]);
      eig2[i0].push(0.0);
      for (i2 = 0; i2 < 4; i2++) {
        mat1[i0][i1].push([0.0, 0.0, 0.0]);
        eig1[i0][i1].push(0.0);
      }
    }
  }

  terminal("    Interpolating ... ");
  /*
   Reallocate
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng[0]; i0++) {
      for (i1 = 0; i1 < ng[1]; i1++) {
        for (i2 = 0; i2 < ng[2]; i2++) {
          vf[ib][i0][i1][i2] = [];
          mat[ib][i0][i1][i2] = [];
        }
        eig[ib][i0][i1] = [];
        mat[ib][i0][i1] = [];
        vf[ib][i0][i1] = [];
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
      eig[ib][i0] = [];
      mat[ib][i0] = [];
      vf[ib][i0] = [];
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
    eig[ib] = [];
    mat[ib] = [];
    vf[ib] = [];
  }/*for (ib = 0; ib < nb; ib++)*/
  eig = [];
  mat = [];
  vf = [];
  for (ii = 0; ii < 3; ii++)ng[ii] = ng0[ii] * interpol;
  /**/
  for (ib = 0; ib < nb; ib++) {
    eig.push([]);
    mat.push([]);
    vf.push([]);
    for (i0 = 0; i0 < ng[0]; i0++) {
      eig[ib].push([]);
      mat[ib].push([]);
      vf[ib].push([]);
      for (i1 = 0; i1 < ng[1]; i1++) {
        eig[ib][i0].push([]);
        mat[ib][i0].push([]);
        vf[ib][i0].push([]);
        for (i2 = 0; i2 < ng[2]; i2++) {
          eig[ib][i0][i1].push(0.0);
          mat[ib][i0][i1].push([0.0, 0.0, 0.0]);
          vf[ib][i0][i1].push([0.0, 0.0, 0.0]);
        }
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  /*
   3rd order - three dimensional Kumo interpolation
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng0[0]; i0++) {
      for (i1 = 0; i1 < ng0[1]; i1++) {
        for (i2 = 0; i2 < ng0[2]; i2++) {
          for (j0 = 0; j0 < 4; j0++) {
            for (j1 = 0; j1 < 4; j1++) {
              for (j2 = 0; j2 < 4; j2++) {
                eig1[j0][j1][j2] = eig0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                           [modulo(i1 + j1 - 1, ng0[1])]
                                           [modulo(i2 + j2 - 1, ng0[2])];
                for (jj = 0; jj < 3; jj++) {
                    mat1[j0][j1][j2][jj] = mat0[ib][modulo(i0 + j0 - 1, ng0[0])]
                                                   [modulo(i1 + j1 - 1, ng0[1])]
                                                   [modulo(i2 + j2 - 1, ng0[2])][jj];
                }
              }/*for (j2 = 0; j2 < 4; j2++)*/
            }/*for (j1 = 0; j1 < 4; j1++)*/
          }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
          for (j0 = 0; j0 < interpol; j0++) {
              kumo_coef(j0, coef, interpol);
            for (j1 = 0; j1 < 4; j1++) {
              for (j2 = 0; j2 < 4; j2++) {
                  eig2[j1][j2] = 0.0;
                for (jj = 0; jj < 3; jj++) mat2[j1][j2][jj] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                  eig2[j1][j2] += coef[ii] * eig1[ii][j1][j2];
                  for (jj = 0; jj < 3; jj++) 
                    mat2[j1][j2][jj] += coef[ii] * mat1[ii][j1][j2][jj];
                }/*for (ii = 0; ii < 4; ii++)*/
              }/*for (j2 = 0; j2 < 4; j2++)*/
            }/*for (j1 = 0; j1 < 4; j1++)*/
            for (j1 = 0; j1 < interpol; j1++) {
              kumo_coef(j1, coef, interpol);
              for (j2 = 0; j2 < 4; j2++) {
                eig3[j2] = 0.0;
                for (jj = 0; jj < 3; jj++) mat3[j2][jj] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                  eig3[j2] += coef[ii] * eig2[ii][j2];
                  for (jj = 0; jj < 3; jj++)
                    mat3[j2][jj] += coef[ii] * mat2[ii][j2][jj];
                }/*for (ii = 0; ii < 4; ii++)*/
              }/*for (j2 = 0; j2 < 4; j2++)*/
              for (j2 = 0; j2 < interpol; j2++) {
                kumo_coef(j2, coef, interpol);
                eig[ib][i0 * interpol + j0]
                       [i1 * interpol + j1]
                       [i2 * interpol + j2] = 0.0;
                for (jj = 0; jj < 3; jj++)
                  mat[ib][i0 * interpol + j0]
                         [i1 * interpol + j1]
                         [i2*interpol + j2][jj] = 0.0;
                for (ii = 0; ii < 4; ii++) {
                    eig[ib][i0*interpol + j0]
                           [i1*interpol + j1]
                           [i2*interpol + j2] += coef[ii] * eig3[ii];
                  for (jj = 0; jj < 3; jj++)
                      mat[ib][i0*interpol + j0]
                             [i1*interpol + j1]
                             [i2*interpol + j2][jj] += coef[ii] * mat3[ii][jj];
                }/*for (ii = 0; ii < 4; ii++)*/
              }/*for (j2 = 0; j2 < interpol; j2++)*/
            }/*for (j1 = 0; j1 < interpol; j1++)*/
          }/*for (j0 = 0; j0 < interpol; j0++)*/
        }/*for (i2 = 0; i2 < ng0[2]; i2++)*/
      }/*for (i1 = 0; i1 < ng0[1]; i1++)*/
    }/*for (i0 = 0; i0 < ng0[0]; i0++)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  /*
   Fermi velocity
  */
  for (ib = 0; ib < nb; ib++) {
    for (i0 = 0; i0 < ng[0]; i0++) {
      i0p = modulo(i0 + 1, ng[0]);
      i0m = modulo(i0 - 1, ng[0]);
      for (i1 = 0; i1 < ng[1]; i1++) {
        i1p= modulo(i1 + 1, ng[1]);
        i1m = modulo(i1 - 1, ng[1]);
        for (i2 = 0; i2 < ng[2]; i2++) {
          i2p = modulo(i2 + 1, ng[2]);
          i2m = modulo(i2 - 1, ng[2]);

          de[0] = eig[ib][i0p][i1][i2] - eig[ib][i0m][i1][i2];
          de[1] = eig[ib][i0][i1p][i2] - eig[ib][i0][i1m][i2];
          de[2] = eig[ib][i0][i1][i2p] - eig[ib][i0][i1][i2m];
          for (ii = 0; ii < 3; ii++)de[ii] *= 0.5 * ng[ii];
          for (ii = 0; ii < 3; ii++) vf[ib][i0][i1][i2][ii] =
            avec[0][ii] * de[0] + avec[1][ii] * de[1] + avec[2][ii] * de[2];
        }/*for (i2 = 0; i2 < ng[2]; i2++)*/
      }/*for (i1 = 0; i1 < ng[1]; i1++)*/
    }/*for (i0 = 0; i0 < ng[0]; i0++)*/
  }/*for (ib = 0; ib < nb; ib++)*/
  terminal("Done\n\n");
}/*function interpol_energy() */

function compute_patch_segment() {
  if (refresh_interpol == 1){
    interpol_energy();
    refresh_patch = 1;
    refresh_interpol = 0;
  }
  if (refresh_patch == 1) {
    fermi_patch();
    refresh_color = 1;
    refresh_section = 1;
    refresh_equator = 1;
    refresh_nodeline = 1;
    refresh_patch = 0;
  }
  if (refresh_color == 1) {
    if (skip_minmax == 1) skip_minmax = 0;
    else max_and_min();
    paint();
    refresh_section = 1;
    refresh_color = 0;
  }
  if (refresh_nodeline == 1) {
    calc_nodeline();
    refresh_nodeline = 0;
  }
  if (refresh_section == 1) {
    calc_2dbz();
    calc_section();
    refresh_section = 0;
  }
  if (refresh_equator == 1) {
    equator();
    refresh_equator = 0;
  }
}
function update_delay() {
  compute_patch_segment()
  drawScene();
}
function update_now() {
  let ib = 0, i0 = 0, i1 =0, i2 = 0;

  linewidth = Number(document.getElementById("linewidth").value);
  //
  // Line color
  //
  LineColor[0] = Number(document.getElementById("linecolorr").value);
  LineColor[1] = Number(document.getElementById("linecolorg").value);
  LineColor[2] = Number(document.getElementById("linecolorb").value);
  //
  // Background color
  //
  BackGroundColor[0] = Number(document.getElementById("backgraoundr").value);
  BackGroundColor[1] = Number(document.getElementById("backgraoundg").value);
  BackGroundColor[2] = Number(document.getElementById("backgraoundb").value);
  //
  //Rotate, scale, translate
  //
  trans[0] = Number(document.getElementById("positionx").value);
  trans[1] = Number(document.getElementById("positiony").value);
  thetax = Number(document.getElementById("rotatex").value);
  thetay = Number(document.getElementById("rotatey").value);
  thetaz = Number(document.getElementById("rotatez").value);
  rot[0][0] = Math.cos(thetay) * Math.cos(thetaz);
  rot[0][1] = -Math.cos(thetay) * Math.sin(thetaz);
  rot[0][2] = Math.sin(thetay);
  rot[1][0] = Math.cos(thetaz) * Math.sin(thetax) * Math.sin(thetay) + Math.cos(thetax) * Math.sin(thetaz);
  rot[1][1] = Math.cos(thetax) * Math.cos(thetaz) - Math.sin(thetax) * Math.sin(thetay) * Math.sin(thetaz);
  rot[1][2] = -Math.cos(thetay) * Math.sin(thetax);
  rot[2][0] = -Math.cos(thetax) * Math.cos(thetaz) * Math.sin(thetay) + Math.sin(thetax) * Math.sin(thetaz);
  rot[2][1] = Math.cos(thetaz) * Math.sin(thetax) + Math.cos(thetax) * Math.sin(thetay) * Math.sin(thetaz);
  rot[2][2] = Math.cos(thetax) * Math.cos(thetay);
  scl = Number(document.getElementById("scale").value);
  //
  //Band
  //
  i0 = Number(document.getElementById("band").value);
  i1 = 10;
  for (ib = 0; ib < nb; ib++) {
    i2 = i0 % i1;
    if (i2 === 0){
      draw_band[ib] = 0;
    }
    else{
      draw_band[ib] = 1;
    }
    i0 += -i2;
    i1 *= 10;
  }

  drawScene();
}
function update_interpol() {
  interpol = Number(document.getElementById("interpol").value);
  refresh_interpol = 1;
}
function update_efermi(){
  EF = Number(document.getElementById("fermienergy").value);
  refresh_patch = 1;
}
function update_color() {
  color_scale = Number(document.getElementById('colorscalemode').colorscalemode.value);
  refresh_color = 1;
}
function update_minmax() {
  patch_min = Number(document.getElementById("scalemin").value);
  patch_max = Number(document.getElementById("scalemax").value);
  refresh_color = 1;
  skip_minmax = 1;
}
function update_tetra() {
  itet = Number(document.getElementById('tetrahedron').tetrahedron.value);
  init_corner();
  refresh_patch = 1;
}
function button_update() {
  let ii = 0, jj = 0;
  /*
   Section vector
   */
  secvec_fr[0] = Number(document.getElementById("sectionv0"));
  secvec_fr[1] = Number(document.getElementById("sectionv1"));
  secvec_fr[2] = Number(document.getElementById("sectionv2"));
  if (document.getElementById("ongamma")) secscale = 0.0;
  else secscale = 1.0;
  for (ii = 0; ii < 3; ii++) {
    secvec[ii] = 0.0;
    for (jj = 0; jj < 3; jj++) {
      secvec[ii] += secvec_fr[jj] * bvec[jj][ii];
    }/*for (jj = 0; jj < 3; jj++)*/
  }/*for (ii = 0; ii < 3; ii++)*/
  /*
   Equator
   */
  eqvec_fr[0] = Number(document.getElementById("equatorv0"));
  eqvec_fr[1] = Number(document.getElementById("equatorv1"));
  eqvec_fr[2] = Number(document.getElementById("equatorv2"));
  for (ii = 0; ii < 3; ii++) {
    eqvec[ii] = 0.0;
    for (jj = 0; jj < 3; jj++) {
      eqvec[ii] += eqvec_fr[jj] * bvec[jj][ii];
    }/*for (jj = 0; jj < 3; jj++)*/
  }/*for (ii = 0; ii < 3; ii++)*/  
  /*  */
  /**/
  fbz = Number(document.getElementById('brillouinzone').brillouinzone.value);
  lstereo = Number(document.getElementById('stereogram').stereogram.value);
  lmouse = Number(document.getElementById('mousedrag').mousedrag.value);
  /*
   BZ number
  */
  BZ_number[0] = Number(document.getElementById("bznumber0"));
  BZ_number[1] = Number(document.getElementById("bznumber1"));
  BZ_number[2] = Number(document.getElementById("bznumber2"));
  glClearColor(BackGroundColor[0], BackGroundColor[1], BackGroundColor[2], BackGroundColor[3]);
  /**/
  if (document.getElementById("colorbar")) lcolorbar = 1;
  else lcolorbar = 0;
  if (document.getElementById("equator")) lequator = 1;
  else lequator = 0;
  if (document.getElementById("nodalline")) nodeline = 1;
  else nodeline = 0;
  if (document.getElementById("section")) lsection = 1;
  else lsection = 0;
  /**/
  side = Number(document.getElementById('lighting').mousedrag.value);
  /**/
  if (document.getElementById('barcolor').mousedrag.value == "rgb") {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = blue[ii];
      BarColor[1][ii] = cyan[ii];
      BarColor[2][ii] = green[ii];
      BarColor[3][ii] = yellow[ii];
      BarColor[4][ii] = red[ii];
    }
  }
  else if (document.getElementById('barcolor').mousedrag.value == "cmy") {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = cyan[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = magenta[ii];
      BarColor[3][ii] = red[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  else if (document.getElementById('barcolor').mousedrag.value == "mcy") {
    for (ii = 0; ii < 4; ii++) {
      BarColor[0][ii] = magenta[ii];
      BarColor[1][ii] = blue[ii];
      BarColor[2][ii] = cyan[ii];
      BarColor[3][ii] = green[ii];
      BarColor[4][ii] = yellow[ii];
    }
  }
  /**/

  free_patch();
  compute_patch_segment();
  Refresh(false);
}
/**
 @brief Glut mouse function

 Modify : ::cx, ::cy, ::scl
*/
/*
function TestGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
  static let dragging = 0;
  static float last_x, last_y;
  let i, j, wheel;
  let dx, dy, a, rot0[3][3], rot1[3][3], ax, ay;

  // Allow default processing to happen, or else the canvas cannot gain focus
  // (for key events).
  event.Skip();

  if (event.LeftIsDown())
  {
    if (!dragging)
    {
      dragging = 1;
    }
    else
    {
      //
      // Translation of mousepointer from starting point
      //
      dx = (event.GetX() - last_x) * sx;
      dy = (event.GetY() - last_y) * sy;
      //
      // Distanse from starting point
      //
      a = Math.sqrt(dx * dx + dy * dy);
      //
      if (lmouse == 1) {
        //
        if (a != 0.0) {
          //
          // Compute rotational matrix from translation of mousepointer
          //
          ax = -dy;
          ay = dx;
          //
          a = a * 10.0;
          //
          rot0[0][0] = (ax * ax + ay * ay * Math.cos(a)) / (ax * ax + ay * ay);
          rot0[0][1] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
          rot0[0][2] = ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[1][0] = ax * ay * (Math.cos(a) - 1.0) / (ax * ax + ay * ay);
          rot0[1][1] = (ax * ax * Math.cos(a) + ay * ay) / (ax * ax + ay * ay);
          rot0[1][2] = ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][0] = -ay * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][1] = -ax * Math.sin(a) / Math.sqrt(ax * ax + ay * ay);
          rot0[2][2] = Math.cos(a);
          //
          for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) rot1[i][j] = rot[i][j];
          //
          for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
              rot[i][j] = rot0[i][0] * rot1[0][j]
                        + rot0[i][1] * rot1[1][j]
                        + rot0[i][2] * rot1[2][j];
            }
          }
          //
          //Print angle to text Box
          //
          thetay = Math.asin(rot[0][2]);
          if (Math.cos(thetay) != 0.0) {
            if (-rot[1][2] / Math.cos(thetay) >= 0.0) thetax = Math.acos(rot[2][2] / Math.cos(thetay));
            else thetax = 6.283185307f - Math.acos(rot[2][2] / Math.cos(thetay));
            if (-rot[0][1] / Math.cos(thetay) >= 0.0) thetaz = Math.acos(rot[0][0] / Math.cos(thetay));
            else thetaz = 6.283185307f - Math.acos(rot[0][0] / Math.cos(thetay));
          }
          else {
            thetax = 0.0;
            if (rot[1][0] >= 0.0) thetaz = Math.acos(rot[1][1]);
            else thetaz = 6.283185307f - Math.acos(rot[1][1]);
          }
          thetax *= 180.0 / 3.14159265;
          thetay *= 180.0 / 3.14159265;
          thetaz *= 180.0 / 3.14159265;
          myf->textbox_rotatex->ChangeValue(wxString::Format(wxT("%f"), thetax));
          myf->textbox_rotatey->ChangeValue(wxString::Format(wxT("%f"), thetay));
          myf->textbox_rotatez->ChangeValue(wxString::Format(wxT("%f"), thetaz));
          myf->Show(true);
        }
      }
      else if (lmouse == 2) {
        scl = scl * expf(-dy);
        myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
        linewidth *= expf(-dy);
        myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
        myf->Show(true);
      }
      else {
        trans[0] = trans[0] + dx;
        trans[1] = trans[1] - dy;
        myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
        myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
        myf->Show(true);
      }
      Refresh(false);
    }
    last_x = event.GetX();
    last_y = event.GetY();
  }
  else
  {
    dragging = 0;
  }

  wheel = event.GetWheelRotation();
  if (wheel > 0) {
    scl = scl * 1.1; 
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 1.1;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  else if (wheel < 0) {
    scl = scl * 0.9;
    myf->textbox_scale->ChangeValue(wxString::Format(wxT("%f"), scl));
    linewidth *= 0.9;
    myf->textbox_linewidth->ChangeValue(wxString::Format(wxT("%f"), linewidth));
    myf->Show(true);
    Refresh(false);
  }
  if (event.LeftDClick()) {
    trans[0] = (event.GetX() * sx * 2.0 - 1.0) / scl;
    trans[1] = -(event.GetY() * sy * 2.0 - 1.0) / scl;
    myf->textbox_positionx->ChangeValue(wxString::Format(wxT("%f"), trans[0]));
    myf->textbox_positiony->ChangeValue(wxString::Format(wxT("%f"), trans[1]));
    Refresh(false);
  }
}
*/
/*
 Allocation of Kohn-Sham energies $ matrix elements
*/
function allocate_griddata(
  ng = [0, 0, 0],
  ng0 = [0, 0, 0]
)
{
  let i = 0, j = 0, ib = 0, i0 = 0, i1 = 0, i2 = 0;

  for (i = 0; i < 3; i++) ng[i] = ng0[i];

  i0 = 0;
  i1 = 1;
  for (i = 0; i < nb; i++) {
    ntri.push(0);
    nnl.push(0);
    n2d.push(0);
    nequator.push(0);
    draw_band.push(1);
    i0 += i1;
    i1 *= 10;
  }
  document.getElementById("band").value = String(i0);
  
  scl /= Math.sqrt(bvec[0][0] * bvec[0][0] + bvec[0][1] * bvec[0][1] + bvec[0][2] * bvec[0][2]);
  linewidth /= scl;
  document.getElementById("scale").value = String(scl);
  document.getElementById("linewidth").value = String(linewidth);
  /*
   Direct lattice vector
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) avec[i][j] = 0.0;
    avec[i][i] = 1.0;
    solve3(bvec, avec[i]);
    terminal("    avec " + String(i + 1) + " : " + String(avec[i][0]) + " "
      + String(avec[i][1]) + " " + String(avec[i][2]) + " \n");
  }/*for (i = 0; i < 3; ++i)*/
  for (i = 0; i < 3; ++i) {
    secvec[i] = bvec[2][i];
    eqvec[i] = bvec[2][i];
    eqvec_fr[i] = 0.0;
    secvec_fr[i] = 0.0;
  }
  eqvec_fr[2] = 1.0;
  secvec_fr[2] = 1.0;
  secscale = 0.0;

  eig0 = [];
  eig = [];
  mat0 = [];
  mat = [];
  vf = [];
  for (ib = 0; ib < nb; ib++) {
    eig0.push([]);
    eig.push([]);
    mat0.push([]);
    mat.push([]);
    vf.push([]);
    for (i0 = 0; i0 < ng0[0]; i0++) {
      eig0[ib].push([]);
      eig[ib].push([]);
      mat0[ib].push([]);
      mat[ib].push([]);
      vf[ib].push([]);
      for (i1 = 0; i1 < ng0[1]; i1++) {
        eig0[ib][i0].push([]);
        eig[ib][i0].push([]);
        mat0[ib][i0].push([]);
        mat[ib][i0].push([]);
        vf[ib][i0].push([]);
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          eig0[ib][i0][i1].push(0.0);
          eig[ib][i0][i1].push(0.0);
          mat0[ib][i0][i1].push([0.0, 0.0, 0.0]);
          mat[ib][i0][i1].push([0.0, 0.0, 0.0]);
          vf[ib][i0][i1].push([0.0, 0.0, 0.0]);
        }
      }
    }
  }
}
function read_from_text(datas) {
  let ib, i, j, i0, i1, i2, ii0, ii1, ii2, iaxis, icount;
  let lshift; //!< Switch for shifted Brillouin zone

  icount = 0;
  terminal("\n");
  terminal("  ##  Brillouin zone informations  ###########\n");
  terminal("\n");
  /*
   k-point grid
  */
  for (i = 0; i < 3; i++) {
    ng0[i] = Number(datas[icount]);
    icount += 1;
  }
  terminal("    k point grid : " + String(ng0[0]) + " " + String(ng0[1]) + " " + String(ng0[2]) + "\n");
  /*
   Shift of k-point grid
  */
  lshift = Number(datas[icount]);
  icount += 1;

  if (lshift == 0) {
    terminal("    k point grid is the Monkhorst-Pack grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = (ng0[i] + 1) % 2;
  }
  else if (lshift == 1) {
    terminal("    k point grid starts from Gamma.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 0;
  }
  else if (lshift == 2) {
    terminal("    k point grid starts from Gamma + a half grid.\n");
    for (i = 0; i < 3; i++) shiftk[i] = 1;
  }
  else {
    exit(0);
  }
  /*
   # of bands
  */
  nb = Number(datas[icount]);
  icount += 1;
  terminal("    # of bands : " + String(nb) + "\n");
  /*
   Reciplocal lattice vectors
  */
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; j++) {
      bvec[i][j] = Number(datas[icount]);
      icount += 1;
    }
    terminal("    bvec " + String(i + 1) + " : " + String(bvec[i][0]) + " " + String(bvec[i][1]) + " " + String(bvec[i][2]) + " \n");
  }/*for (i = 0; i < 3; ++i)*/
  allocate_griddata(ng, ng0);
  /*
   Kohn-Sham energies
  */
  for (ib = 0; ib < nb; ++ib) {
    for (i0 = 0; i0 < ng0[0]; ++i0) {
      if (lshift != 0) ii0 = i0;
      else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
      for (i1 = 0; i1 < ng0[1]; ++i1) {
        if (lshift != 0) ii1 = i1;
        else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
        for (i2 = 0; i2 < ng0[2]; ++i2) {
          if (lshift != 0) ii2 = i2;
          else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
          eig0[ib][ii0][ii1][ii2] = Number(datas[icount]);
          icount += 1;
        }
      }
    }
  }
  /*
   Matrix elements
  */
  for (iaxis = 0; iaxis < 1; iaxis++) {
    for (ib = 0; ib < nb; ++ib) {
      for (i0 = 0; i0 < ng0[0]; ++i0) {
        if (lshift != 0) ii0 = i0;
        else ii0 = modulo(i0 + (ng0[0] + 1) / 2, ng0[0]);
        for (i1 = 0; i1 < ng0[1]; ++i1) {
          if (lshift != 0) ii1 = i1;
          else ii1 = modulo(i1 + (ng0[1] + 1) / 2, ng0[1]);
          for (i2 = 0; i2 < ng0[2]; ++i2) {
            if (lshift != 0) ii2 = i2;
            else ii2 = modulo(i2 + (ng0[2] + 1) / 2, ng0[2]);
            mat0[ib][ii0][ii1][ii2][iaxis] = Number(datas[icount]);
            icount += 1;
          }/*for (i2 = 0; i2 < ng0[2]; ++i2)*/
        }/*for (i1 = 0; i1 < ng0[1]; ++i1)*/
      }/*for (i0 = 0; i0 < ng0[0]; ++i0)*/
    }/*for (ib = 0; ib < nb; ++ib)*/
  }//for (iaxis = 0; iaxis < 1; iaxis++)
  //
  interpol_energy();
  bragg_vector();
  init_corner();
  //
  //Brillouin zone
  //
  bz_lines();
  calc_2dbz();
  //
  max_and_min_bz();
  //
  compute_patch_segment();

  main();
}
/**
 @brief Input from Fermi surface file
*/
function read_file()
{
  const selectedFile = document.getElementById('inputfile').files[0];
  var reader = new FileReader();
  reader.readAsText(selectedFile);
  reader.onload = function (event) {
    var result = event.target.result;
    let datas = result.replace(/\n/g, ' ').replace(/^ +/, '').split(/ +/);

    read_from_text(datas);
  };
  reader.onerror = function () {
    terminal("File can not be loaded.");
  };
  return 1;
} /* read_file */
/**
 @brief Project 3D \f$k\f$-vector into 2D plane. 

 Modify: ::kv2d
*/
function proj_2d(
  axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]],
  vec = [0.0, 0.0, 0.0]//!< [in,out] Line ends to be projected
) 
{
  let ii = 0, kk = 0;
  let vec0 = [0.0, 0.0, 0.0];

  for (kk = 0; kk < 2; kk++) {
    vec0[kk] = 0.0;
    for (ii = 0; ii < 3; ii++)
      vec0[kk] += axis2d[kk][ii] * vec[ii];
  }/*for (kk = 0; kk < 2; kk++)*/
  vec0[2] = 0.0;
  for (kk = 0; kk < 3; kk++)vec[kk] = vec0[kk];
}/*proj_2d*/
/**
 @brief Set Projection axis for 2D plane

 Modify : ::axis2d
*/
function set2daxis(
  secvec = [0.0, 0.0, 0.0],
  axis2d = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
) {
  let ii, jj;
  let snorm, norm, thr = 0.001;

  snorm = 0.0;
  for (ii = 0; ii < 3; ii++) snorm += secvec[ii] * secvec[ii];
  /*
   Define the first axis
  */
  for (ii = 0; ii < 3; ii++) {
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] = 0.0;
    axis2d[0][ii] = 1.0;
    for (jj = 0; jj < 3; jj++) axis2d[0][jj] += - secvec[ii] * secvec[jj] / snorm;

    norm = 0.0;
    for (jj = 0; jj < 3; jj++) norm += axis2d[0][jj] * axis2d[0][jj];
    norm = Math.sqrt(norm);
    if (norm > thr) {
      for (jj = 0; jj < 3; jj++) axis2d[0][jj] /= norm;
      break;
    }/*if (norm > 0.thr)*/
  }/*for (ii = 0; ii < 3; ii++)*/
  /*
   Define the second axis with outor product
  */
  axis2d[1][0] = secvec[1] * axis2d[0][2] - secvec[2] * axis2d[0][1];
  axis2d[1][1] = secvec[2] * axis2d[0][0] - secvec[0] * axis2d[0][2];
  axis2d[1][2] = secvec[0] * axis2d[0][1] - secvec[1] * axis2d[0][0];
  norm = 0.0;
  for (jj = 0; jj < 3; jj++) norm += axis2d[1][jj] * axis2d[1][jj];
  norm = Math.sqrt(norm);
  for (jj = 0; jj < 3; jj++) axis2d[1][jj] /= norm;
}/*function set_2daxis*/
/**
 @brief Judge wheser this line is the edge of 1st BZ (or the premitive BZ)
*/
function bragg_vert2d(
  nbragg = 0,
  bragg=[],
  brnrm=[],
  secvec = [0.0, 0.0, 0.0],
  secscale=0.0,
  jbr =0, //!< [in] Index of a Bragg plane
  nbr = 0, //!< [in]
  vert = [0.0, 0.0, 0.0], //!< [inout] start point of line
  vert2 = [0.0, 0.0, 0.0] //!< [in] end point of line
)
{
  let kbr, i, lbr, nbr0;
  let bmat = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], rhs = [0.0, 0.0, 0.0], prod = 0.0, thr = 0.0, det = 0.0;
  /**/
  nbr0 = nbr;
  /**/
  for (kbr = nbr0; kbr < nbragg; ++kbr) {
    /**/
    /**/
    for (i = 0; i<3; ++i) bmat[0][i] = secvec[i];
    for (i = 0; i<3; ++i) bmat[1][i] = bragg[jbr][i];
    for (i = 0; i<3; ++i) bmat[2][i] = bragg[kbr][i];
    /**/
    rhs[0] = 0.0;
    for (i = 0; i < 3; ++i)rhs[0] += secvec[i] * secvec[i];
    rhs[1] = brnrm[jbr];
    rhs[2] = brnrm[kbr];
    thr = Math.sqrt(rhs[0] * rhs[1] * rhs[2]) * 0.001;
    rhs[0] *= secscale;
    /*
    if Bragg planes do not cross, roop next kbr
    */
    det = solve3(bmat, rhs);
    if (Math.abs(det) < thr) continue;
    /*
    if vert0 = vert1, roop next kbr
    */
    prod = (vert2[0] - rhs[0]) * (vert2[0] - rhs[0])
         + (vert2[1] - rhs[1]) * (vert2[1] - rhs[1])
         + (vert2[2] - rhs[2]) * (vert2[2] - rhs[2]);
    if (prod < thr) continue;
    /*
    is corner really in 1st BZ ?
    */
    i = 0;
    for (lbr = 0; lbr < nbragg; ++lbr) {
      prod = bragg[lbr][0] * rhs[0]
           + bragg[lbr][1] * rhs[1]
           + bragg[lbr][2] * rhs[2];
      /**/
      if (prod > brnrm[lbr] + thr) {
        i = 1;
        break;
      }
    }/*for (lbr = 0; lbr < nbragg; ++lbr)*/
    if (i == 1) {
    }
    else {
      for (i = 0; i<3; ++i) vert[i] = rhs[i];
      return kbr + 1;
    }
  }/*for (kbr = nbr0; kbr < nbragg; ++kbr)*/
  /*
  this line is not a BZ boundary
  */
  return 0;
}/* bragg_vert2d */
/**
 @brief Compute boundary of 2D BZ

 Modify : ::nbzl2d, ::bzl2d_proj
*/
function calc_2dbz() {
  let jbr = 0, nbr = 0, i = 0, j = 0, lvert = 0, ibzl = 0;
  let vert = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], vec = [], prod = 0.0, thr = 0.0;

  if (fbz != 1)return;
  /*
   Set Projection axis for 2D plane
  */
  set2daxis(secvec, axis2d);

  nbzl2d = 0;
  vec = [];
  for (jbr = 0; jbr < nbragg; ++jbr) {
    /**/
    for (i = 0; i < 3; ++i) vert[1][i] = 0.0;
    nbr = 0;
    lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[0], vert[1]);
    if (lvert == 0) continue;
    nbr = lvert;
    /**/
    lvert = bragg_vert2d(nbragg, bragg, brnrm, secvec, secscale, jbr, nbr, vert[1], vert[0]);
    if (lvert == 0) continue;
    /**/
    vec.push([]);
    for (i = 0; i < 2; ++i) {
      vec[nbzl2d].push([]);
      for (j = 0; j < 3; ++j) vec[nbzl2d][i].push(vert[i][j]);
    }
    nbzl2d += 1;
  }/*for (jbr = 0; jbr < nbragg; ++jbr)*/
  /*
   Order bz lines
  */
  bzl2d = [[], []];
  for (i = 0; i < 3; i++) bzl2d[0].push(vec[0][0][i]);
  for (i = 0; i < 3; i++) bzl2d[1].push(vec[0][1][i]);
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {

    thr = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[j][i] * bzl2d[j][i];
    thr *= 0.001;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[j][i] - vec[ibzl][j][i]) * (bzl2d[j][i] - vec[ibzl][j][i]);
    if (prod < thr) 
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;

    prod = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++)
      prod += (bzl2d[1 - j][i] - vec[ibzl][j][i]) * (bzl2d[1 - j][i] - vec[ibzl][j][i]);
    if (prod < thr)
      for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
  }/*for (ibzl = 1; ibzl < *nbzl2d; ibzl++)*/

  for (jbr = 1; jbr < nbzl2d - 1; jbr++) {

    thr = 0.0;
    for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) thr += bzl2d[jbr][i] * bzl2d[jbr][i];
    thr *= 0.001;

    prod = 0.0;
    for (ibzl = 0; ibzl < nbzl2d; ibzl++) for (i = 0; i < 3; i++)
      prod += vec[ibzl][0][i] * vec[ibzl][0][i];
    if (prod < thr) {
      nbzl2d = jbr + 1;
      break;
    }

    for (ibzl = 1; ibzl < nbzl2d; ibzl++) {
      prod = (bzl2d[jbr][0] - vec[ibzl][0][0]) * (bzl2d[jbr][0] - vec[ibzl][0][0])
           + (bzl2d[jbr][1] - vec[ibzl][0][1]) * (bzl2d[jbr][1] - vec[ibzl][0][1])
           + (bzl2d[jbr][2] - vec[ibzl][0][2]) * (bzl2d[jbr][2] - vec[ibzl][0][2]);
      if (prod < thr) {
        bzl2d.push([]);
        for (i = 0; i < 3; i++) bzl2d[jbr + 1][i] = vec[ibzl][1][i];
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }

      prod = (bzl2d[jbr][0] - vec[ibzl][1][0]) * (bzl2d[jbr][0] - vec[ibzl][1][0])
           + (bzl2d[jbr][1] - vec[ibzl][1][1]) * (bzl2d[jbr][1] - vec[ibzl][1][1])
           + (bzl2d[jbr][2] - vec[ibzl][1][2]) * (bzl2d[jbr][2] - vec[ibzl][1][2]);
      if (prod < thr) {
        bzl2d.push([]);
        for (i = 0; i < 3; i++) bzl2d[jbr + 1].push(vec[ibzl][0][i]);
        for (j = 0; j < 2; j++) for (i = 0; i < 3; i++) vec[ibzl][j][i] = 0.0;
      }
    }/*for (ibzl = 1; ibzl < *nbzl2d; ibzl++)*/
  }/*for (ibzl = 1; ibzl < nbzl; ibzl++)*/
  /*
   Project into 2D plane
  */
  bzl2d_proj = [];
  for (ibzl = 0; ibzl < nbzl2d; ibzl++) {
    bzl2d_proj.push([]);
    for (i = 0; i < 3; i++) bzl2d_proj[ibzl].push(bzl2d[ibzl][i]);
    proj_2d(axis2d, bzl2d_proj[ibzl]);
  }/*for (ibzl = 0; ibzl < *nbzl2d; ibzl++)*/
}/*calc_2dbz()*/
/**
 @brief Compute Fermi-line 

 Modify : ::n2d, ::clr2d, ::kv2d
*/
function calc_section() {
  let i = 0, j = 0, ib = 0, itri = 0;
  let kv2d_0 = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]], clr2d_0 = [[0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0]];
  let sw = [0, 0, 0];
  let norm = [0.0, 0.0, 0.0], a = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]];

  if (fbz != 1)return;

  terminal("    band   # of Fermi-line\n");
  kv2d = [];
  clr2d = [];
  for (ib = 0; ib < nb; ib++) {

    kv2d.push([]);
    clr2d.push([]);

    for (itri = 0; itri < ntri[ib]; ++itri) {
      /**/
      for (i = 0; i < 3; i++) {
         norm[i] = 0.0;
        for (j = 0; j < 3; j++) norm[i] += secvec[j] * (kvp[ib][itri][i][j] - secscale * secvec[j]);
      }
      eigsort(3, norm, sw);
      for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
          a[i][j] = (0.0 - norm[sw[j]]) / (norm[sw[i]] - norm[sw[j]]);
        }/*for (j = 0; j < 3; ++j)*/
      }/*for (i = 0; i < 3; ++i)*/
      /**/
      if ((norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]]) || (norm[sw[0]] <= 0.0 && 0.0 < norm[sw[1]])) {
        for (i = 0; i < 3; ++i) {
          kv2d_0[0][i]
              = kvp[ib][itri][sw[1]][i] * a[1][0] + kvp[ib][itri][sw[0]][i] * a[0][1];
          kv2d_0[1][i]
            = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 4; ++i) {
          clr2d_0[0][i]
            = clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][0]
            + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][1];
          clr2d_0[1][i]
            = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
            + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
        }/*for (i = 0; i < 4; ++i)*/
        proj_2d(axis2d, kv2d_0[0]);
        proj_2d(axis2d, kv2d_0[1]);
        kv2d[ib].push(kv2d_0);
        clr2d[ib].push(clr2d_0);
      }/*else if (norm[sw[0]] < 0.0 && 0.0 <= norm[sw[1]])*/
      else if ((norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]]) || (norm[sw[1]] <= 0.0 && 0.0 < norm[sw[2]])) {
        for (i = 0; i < 3; ++i) {
          kv2d_0[0][i]
            = kvp[ib][itri][sw[2]][i] * a[2][0] + kvp[ib][itri][sw[0]][i] * a[0][2];
          kv2d_0[1][i]
            = kvp[ib][itri][sw[2]][i] * a[2][1] + kvp[ib][itri][sw[1]][i] * a[1][2];
        }/*for (i = 0; i < 3; ++i)*/
        for (i = 0; i < 4; ++i) {
          clr2d_0[0][i]
            = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][0]
            + clr[ib][i + 4 * sw[0] + 12 * itri] * a[0][2];
          clr2d_0[1][i]
            = clr[ib][i + 4 * sw[2] + 12 * itri] * a[2][1]
            + clr[ib][i + 4 * sw[1] + 12 * itri] * a[1][2];
        }/*for (i = 0; i < 4; ++i)*/
        proj_2d(axis2d, kv2d_0[0]);
        proj_2d(axis2d, kv2d_0[1]);
        kv2d[ib].push(kv2d_0);
        clr2d[ib].push(clr2d_0);
      }/*else if (norm[sw[1]] < 0.0 && 0.0 <= norm[sw[2]])*/
    }/*for (itri = 0; itri < ntri[ib]; ++itri)*/
    /*
     Allocation of Fermi-lines
    */
    n2d[ib] =  kv2d[ib].length;

    terminal("    " + String(ib + 1) + "       " + String(n2d[ib]) + "\n");
  }/*for (ib = 0; ib < nb; ib++)*/
}/*function calc_nodeline()*/

if (frmsf != "") {
  let datas = frmsf.replace(/\n/g, ' ').replace(/^ +/, '').split(/ +/);
  read_from_text(datas);
}
function resizeCanvas() {
  let el = document.getElementById("glcanvas");
  let width = document.getElementById('width').value;
  let height = document.getElementById('height').value;

  el.setAttribute("width", width);
  el.setAttribute("height", height);
}
