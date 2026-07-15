set OUT_DIR=%1
set SRC_DIR=%2

mkdir %OUT_DIR%

glslc -o %OUT_DIR%\VertexShader.gl -fshader-stage=vertex --target-env=opengl %SRC_DIR%\VertexShader.glsl 
glslc -o %OUT_DIR%\FragmentShader.gl -fshader-stage=fragment --target-env=opengl %SRC_DIR%\FragmentShader.glsl