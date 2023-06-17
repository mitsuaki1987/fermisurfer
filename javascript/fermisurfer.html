<!DOCTYPE html>
<html lang="en">
<head>
  <title>FermiSurfer on Web</title>
  <link rel="shortcut icon" href="../figs/fermisurfer.ico" type="image/vnd.microsoft.icon">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/gl-matrix/2.8.1/gl-matrix-min.js"
          integrity="sha512-zhHQR0/H5SEBL3Wn6yYSaTTZej12z0hVZKOv3TwCUXT1z5qeqGcXJLLrbERYRScEDDpYIJhPC1fk31gqR783iQ=="
          crossorigin="anonymous" defer>
  </script>
  <?php if ($_GET['frmsf'] == ""): ?>
  <script type="text/javascript">
    var frmsf = "";
  </script>
  <?php else: ?>
  <script type="text/javascript" src="<?=$_GET['frmsf']?>"></script>
  <?php endif; ?>
  <script type="text/javascript" src="./fermisurfer.js" defer></script>
</head>
<body>
  <img src="ads.png" height="200" alt="ADVERTISE HERE">
  <br>
  File: <input type="file" id="inputfile" name="inputfile" onchange="read_file();">
  <a href="https://mitsuaki1987.github.io/fermisurfer/ja/_build/html/onweb.html" target="_blank">日本語マニュアル</a> / <a href="https://mitsuaki1987.github.io/fermisurfer/en/_build/html/onweb.html" target="_blank">Manual in English</a>
  <a href="https://mitsuaki1987.github.io/fermisurfer/" target="_blank">FermiSurfer Web page</a>
  <a href="https://doi.org/10.1016/j.cpc.2019.01.017" target="_blank">CPC paper</a>
  <a href="https://arxiv.org/abs/1811.06177" target="_blank">arXiv paper</a>
  <table style="border:0;">
    <tr style="text-align: left;">
      <td><canvas id="glcanvas" width="600" height="600" style="border:solid black 1px;"></canvas></td>
      <td>
        <table style="border:0;">
          <tr style="text-align: left;">
            <td><label>Width: <input type="text" id="width" value="600"></label></td>
            <td><label>Height: <input type="text" id="height" value="600"></label></td>
            <td><input type="button" value="resize" onclick="resizeCanvas();"></td>
          </tr>
          <tr style="text-align: left;">
            <td><input type="button" value="update" onclick="update_delay();"></td>
            <td>Line width :</td>
            <td><label><input type="number" name="linewidth" id="linewidth" value="1" onchange="update_now();" step="0.1" min="0"></label></td>
            <td><label><input type="checkbox" name="ongamma" id="ongamma" value="ongamma" checked="checked">On Gamma</label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Section-v : </td>
            <td><label><input type="number" name="sectionv0" id="sectionv0" value="0"></label></td>
            <td><label><input type="number" name="sectionv1" id="sectionv1" value="0"></label></td>
            <td><label><input type="number" name="sectionv2" id="sectionv2" value="1"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Equator-v : </td>
            <td><label><input type="number" name="equatorv0" id="equatorv0" value="0"></label></td>
            <td><label><input type="number" name="equatorv1" id="equatorv1" value="0"></label></td>
            <td><label><input type="number" name="equatorv2" id="equatorv2" value="1"></label></td>
          </tr>
          <tr style="text-align: left;">
              <td>Interpol ratio : </td>
              <td>
                <label>
                  <input type="number" name="interpol" id="interpol" value="1" step="1" min="1" onchange="update_interpol();">
                </label>
              </td>
            <td>Fermi energy : </td>
            <td>
              <label>
                <input type="number" name="fermienergy" id="fermienergy" value="0.0" onchange="update_efermi();">
              </label>
            </td>
          </tr>
          <tr style="text-align: left;">
            <td>Min. of Scale : </td>
            <td><label><input type="number" name="scalemin" id="scalemin" value="" onchange="update_minmax();"></label></td>
            <td>Max. of Scale : </td>
            <td><label><input type="number" name="scalemax" id="scalemax" value="" onchange="update_minmax();"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Tetrahedron : </td>
            <td></td>
            <td></td>
            <td></td>
          </tr>
          <tr style="text-align: left;">
            <form id="tetrahedron">
              <td>
                <label><input type="radio" name="tetrahedron" value="0" checked="checked" onchange="update_tetra();">0</label>
                <label><input type="radio" name="tetrahedron" value="1" onchange="update_tetra();">1</label>
                <label><input type="radio" name="tetrahedron" value="2" onchange="update_tetra();">2</label>
              </td>
              <td>
                <label><input type="radio" name="tetrahedron" value="3" onchange="update_tetra();">3</label>
                <label><input type="radio" name="tetrahedron" value="4" onchange="update_tetra();">4</label>
                <label><input type="radio" name="tetrahedron" value="5" onchange="update_tetra();">5</label>
              </td>
              <td>
                <label><input type="radio" name="tetrahedron" value="6" onchange="update_tetra();">6</label>
                <label><input type="radio" name="tetrahedron" value="7" onchange="update_tetra();">7</label>
              </td>
              <td>
                <label><input type="radio" name="tetrahedron" value="8" onchange="update_tetra();">8</label>
                <label><input type="radio" name="tetrahedron" value="9" onchange="update_tetra();">9</label>
              </td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <td>Color-scale mode : </td>
            <td></td>
            <td></td>
            <td></td>
          </tr>
          <tr style="text-align: left;">
            <form id="colorscalemode">
              <td><label><input type="radio" name="colorscalemode" value="1" checked="checked" onchange="update_color();">Input(1D)</label></td>
              <td><label><input type="radio" name="colorscalemode" value="2" onchange="update_color();">Input(2D)</label></td>
              <td><label><input type="radio" name="colorscalemode" value="3" onchange="update_color();">Input(3D)</label></td>
              <td><label><input type="radio" name="colorscalemode" value="4" onchange="update_color();">Fermi velocity</label></td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <td>Brillouin zone : </td>
            <form id="brillouinzone">
              <td><label><input type="radio" name="brillouinzone" value="1" checked="checked">First</label></td>
              <td><label><input type="radio" name="brillouinzone" value="-1">Primitive</label></td>
            </form>
            <td></td>
          </tr>
          <tr style="text-align: left;">
            <td>Stereogram : </td>
            <form id="stereogram">
              <td><label><input type="radio" name="stereogram" value="1" checked="checked">None</label></td>
              <td><label><input type="radio" name="stereogram" value="2">Parallel</label></td>
              <td><label><input type="radio" name="stereogram" value="3">Cross</label></td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <td>Mouse drag : </td>
            <form id="mousedrag">
              <td><label><input type="radio" name="mousedrag" value="1" checked="checked">Rotate</label></td>
              <td><label><input type="radio" name="mousedrag" value="2">Scale</label></td>
              <td><label><input type="radio" name="mousedrag" value="3">Translate</label></td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <td>BZ number : </td>
            <td><label><input type="number" name="bznumber1" id="bznumber0" value="1"></label></td>
            <td><label><input type="number" name="bznumber2" id="bznumber1" value="1"></label></td>
            <td><label><input type="number" name="bznumber3" id="bznumber2" value="1"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Backgraound (RGB) : </td>
            <td><label><input type="number" name="backgraoundr" id="backgraoundr" value="0" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
            <td><label><input type="number" name="backgraoundg" id="backgraoundg" value="0" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
            <td><label><input type="number" name="backgraoundb" id="backgraoundb" value="0" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Line color (RGB) : </td>
            <td><label><input type="number" name="linecolorr" id="linecolorr" value="1" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
            <td><label><input type="number" name="linecolorg" id="linecolorg" value="1" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
            <td><label><input type="number" name="linecolobr" id="linecolorb" value="1" onchange="update_now();" step="0.1" min="0" max="1"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td><input type="button" value="Rotate" onclick="update_now();"></td>
            <td><label><input type="number" name="rotatex" id="rotatex" value="0"></label></td>
            <td><label><input type="number" name="rotatey" id="rotatey" value="0"></label></td>
            <td><label><input type="number" name="rotatez" id="rotatez" value="0"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td><label>Scale : <input type="number" name="scale" id="scale" value="1" onchange="update_now();" step="0.1" min="0"></label></td>
            <td>Position : </td>
            <td><label><input type="number" name="positionx" id="positionx" value="0" onchange="update_now();"></label></td>
            <td><label><input type="number" name="positiony" id="positiony" value="0" onchange="update_now();"></label></td>
          </tr>
          <tr style="text-align: left;">
            <td><label><input type="checkbox" name="colorbar" id="colorbar" value="colorbar" checked="checked" onchange="update_now();">Color bar</label></td>
            <td><label><input type="checkbox" name="equator" id="equator" value="equator" onchange="update_now();">Equator</label></td>
            <td><label><input type="checkbox" name="nodalline" id="nodalline" value="nodalline" onchange="update_now();">Nodal line</label></td>
            <td><label><input type="checkbox" name="section" id="section" value="section" onchange="update_now();">Section</label></td>
          </tr>
          <tr style="text-align: left;">
            <td>Lighting : </td>
            <form id="lighting">
              <td><label><input type="radio" name="lighting" value="1.0" checked="checked" onchange="update_now();">Both</label></td>
              <td><label><input type="radio" name="lighting" value="-1.0" onchange="update_now();">Occupy</label></td>
              <td><label><input type="radio" name="lighting" value="1.0" onchange="update_now();">Empty</label></td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <form id="barcolor">
              <td><label><input type="radio" name="barcolor" value="bgr" checked="checked">BGR</label></td>
              <td><label><input type="radio" name="barcolor" value="cmy">CMY</label></td>
              <td><label><input type="radio" name="barcolor" value="mcy">MCY</label></td>
            </form>
          </tr>
          <tr style="text-align: left;">
            <td><input type="button" value="Section file"></td>
            <td><label>Band: <input type="number" name="band" id="band" value="0" onchange="update_now();"></label></td>
          </tr>
        </table>
      </td>
    </tr>
  </table>

  <pre id="log" style="border: 1px solid #ccc; max-width: 80em; overflow: auto; max-height: 10em;"></pre>
</body>
</html>
