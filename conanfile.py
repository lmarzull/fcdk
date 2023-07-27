from conan import ConanFile
from conans.model.version import Version
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout

class fcdk_recipe(ConanFile):
    name        = "fcdk"
    settings    = "os", "build_type"
    generators  = "CMakeDeps", "VirtualRunEnv"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

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
