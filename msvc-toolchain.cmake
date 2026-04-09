set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang-cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang-cl.exe")
set(CMAKE_LINKER "C:/Program Files/LLVM/bin/lld-link.exe")

set(MSVC_DIR "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207")
set(WINSDK_DIR "C:/Program Files (x86)/Windows Kits/10")
set(WINSDK_VER "10.0.22621.0")

set(CMAKE_C_FLAGS_INIT "/EHsc")
set(CMAKE_CXX_FLAGS_INIT "/EHsc")

include_directories(SYSTEM
    "${MSVC_DIR}/include"
    "${WINSDK_DIR}/Include/${WINSDK_VER}/ucrt"
    "${WINSDK_DIR}/Include/${WINSDK_VER}/um"
    "${WINSDK_DIR}/Include/${WINSDK_VER}/shared"
)

link_directories(
    "${MSVC_DIR}/lib/x64"
    "${WINSDK_DIR}/Lib/${WINSDK_VER}/ucrt/x64"
    "${WINSDK_DIR}/Lib/${WINSDK_VER}/um/x64"
)

set(DIA_SDK "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/DIA SDK")
set(CMAKE_EXE_LINKER_FLAGS_INIT "/LIBPATH:\"${MSVC_DIR}/lib/x64\" /LIBPATH:\"${WINSDK_DIR}/Lib/${WINSDK_VER}/ucrt/x64\" /LIBPATH:\"${WINSDK_DIR}/Lib/${WINSDK_VER}/um/x64\" /LIBPATH:\"${DIA_SDK}/lib/amd64\"")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")
