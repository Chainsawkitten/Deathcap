# Find PSAPI
# Define:
# PSAPI_FOUND
# PSAPI_INCLUDE_DIRS
# PSAPI_LIBRARIES

if(WIN32) # The only platform it makes sense to check for PSAPI
    include(FindPkgMacros)
    findpkg_begin(PSAPI)

    # Windows 10 Universal SDK
    if(MSVC14)
        if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
            set(W10SDK_VER ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
        else()
            set(W10SDK_VER 10.0.10240.0)
        endif()
        find_path(PSAPI_INCLUDE_DIR NAMES psapi.h HINTS "C:/Program Files (x86)/Windows Kits/10/include/${W10SDK_VER}/um" "C:/Program Files/Windows Kits/10/include/${W10SDK_VER}/um")
        set(PSAPI_LIBRARY psapi.lib) # in "C:/Program Files (x86)/Windows Kits/10/lib/${W10SDK_VER}/um/${MSVC_CXX_ARCHITECTURE_ID}/"
        
    # Windows Phone 8.1 SDK
    elseif(WINDOWS_PHONE AND MSVC12)
        find_path(PSAPI_INCLUDE_DIR NAMES psapi.h HINTS "C:/Program Files (x86)/Windows Phone Kits/8.1/include" "C:/Program Files/Windows Phone Kits/8.1/include")
        set(PSAPI_LIBRARY psapi.lib) # in "C:/Program Files (x86)/Windows Phone Kits/8.1/lib/${MSVC_CXX_ARCHITECTURE_ID}/"

    # Windows Phone 8.0 SDK
    elseif(WINDOWS_PHONE AND MSVC11)
        find_path(PSAPI_INCLUDE_DIR NAMES psapi.h HINTS "C:/Program Files (x86)/Windows Phone Kits/8.0/include" "C:/Program Files/Windows Phone Kits/8.0/include")
        set(PSAPI_LIBRARY psapi.lib) # in "C:/Program Files (x86)/Windows Phone Kits/8.0/lib/${MSVC_CXX_ARCHITECTURE_ID}/"

    # Windows 8.1 SDK
    elseif(MSVC12)
        find_path(PSAPI_INCLUDE_DIR NAMES psapi.h HINTS "C:/Program Files (x86)/Windows Kits/8.1/include/um" "C:/Program Files/Windows Kits/8.1/include/um")
        set(PSAPI_LIBRARY psapi.lib) # in "C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/${MSVC_CXX_ARCHITECTURE_ID}/"
        
    # Windows 8.0 SDK
    elseif(MSVC11)
        find_path(PSAPI_INCLUDE_DIR NAMES psapi.h HINTS "C:/Program Files (x86)/Windows Kits/8.0/include/um" "C:/Program Files/Windows Kits/8.0/include/um")
        set(PSAPI_LIBRARY psapi.lib) # in "C:/Program Files (x86)/Windows Kits/8.0/lib/win8/um/${MSVC_CXX_ARCHITECTURE_ID}/"
    endif ()

    findpkg_finish(PSAPI)
    
endif(WIN32)
