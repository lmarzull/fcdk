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


  def requirements(self):
    self.requires("spdlog/1.12.0", transitive_headers=True, transitive_libs=True)
    self.requires("doctest/2.4.11", test=True)

  def generate(self):
    tc = CMakeToolchain(self)
    tc.preprocessor_definitions["SPDLOG_FMT_EXTERNAL"] = "1"
    tc.generate()
    deps = CMakeDeps(self)
    deps.generate()


  def validate(self):
    # Use of concept in Plugin.h
    tools.build.check_min_cppstd(self, "20")

  def layout(self):
    cmake_layout(self, build_folder=".build")
    self.cpp.source.includedirs = ["src"]
    self.cpp.build.libdirs = ["src"]

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
    self.cpp_info.requires = ["spdlog::spdlog"]
    self.cpp_info.set_property("cmake_target_name", "fcdk::fcdk")

