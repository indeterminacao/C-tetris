# C-tetris 
A Tetris clone written in C and SDL2 for learning purposes.

## Dependencies & Building

This project relies on the **SDL2** library, discovered via `pkg-config`, and builds on Windows, Linux, and WSL from a single `CMakeLists.txt`.

### Prerequisites

-   **C Compiler:** [MSYS2](https://www.msys2.org/) (UCRT64 environment) on Windows, or Clang/GCC on Linux/WSL.
-   **CMake:** Version 3.16 or newer.
-   **SDL2 Development Libraries:** Version 2.32.x (or newer), providing a `pkg-config` `.pc` file.

---

### Setup for Windows (MSYS2)

1.  **Install [MSYS2](https://www.msys2.org/)** (or `winget install MSYS2.MSYS2`).

2.  **Install the toolchain and SDL2** — open the **MSYS2 UCRT64** terminal specifically (not the plain "MSYS2" one) and run:
    ```bash
    pacman -Syu
    pacman -S mingw-w64-ucrt-x86_64-toolchain \
              mingw-w64-ucrt-x86_64-cmake \
              mingw-w64-ucrt-x86_64-ninja \
              mingw-w64-ucrt-x86_64-pkgconf \
              mingw-w64-ucrt-x86_64-SDL2 \
              mingw-w64-ucrt-x86_64-SDL2_image \
              mingw-w64-ucrt-x86_64-SDL2_ttf \
              mingw-w64-ucrt-x86_64-SDL2_mixer
    ```
    `pacman` installs headers, libs, the runtime DLL, and a `pkg-config` `.pc` file, all correctly in place — no manual downloading or copying files into the project. (`SDL2_image`/`SDL2_ttf`/`SDL2_mixer` aren't linked by the build yet — they're here ahead of planned text, image, and sound features.)

3.  **Compile & Run**, from the same UCRT64 terminal, in the project directory:
    ```bash
    cmake -B build -S . -G Ninja
    cmake --build build
    ./build/ctetris.exe
    ```
    `SDL2.dll` is located automatically from the MSYS2 UCRT64 installation and copied next to the executable — no manual DLL placement needed.

> **Note:** > Note: The recommended environment is the MSYS2 **UCRT64** terminal. Regular PowerShell/cmd terminals do not automatically have access to the MSYS2 toolchain unless configured manually.

---

### Setup for Linux / WSL

1.  **Install SDL2:**
    ```bash
    sudo apt-get update
    sudo apt-get install libsdl2-dev pkg-config
    ```

2.  **Compile & Run:**
    ```bash
    cmake -B build -S .
    cmake --build build
    ./build/ctetris
    ```

---

## Build options

All platforms share the same `CMakeLists.txt`, configurable via `-D` flags at the `cmake -B build -S .` step:

| Option               | Default                              | Purpose                                                                              |
|----------------------|---------------------------------------|---------------------------------------------------------------------------------------|
| `WARNINGS_AS_ERRORS` | `ON`                                 | Treat compiler warnings as errors (`-Werror`)                                        |
| `ENABLE_ASAN`        | `ON` on Linux/macOS, `OFF` on Windows | AddressSanitizer -- off by default on Windows. AddressSanitizer is enabled by default on Unix-like systems and disabled by default on Windows. Some MinGW/MSYS2 GCC toolchains do not provide the required libasan runtime. If your toolchain supports it, you can enable it manually with -DENABLE_ASAN=ON. |

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