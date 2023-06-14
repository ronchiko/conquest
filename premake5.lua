---@diagnostic disable: undefined-global

require "os"

WKS_DIR = os.getcwd()

language "C++"
cppdialect "C++20"
cdialect "C17"

workspace "Conquest"
    kind "ConsoleApp"
    staticruntime "on"

    configurations { 
        "Release", 
        "Debug"
    }
    platforms { 
        "Windows", 
        "Unix" 
    }

    includedirs {
        "include"
    }

    flags {
		"MultiProcessorCompile"
	}

    filter {"configurations:Debug"}
        symbols "on"
        runtime "Debug"

    filter {"configurations:Release"}
        optimize "on"
        runtime "Release"

    filter {"platforms:Windows"}
        system "Windows"
        architecture "x64"

local output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Conquest"
    filter {}
    files {
        "src/**.cpp",
        "include/**.h"
    }

    targetdir ("%{wks.location}/out/" .. output_dir .. "/%{prj.name}")
    objdir ("%{wks.location}/obj/" .. output_dir .. "/%{prj.name}")
