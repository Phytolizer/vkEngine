workspace "HelloWorld"
    configurations { "Debug", "Release" }

project "HelloWorld"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"

    files { "include/**.h", "source/**.c" }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
