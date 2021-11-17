# ForkerPathTracer: CPU-Based Software Path Tracer 🐰

Implemented a CPU-based software path tracer that generates high-quality image with BVH and multithreading >_<

![](https://raw.githubusercontent.com/forkercat/StorageBaseWithoutCatNotice/main/ForkerPathTracerPic/ForkerPathTracer_Header.jpg)

Bug: some triangles are not rendered...

## Building & Usage 🔨

```sh
# Logging
brew install spdlog  # for macOS

# Clone
git clone https://github.com/forkercat/ForkerPathTracer.git
cd ForkerPathTracer

# Compile
mkdir build && cd build
cmake .. && make

./ForkerPathTracer
```

## Features ⭐

![](https://raw.githubusercontent.com/forkercat/StorageBaseWithoutCatNotice/main/ForkerPathTracerPic/ForkerPathTracer_Light.jpg)

- [x] Path Tracing
- [x] Supported Primitives
    - [x] Sphere
    - [x] Plane
    - [x] Triangle (Möller–Trumbore algorithm)
- [x] Transformations (Translate, Rotate, Scale)
- [x] Supported Materials
    - [x] Lambertian
    - [x] Metal
    - [x] Dielectric
    - [x] Emissive
- [x] Light
    - [x] Area Light
- [x] Anti-Aliasing
- [x] Support Bounding Volume Hierarchy (BVH) acceleration
- [x] Multithreading (`std::thread` and `std::future`)

## Console Output 📜

```console
$ ./ForkerPathTracer
[info] #Threads: 8
[info] Loading OBJ file: obj/chalkboard/chalkboard.obj
[info] [Loader] Loading MTL file: obj/chalkboard/chalkboard.mtl
[info] OBJ Info: v# 1120, vt# 1752, vn# 2504, mesh# 1 | normalized: false, filpTexCoordY: true
[info] >> [chalkboard] #tri 2176
[info] [MeshTriangle <chalkboard>] Building BVH...
[info] [BVHAccel] BVH Generation Complete: 0 hrs, 0 mins, 0 secs
[info] [Scene] Building BVH...
[info] [BVHAccel] BVH Generation Complete: 0 hrs, 0 mins, 0 secs
Progress: [============================================>                         ] 63 %
```


