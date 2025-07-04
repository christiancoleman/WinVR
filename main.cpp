#include <openxr/openxr.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <iostream>
#include <cstring>

using Microsoft::WRL::ComPtr;

bool CaptureDesktop(ComPtr<ID3D11Device> device, ComPtr<ID3D11Texture2D>& frame) {
    ComPtr<IDXGIDevice> dxgiDevice;
    if (FAILED(device.As(&dxgiDevice))) return false;
    ComPtr<IDXGIAdapter> adapter;
    if (FAILED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &adapter))) return false;
    ComPtr<IDXGIOutput> output;
    if (FAILED(adapter->EnumOutputs(0, &output))) return false;
    ComPtr<IDXGIOutput1> output1;
    if (FAILED(output.As(&output1))) return false;
    ComPtr<IDXGIOutputDuplication> duplication;
    if (FAILED(output1->DuplicateOutput(device.Get(), &duplication))) return false;

    DXGI_OUTDUPL_FRAME_INFO frameInfo{};
    ComPtr<IDXGIResource> resource;
    HRESULT hr = duplication->AcquireNextFrame(100, &frameInfo, &resource);
    if (FAILED(hr)) return false;
    resource.As(&frame);
    duplication->ReleaseFrame();
    return true;
}

int main() {
    XrInstanceCreateInfo createInfo{XR_TYPE_INSTANCE_CREATE_INFO};
    std::strncpy(createInfo.applicationInfo.applicationName, "WinVR", XR_MAX_APPLICATION_NAME_SIZE);
    createInfo.applicationInfo.applicationVersion = 1;
    std::strncpy(createInfo.applicationInfo.engineName, "WinVREngine", XR_MAX_ENGINE_NAME_SIZE);
    createInfo.applicationInfo.engineVersion = 1;
    createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

    XrInstance instance{XR_NULL_HANDLE};
    XrResult result = xrCreateInstance(&createInfo, &instance);
    if (XR_FAILED(result)) {
        std::cerr << "Failed to create OpenXR instance: " << result << std::endl;
        return -1;
    }

    // Create D3D11 device for desktop capture
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    D3D_FEATURE_LEVEL level;
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                 nullptr, 0, D3D11_SDK_VERSION, &device, &level,
                                 &context))) {
        std::cerr << "Failed to create D3D11 device" << std::endl;
    } else {
        ComPtr<ID3D11Texture2D> frame;
        if (CaptureDesktop(device, frame)) {
            std::cout << "Captured one desktop frame." << std::endl;
        } else {
            std::cerr << "Desktop capture failed" << std::endl;
        }
    }

    xrDestroyInstance(instance);
    std::cout << "OpenXR initialization succeeded." << std::endl;
    return 0;
}
