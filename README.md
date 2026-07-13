# C-tetris 
A Tetris clone written in C and SDL2 for learning purposes.

## Dependencies & Building

This project relies on the **SDL2** library and builds on both Windows and Linux/WSL from a single `CMakeLists.txt`. Since binary files are excluded from this repository for portability, you must set up the development environment locally.

> **Note:** The `lib/`, `include/`, `bin/`, and required runtime binaries (such as `SDL2.dll`) are not included in this repository and must be provided locally.

### Prerequisites

-   **C Compiler:** GCC (MinGW for Windows) or Clang/GCC (Linux).
-   **CMake:** Version 3.16 or newer.
-   **SDL2 Development Libraries:** Version 2.32.x (or newer).

---

### Setup for Windows (MinGW)

1.  **Download SDL2:**
    -   Go to the [SDL GitHub Releases](https://github.com/libsdl-org/SDL/releases).
    -   Download the latest MinGW development libraries (e.g., `SDL2-devel-2.32.x-mingw.tar.gz`).

2.  **Install Dependencies:**
    -   Extract the archive, then from the folder for your compiler (e.g., `i686-w64-mingw32`):
    -   **Library Files:** Copy the contents of `lib` into this project's `lib/` directory (this includes `lib/cmake/SDL2/`, which lets CMake locate SDL2 automatically).
    -   **Header Files:** Copy the `include` folder into this project's root directory (so you have an `include/SDL2` folder structure).
    -   **Runtime Binary:** Copy `SDL2.dll` from the `bin` folder (inside the downloaded archive) into this project's own `bin/` directory (create it if it does not exist yet).

3.  **Compile & Run:**
    ```bash
    cmake -B build -S . -G "MinGW Makefiles"
    cmake --build build
    build\ctetris.exe
    ```
    The `-G "MinGW Makefiles"` flag is required: without it, CMake's default generator guess on Windows is often "NMake Makefiles" (Visual Studio's build tool), which fails immediately if only MinGW is installed (`Running 'nmake' '-?' failed`). CMake copies `SDL2.dll` next to the built executable automatically, so no manual copy step is needed after the first setup.

    **If configuring still fails** with `CMAKE_MAKE_PROGRAM is not set`: some MinGW distributions (notably the classic, non -w64 one) name their build tool `make.exe` instead of the `mingw32-make.exe` CMake looks for by default. Find yours and point CMake at it directly:
    ```powershell
    Get-Command make | Select-Object -ExpandProperty Source
    cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="<path from above, with / instead of \>"
    ```

---

### Setup for Linux / WSL

1.  **Install SDL2:**
    ```bash
    sudo apt-get update
    sudo apt-get install libsdl2-dev
    ```

2.  **Compile & Run:**
    ```bash
    cmake -B build -S .
    cmake --build build
    ./build/ctetris
    ```

---

## Build options

Both platforms share the same `CMakeLists.txt`, configurable via `-D` flags at the `cmake -B build -S .` step:

| Option               | Default                              | Purpose                                                                              |
|----------------------|---------------------------------------|---------------------------------------------------------------------------------------|
| `WARNINGS_AS_ERRORS` | `ON`                                 | Treat compiler warnings as errors (`-Werror`)                                        |
| `ENABLE_ASAN`        | `ON` on Linux/macOS, `OFF` on Windows | AddressSanitizer -- off by default on Windows, since classic (non -w64) MinGW does not ship `libasan` |

Example: `cmake -B build -S . -DENABLE_ASAN=OFF -DWARNINGS_AS_ERRORS=OFF`

---

## Memory Safety & Debugging (Linux)

This project is configured with **AddressSanitizer (ASan)** by default on Linux/macOS to detect memory leaks and buffer overflows during development.

### Suppressing External Leaks (Mesa/GLX)
On Linux systems (especially when using WSL with Mesa drivers), you might encounter "false positive" memory leaks originating from the video drivers, not the game code.

To run the game with these external errors filtered out, use the `run` target, which applies the suppressions in `asan.supp`:

```bash
cmake --build build --target run
```