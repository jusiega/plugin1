cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

find_library(LIBRARY1 libfftw3l-3 HINTS "C:/Users/pj/fftw")
find_library(LIBRARY2 libfftw3-3 HINTS "C:/Users/pj/fftw")
find_library(LIBRARY3 libfftw3f-3 HINTS "C:/Users/pj/fftw")

set(vst3sdk_SOURCE_DIR "C:/Users/pj/vst3/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(EQF1
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "EQF1 VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(EQF1
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
)

include_directories("C:/Users/pj/fftw")

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(EQF1
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(EQF1
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(EQF1
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ADD_VSTGUI)
# -------------------

smtg_target_add_plugin_snapshots (EQF1
    RESOURCES
        resource/309B62275D1350D79FAB2403573B6AFD_snapshot.png
        resource/309B62275D1350D79FAB2403573B6AFD_snapshot_2.0x.png
)

target_link_libraries(EQF1
    PRIVATE
        sdk
        ${LIBRARY1}
        ${LIBRARY2}
        ${LIBRARY3}
)

smtg_target_configure_version_file(EQF1)

if(SMTG_MAC)
    smtg_target_set_bundle(EQF1
        BUNDLE_IDENTIFIER wtf
        COMPANY_NAME "pjmw"
    )
    smtg_target_set_debug_executable(EQF1
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(EQF1 PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EQF1)

        smtg_target_set_debug_executable(EQF1
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
