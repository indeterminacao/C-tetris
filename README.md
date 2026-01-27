# C-tetris 
A Tetris clone written in C and SDL2 for learning purposes.

## Dependencies & Building

This project relies on the **SDL2** library. Since binary files are excluded from this repository for portability, you must set up the development environment locally.

> **Note:** The `lib/`, `include/`, and required runtime binaries (such as `SDL2.dll`) are not included in this repository and must be provided locally.

### Prerequisites

-   **C Compiler:** GCC (MinGW for Windows) or Clang/GCC (Linux).
-   **SDL2 Development Libraries:** Version 2.32.x (or newer).
-   **Make:** (Optional but recommended for Linux/WSL).

---

### 🪟 Setup for Windows (MinGW)

1.  **Download SDL2:**
    -   Go to the [SDL GitHub Releases](https://github.com/libsdl-org/SDL/releases).
    -   Download the latest MinGW development libraries (e.g., `SDL2-devel-2.32.x-mingw.tar.gz`).

2.  **Install Dependencies:**
    -   Extract the archive.
    -   **Library Files:** Go to the folder for your compiler (e.g., `i686-w64-mingw32`), copy the contents of `lib` into this project's `lib/` directory.
    -   **Header Files:** Copy the `include` folder from the download and paste it into this project's root directory (so you have a `include/SDL2` folder structure).
    -   **Runtime Binary:** Copy `SDL2.dll` from the `bin` folder (inside the downloaded archive) to this project's root directory (next to `main.c`).

3.  **Compile & Run:**
    Open your terminal in the project root and run:
    ```bash
    gcc source_files/*.c -o game.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2
    ./game.exe
    ```

---

### 🐧 Setup for Linux / WSL

1.  **Install SDL2:**
    ```bash
    sudo apt-get update
    sudo apt-get install libsdl2-dev
    ```

2.  **Compile & Run (Using Makefile - Recommended):**
    This project includes a Makefile configured for Linux/WSL.
    ```bash
    make -f Makefile.linux run
    ```

    *Alternatively, strictly manual compilation:*
    ```bash
    gcc source_files/*.c -o game_linux -lSDL2 -lm
    ./game_linux
    ```

---

## Memory Safety & Debugging (Linux)

This project is configured with **AddressSanitizer (ASan)** to detect memory leaks and buffer overflows during development.

### Suppressing External Leaks (Mesa/GLX)
On Linux systems (especially when using WSL with Mesa drivers), you might encounter "false positive" memory leaks originating from the video drivers, not the game code.

To run the game with these external errors filtered out, use the `run` command defined in the Makefile:

```bash
make -f Makefile.linux run