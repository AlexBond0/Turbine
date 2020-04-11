# Objects

## Object3D
The Object3D class contains the data required to define an object in the 3D scene. This includes the vertex and polygon data, along with material and textural properties, translation data, and the ability to have child objects appended to a parent object.

The class is derived from the [Material](material.h) and [Moveable](moveable.h) base classes, which give the object material proeprties and translation properties respectively. Currently vertex and poly data is accessed using pointers, as the memory is created for this data on object creation (or if this data changes size during the object's life cycle).

VBO handles are also handled by these objects. Initialising VBO's initialises 2 handles for the object, where the vertex data and polygon data is bound. Handelling VBO data safely per object keeps this OpenGL code cleanly seperated from other areas of Turbine. 

## Material

The Material class defines a Phong-based material property set, including Ambient, Diffuse and Specular colours. the Specular level can also be defined in this class. These elements are protected, and have accessers, mainly used for converting different forms of input to the [color4](Color.h) datatype which colours are stored as.

## Moveable

The Moveable class defines transformation properties, including world and local position, along with translation, rotation and scale properties. This class can be appended to an Object3D to give control over the geometory in the scene. These attributes are stored as glm::vec3, and accessors allow these protected properties to be accessed safely from outside the class.

## Model3D

The Model3D acts as an Object3D collection, with some extra features mainly for loading in pre-modelled objects. The class includes the option to define a base object, and includes special Draw override methods to draw the sub-objects with the correct heirarchy defined in the model. 

Currently the Model3D class also included a model loading function, which can parse special 3dsMax models. This will be broadened (and possibly replaced) by other object file type loaders, including STL and OBJ file types. 

## Primitive

The Primitive class is a child of the Object3D class, with tools for generating primitove objects. These tools include geometory, polygon and uv coordiate generation for multiple base shapes, and include other tools for combining these shapes together to form complex shapes.