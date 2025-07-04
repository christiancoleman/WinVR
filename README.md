# WinVR

Minimal C++ sample using OpenXR. This proof of concept creates an OpenXR instance and demonstrates how to grab a frame of the Windows desktop using the Desktop Duplication API. The captured frame would normally be mapped onto a quad in your OpenXR scene.

## Build

Install Visual Studio with the "Desktop development with C++" workload and the **OpenXR-SDK**. Open a "x64 Native Tools" command prompt and build with CMake:

```bash
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run the resulting executable (`winvr`) while your OpenXR runtime (e.g., SteamVR or Quest Link) is active.

When launched, the program creates an OpenXR instance and attempts to capture one frame of your primary monitor using Direct3D 11's Desktop Duplication API. If capture succeeds, the frame is copied into a texture. Integrating that texture with your OpenXR swapchain is left as an exercise.
