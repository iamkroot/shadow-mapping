# Shadow Mapping in OpenGL

## Algorithm Discussion

There are two main steps involved in the algorithm. First step, involves creating the shadow map, which takes one pass for each light source. The second step is actually shading and rendering the scene according to the saved shadow maps.

### Creating the Shadow map

Consider a single light source in the scene. This step easily extended to other light sources in the scene as well. We create the perspective projection of the scene from the point of of view of the light source. From rendering, we store the depth buffer/ depth map as a texture. This is called a shadow map.

### Shading the scene

The scene is rendered normally first from the point of view of the camera. After doing this, each coordinate in the camera space representing a pixel in the rendered scene is converted into its equivalent coordinate in the light space scene. This is then checked against the depth buffer at the xy coordinate to determine if this point must be shaded in the light or in shadow.
