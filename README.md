# C-tetris
A Tetris clone written in C and SDL2 for learning purposes.

## Dependencies & Building

This project relies on the **SDL2** library. Since binary files are excluded from this repository for portability, you must set up the development environment locally.

> **Note:** The `lib/`, `include/`, and required runtime binaries (such as `SDL2.dll`) are not included in this repository and must be provided locally.

### Prerequisites

-   **C Compiler:** GCC (MinGW for Windows) or Clang.
-   **SDL2 Development Libraries:** Version 2.32.x (or newer).

### 🪟 Setup for Windows (MinGW)

1.  **Download SDL2:**
    -   Go to the [SDL GitHub Releases](https://github.com/libsdl-org/SDL/releases).
    -   Download the latest MinGW development libraries (e.g., `SDL2-devel-2.32.2-mingw.tar.gz`).

2.  **Install Dependencies:**
    -   Extract the archive.
    -   **Library Files:** Go to the folder for your compiler (e.g., `i686-w64-mingw32`), copy the contents of `lib` into this project's `lib/` directory.
    -   **Header Files:** Copy the `include` folder from the download and paste it into this project's root directory (so you have a `include/SDL2` folder structure).
    -   **Runtime Binary:** Copy `SDL2.dll` from the `bin` folder (inside the downloaded archive) to this project's root directory (next to `main.c`).

3.  **Compile:**
    Open your terminal in the project root and run:
    ```bash
    gcc source_files/*.c -o game.exe -I include -L lib -lmingw32 -lSDL2main -lSDL2 
    ```

4.  **Run:**
    -   Run `./game.exe`.

### 🐧 Setup for Linux

1.  **Install SDL2:**
    ```bash
    sudo apt-get update
    sudo apt-get install libsdl2-dev
    ```

2.  **Compile:**
    ```bash
    gcc source_files/*.c -o game -lSDL2 -lm
    ```
    *(Note: The `-lm` flag links the math library, which is required on Linux).*

3.  **Run:**
    ```bash
    ./game
    ```
