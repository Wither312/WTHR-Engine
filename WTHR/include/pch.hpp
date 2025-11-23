// pch.hpp
#pragma once
//Platform specfic
#ifdef _WIN32
#define LEAN_AND_MEAN
#include <Windows.h>
#endif


// Standard library
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <map>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <stdexcept>



// Third-party libs you use often
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <spdlog/spdlog.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>



#include <Camera.hpp>
#include <PrimitiveShape.hpp>
#include <Mesh.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
