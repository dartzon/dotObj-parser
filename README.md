# dotObj parser

A C++ parser for Wavefront's .obj 3D files

## Prerequisites

**All OSs:**
```
cmake 3.0 and later
A web browser with WebGL support (All modern Web browsers should work)
```
**Linux:**
```sh
Clang 3.4 and later
```
**Windows:**
```sh
Visual C++ 14 (2015) and later
```

## Getting the project and building

**Linux:**
```sh
git clone https://github.com/dartzon/dotObj-parser
cd dotObj-parser
./build.sh
```
The `build.sh` runs cmake and make automaticaly.

**Windows:**
```sh
git clone https://github.com/dartzon/dotObj-parser
cd dotObj-parser
generateVSProject.bat
```
The `generateVSProject.bat` runs cmake and generates by default a `Visual Studio 2015 solution` in the build directory.

### Build customization

Modify the content of `build.sh` or `generateVSProject.bat` to customize the following build options:

* Build directory.
* Build type (release is the default).

## Usage

The generated binary is located in `build/bin/objparser`. `objparser` is the name of the executable.

If you are using Linux, you can just execute run.sh:

**Linux:**
```sh
./run.sh
```

**Windows:**
```sh
build/bin/objparser
```

**Command line arguments:**

The program takes two optional arguments.

The .obj file path:

**Linux:**
```sh
./run.sh ~/MyObjFiles/Spaceship.obj
```

**Windows:**
```sh
build/bin/objparser c:\Users\Me\Desktop\Spaceship.obj
```

And a scale factor:

**Linux:**
```sh
./run.sh ~/MyObjFiles/Spaceship.obj 5
```

**Windows:**
```sh
build/bin/objparser c:\Users\Me\Desktop\Spaceship.obj 5
```

Running the program with no arguments will do nothing.

After a successful run, go to the folder `out3d` in the base of the project and open the html page `index.html` in a web browser. You should see the 3D content of your .obj file and you can interact with it by zooming and moving the model using the mouse or the keyboard like you would do on any 3D graphics viewer.

If nothing shows up, try to zoom-out with your mouse wheel or change the scale factor argument when executing the program.

### Debug execution

You can see debug messages on the terminal if you compile the program in debug mode and execute it.

```sh
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(39): Obj file parsing started...
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(214): Parsing Vertex
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(304): Parsing Group
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(304): Parsing Group
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(304): Parsing Group
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(304): Parsing Group
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(242): Parsing Face
[DBG INFO] /home/dartzon/Dev-Workspace/CPP/dotObj-parser/src/ObjFileParser.cpp(64): Obj file parsing ended
```

## Running the tests

Automated tests source files are located in `tests/` folder

### Building

Use the argument `-test` with the building scripts to build the test program.

**Linux:**
```sh
./build.sh -test
```

**Windows:**
```sh
generateVSProject.bat -test
```

### Running

Use the argument `-test` with the running script under `Linux` to launch the tests.

**Linux:**
```sh
./run.sh -test
```

**Windows:**
```sh
build/bin/objparser_tests
```

The automated tests will open a 3D .obj file from `tests/models` folder and check for a number of parameters like the count of Vertices, the type of Faces (Triangles or Quads) and the count of Groups (g). Any error will be displayed in the console.

## Built With

* Plain C++ 11/14 and STL
* [Catch 2](http://catch-lib.net) - The test framework for unit-tests
* [three.js](https://threejs.org/) - JavaScript 3D library used to display 3D content in web browsers

## Authors

* **Othmane AIT EL CADI** - *Initial work* - [dartzon](https://github.com/dartzon/)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details

## Documentation sources

* [Object Files (.obj) data format](http://paulbourke.net/dataformats/obj/)
* [three.js documentation](https://threejs.org/docs/index.html#manual/introduction/Creating-a-scene) & [Examples](https://threejs.org/examples/)
