# Installation Guide

This guide provides step-by-step instructions to install and set up the necessary dependencies and build the project.

## Prerequisites

Make sure you have the following installed on your system:
- `cmake`
- `make`
- `python`
- `conan`
- **A working C/C++ compiler** (such as `gcc` for optimal compatibility)

## Installation Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/ICOMP-UNC/so-i-24-chp2-1v6n.git
   cd so-i-24-chp2-1v6n
   export PROJECT_ROOT=$(pwd)
   ```

2. **Update and install `libmicrohttpd-dev` package:**
   ```bash
   sudo apt update
   sudo apt install libmicrohttpd-dev
   ```

3. **Navigate to the Prometheus C Client Library directory and build it with CMake:**
   ```bash
   cd "$PROJECT_ROOT/submodule/lib/prometheus-client-c/prom"
   mkdir -p build
   cd build
   cmake ..
   make
   sudo make install
   ```

4. **Build and install `promhttp`:**
   ```bash
   cd "$PROJECT_ROOT/submodule/lib/prometheus-client-c/promhttp"
   mkdir -p build
   cd build
   cmake ..
   make
   sudo make install
   ```

5. **Return to the project root directory:**
   ```bash
   cd "$PROJECT_ROOT"
   ```

6. **Install Conan and configure the default profile:**
   ```bash
   pip install conan
   conan profile detect
   ```

7. **Install dependencies with Conan:**
   ```bash
   conan install "$PROJECT_ROOT" --build=missing
   ```

8. **Build the project:**
   ```bash
   mkdir -p "$PROJECT_ROOT/build"
   cd "$PROJECT_ROOT/build"
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make
   ```
   
## Usage

1. **Navigate to the `build` folder:**
   ```bash
   cd "$PROJECT_ROOT/build"
   ```

2. **Run the `ShellProject` executable:**
   ```bash
   ./ShellProject
   ```
