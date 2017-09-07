# Find DirectX11 SDK
# Define:
# DirectX11_FOUND
# DirectX11_INCLUDE_DIRS
# DirectX11_LIBRARIES

if(WIN32) # The only platform it makes sense to check for DirectX11 SDK
    include(FindPkgMacros)
    findpkg_begin(DirectX11)
    
    # Windows 10 Universal SDK
    if(MSVC14)
        if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
            set(W10SDK_VER ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
        else()
            set(W10SDK_VER 10.0.10240.0)
        endif()
    else()
        set(W10SDK_VER 10.0.15063.0)
    endif()
    
    find_path(DirectX11_INCLUDE_DIR NAMES d3d11.h HINTS "C:/Program Files (x86)/Windows Kits/10/include/${W10SDK_VER}/um" "C:/Program Files/Windows Kits/10/include/${W10SDK_VER}/um")
    set(DirectX11_LIBRARY d3d11.lib dxgi.lib dxguid.lib) # in "C:/Program Files (x86)/Windows Kits/10/lib/${W10SDK_VER}/um/${MSVC_CXX_ARCHITECTURE_ID}/"

    findpkg_finish(DirectX11)
    
endif(WIN32)
