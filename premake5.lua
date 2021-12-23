workspace "Lander"
    configurations { "Debug", "Release" }

project "Lander"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    files { "src/**.h", "src/**.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"
        targetdir "bin/Debug"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
        targetdir "bin/Release"