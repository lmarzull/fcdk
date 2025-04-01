from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class fcdk_recipe(ConanFile):
  name        = "fcdk"
  version     = "1.0.0"
  license     = ""
  author      = "laurent Marzullo laurent.marzullo at gmail.com"
  url         = "https://github.com/lmarzull/fcdk"
  description = "Fratal C++ Development kit"
  topics      = ("C++")

  settings        = "os", "build_type", "arch", "compiler"
  options         = {"shared": [True, False]}
  default_options = {"shared": True}
  generators      = "VirtualRunEnv"

  requires = [
            "doctest/2.4.11",
            "spdlog/1.12.0"
          ]

  def validate(self):
    # Use of concept in Plugin.h
    tools.build.check_min_cppstd(self, "20")

  def layout(self):
      cmake_layout(self, build_folder=".build")
      self.cpp.source.includedirs = ["src"]
      self.cpp.build.libdirs = ["src"]

  def generate(self):
    tc = CMakeToolchain(self)
    tc.generate()

    deps = CMakeDeps(self)
    deps.generate()


  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["fcdk"]
    self.cpp_info.libdirs = ["lib"]
    self.cpp_info.includedirs = ["include"]
