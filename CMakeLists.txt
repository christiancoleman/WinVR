cmake_minimum_required(VERSION 3.10)
project(WinVR)

find_package(OpenXR QUIET)
if(NOT OpenXR_FOUND)
    message(FATAL_ERROR "OpenXR not found")
endif()

add_executable(winvr
    main.cpp
)

# Windows specific libraries for desktop capture
if(WIN32)
    target_link_libraries(winvr PRIVATE
        OpenXR::openxr_loader
        d3d11 dxgi
    )
    target_compile_definitions(winvr PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
else()
    target_link_libraries(winvr PRIVATE OpenXR::openxr_loader)
endif()
