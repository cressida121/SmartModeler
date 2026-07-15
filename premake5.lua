workspace "SmartModeler"
    configurations { "Debug", "Release" }

project "SmartModeler"
    location "out"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    architecture "x64"
    files { "**.h", "**.cpp", "**.c"}

    vpaths {
        ["qt_moc/*"] = "src/moc/moc_MainWindow.cpp",
        ["*"] = "src"
    }

    Qt6_DIR = os.getenv('Qt6_DIR')
    Qt6_INCLUDE_DIR = path.normalize(path.join(Qt6_DIR, "include"))
    Qt6_LIB_DIR = path.normalize(path.join(Qt6_DIR, "lib"))
    
    if os.isdir(Qt6_INCLUDE_DIR) and os.isdir(Qt6_LIB_DIR) then 
        print("Qt6 FOUND")
    else
        error("[FATAL ERROR] - Qt6 NOT FOUND")
    end

    GLM_DIR = os.getenv('GLM_DIR')

    if os.isdir(path.normalize(path.join(GLM_DIR, "glm"))) then 
        print("GLM FOUND")
    else
        error("[FATAL ERROR] - GLM NOT FOUND")
    end

    includedirs {
        GLM_DIR,
        Qt6_INCLUDE_DIR,
        "third_party/glad/include",
        "src"
    }

    libdirs { Qt6_LIB_DIR }

    cppdialect("C++17")
    defines { "_COMPILED_SHADERS_DIR=\"$(SolutionDir.Replace(\'\\\', \'\\\\\'))compiled_shaders\"",
              "_RESOURCES_DIR=\"$(SolutionDir.Replace(\'\\\', \'\\\\\'))resources\"" }
    buildoptions { "/Zc:__cplusplus", "/permissive-", "/fp:fast" }

    prebuildcommands {
        "call $(SolutionDir)src\\shaders\\CompileShaders.bat  $(SolutionDir)compiled_shaders $(SolutionDir)src\\shaders",
        "moc.exe $(SolutionDir)src\\gui\\MainWindow.h -o $(SolutionDir)src\\moc\\moc_MainWindow.cpp"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        links {
            "Qt6OpenGLd.lib",
            "Qt6Widgetsd.lib",
            "Qt6Cored.lib",
            "Qt6Guid.lib",
            "opengl32.lib"
        }

    filter "configurations:Release"
        defines {"NDEBUG"}
        links {
            "Qt6OpenGL.lib",
            "Qt6Widgets.lib",
            "Qt6Core.lib",
            "Qt6Gui.lib",
            "opengl32.lib"
        }
    
