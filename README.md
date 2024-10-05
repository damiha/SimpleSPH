
### Simple SPH simulation

SPH (smooth particle hydrodynamics) is a Lagrangian approach
to solve the Navier-Stokes equations. We consider a finite number of particles that can move around freely (they are not tied to grid points as in an Eulerian method) and transform a partial differential equation into a set of ordinary differential equations.

Below is a little demo of how it looks like (the gif needs a few seconds to loads). Following [this awesome tutorial](https://www.youtube.com/watch?v=-0m05gzk8nk) and in light of the ongoing Oktoberfest in Munich, Germany, I tried to simulate beer (Hefeweizen).

![Video](gifs/beer_sph_3x.GIF)

**NOTE**: On a single laptop CPU, I get ~60 FPS for ~700 particles.
The video recording and conversion to a GIF makes the simulation appear slower than it actually is.

What's implemented?

- smooth particle hydrodynamics with **linked cell** data structure ->
O(N) instead of O(N^2) neighborhood computational complexity in
**C++** with **SFML**

- fragment **shaders** for background (box blur) and fluid surface
in GLSL

- **real time interaction** by moving obstacles (sphere, lines) around
using drag-and-drop

**NOTE**: In order to run the code yourself, SFML needs to be installed using e.g. ```sudo apt install libsfml-dev```. Then you should be able to run ```make ``` and lastly ```./simulation```. Start the simulation by pressing the space bar.

