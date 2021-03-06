# CppMol
CppMol is a visualization tool for macromolecules found in the [Protein Data Bank](http://www.rcsb.org/). It's written in C++ with OpenGL, allowing it to achieve high performance when rendering complex molecules.

## Dependencies
* [libcurl](https://curl.haxx.se/libcurl/): For fetching online PDB files
* [GLFW](https://github.com/glfw/glfw): For handling windows and user inputs
* [GLEW](https://github.com/nigels-com/glew)

## Demo
Watch CppMol render the SARS-CoV-2 spike glycoprotein ([6VXX](http://www.rcsb.org/structure/6VXX)) [here](https://youtu.be/duECJbRdzNk).

**Protein [7BWJ](http://www.rcsb.org/structure/7BWJ) (SARS-CoV-2 antibody with RBD):**
<div style="text-align: center;">
    <img src="7BWJ_1.png" alt="7BWJ Carbon Backbone and Atoms" width="200"/>
    <img src="7BWJ_2.png" alt="7BWJ Carbon Backbone" width="200"/>
    <img src="7BWJ_3.png" alt="7BWJ Carbon Backbone (Colored by Secondary Structure)" width="200"/>
</div>

## Commands
### Load a PDB file from a URL
```
load pdb <URL>
```

### Load a PDB file with the matching PDB ID
```
load pdb <PDB ID>
```

### Print molecule data
```
print [sequence | helices | sheets | ssbonds | atoms | chains | selection]
```

### Select molecule region
```
select [r=<RESIDUE_START>:<RESIDUE_END> | r=<RESIDUE>] [e=<ELEMENT>] [c=<CHAIN>]
```

### Restrict molecule region
```
restrict [r=<RESIDUE_START>:<RESIDUE_END> | r=<RESIDUE>] [e=<ELEMENT>] [c=<CHAIN>]
```
Restricting selects the specified region and hides all other parts of the molecule.

### Change component sizes
```
atom <SIZE>
```
```
backbone <SIZE>
```
```
ssbond <SIZE>
```
Where `SIZE` is an integer between 0 and 100.

### Color components
```
color [atom | backbone | ssbond] <R> <G> <B>
```
Where `R`, `G`, and `B` are integers between 0 and 255.

```
color [atom | backbone | ssbond] <COLOR_NAME>
```
Where `COLOR_NAME` is any of the following colors: red, green, blue, orange, yellow, purple, white, light-gray, dark-gray, brown, black.

```
color [atom | backbone | ssbond] default
```
Color the molecule's selected components based on their elements (e.g. nitrogen is displayed as blue).

```
color [atom | backbone | ssbond] structure
```
Color the molecule's selected components based on the secondary protein structure.

### Additional commands
Use `unload` to remove the currently-loaded molecule from the window.

Use `reset` to undo all graphical changes made to the model.

## Window Controls
* **Left click and drag:** Rotate about X and Y axes
* **Right click and drag:** Rotate about Z axis
* **Ctrl + Left click and drag**: Pan
