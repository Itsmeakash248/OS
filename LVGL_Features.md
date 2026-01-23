# LVGL v9.5.0-dev Developer Reference

## 1. Core Architecture & Kernel (`src/core`)
The v9 core has been refactored for parallelism, memory safety, and reactive programming.

*   **Multi-Instance Support (`lv_global`):** All global state is encapsulated, allowing multiple independent LVGL instances (e.g., spanning multiple displays) in a single process.
*   **Reactive UI / Data Binding (`lv_observer`):** A native implementation of the observer pattern. Widgets can bind to data sources, updating automatically when variables change.
*   **Unified Property System (`lv_obj_property`):** Generic access to widget properties via ID or Name, facilitating script bindings (Python/Lua) and runtime introspection.
*   **Object System:**
    *   **Base:** `lv_obj`, `lv_obj_class`, `lv_obj_tree`.
    *   **Events:** `lv_obj_event` with bubbling and filtering.
    *   **Groups:** `lv_group` for input device grouping (keypad/encoder navigation).
    *   **Styles:** CSS-like styling system (`lv_obj_style`) supporting state inheritance (hover, pressed, focused) and transitions.

## 2. Widgets & Layouts (`src/widgets` & `src/layouts`)
A complete list of built-in UI components and positioning engines.

### Layout Engines
*   **Flexbox (`flex`):** CSS-compliant 1D layout for responsive rows/columns.
*   **Grid (`grid`):** CSS-compliant 2D layout for complex cell-based arrangements.

### Widget Library
*   **Basic:** `button`, `label`, `image` (supports distinct sources), `line`, `bar`, `slider`, `switch`, `checkbox`.
*   **Navigation:** `menu` (multi-level), `tabview`, `tileview` (screen swiping), `win` (header + content window).
*   **Data & Lists:** `list`, `dropdown`, `roller`, `table`, `buttonmatrix`.
*   **Input:** `keyboard` (touchscreen), `textarea`, `spinbox`, `ime` (Input Method Editor for CJK).
*   **Indicators:** `arc`, `spinner`, `led`, `scale` (linear/round gauges), `chart` (line/bar/scatter), `calendar`.
*   **Rich Media:**
    *   **`canvas`:** Direct per-pixel drawing buffer.
    *   **`animimage`:** Sequence-based image animation.
    *   **`span`:** Rich text (multiple fonts/colors in one block).
    *   **`lottie`:** Vector animation playback (JSON based).
    *   **`3dtexture`:** Mapping 2D textures onto 3D simulated surfaces.

## 3. Graphics, Rendering & Acceleration (`src/draw`)
LVGL v9 features a decoupled rendering pipeline supporting software fallback, vector graphics, and hardware acceleration.

### Software Rendering (`sw`)
*   **Optimized Engines:**
    *   **Arm-2D:** Integration for Cortex-M acceleration.
    *   **SIMD:** Assembly optimizations for **Neon** (Arm Cortex-A) and **Helium** (Arm Cortex-M Vector extension).
*   **Capabilities:** Anti-aliasing, advanced blending (Add, Sub, Multiply), linear/radial/conical gradients, and real-time Gaussian blur.

### Vector Engines
*   **ThorVG (`vector`):** Integrated SVG and Lottie rendering.
*   **VG-Lite / NemaVG:** Hardware-accelerated vector path drawing.

### Hardware Acceleration (GPU Drivers)
Drivers located in `src/draw` that offload rendering from the CPU:
*   **Espressif:** PPA (Pixel Processing Accelerator) for ESP32-P4.
*   **STM32:** DMA2D (Chrom-ART).
*   **NXP:** PXP and G2D subsystems.
*   **Renesas:** Dave2D.
*   **Think Silicon:** NemaGFX.
*   **Generic/Desktop:** OpenGL ES 2.0+, SDL2 texture rendering.

## 4. Display & Input Ecosystem
Support for specific hardware controllers and platform abstraction.

### Supported Display Controllers (IC)
Drivers available for direct embedded integration:
*   **Ilitek:** ILI9341.
*   **Sitronix:** ST7735, ST7789, ST7796.
*   **Others:** NXP ELCDIF, ST LTDC, Renesas GLCDC.

### Input Systems (`src/indev`)
*   **Device Types:** Pointer (Mouse/Touch), Keypad, Encoder, Button.
*   **Gestures:** Swipe, Long-press, Multi-touch gestures.
*   **Navigation:** "Grid Navigation" for non-touch interfaces (focus movement logic).

### OS & Platform Integration (`src/osal`)
*   **RTOS:** FreeRTOS (native sync), RT-Thread, CMSIS-RTOS2, MQX.
*   **Desktop/High-Level:** Linux (FBDev, DRM/KMS, Wayland, X11), Windows (Win32), SDL2, QNX.
*   **Frameworks:** Arduino, Zephyr, ESP-IDF, NuttX.
*   **Languages:** C, C++, MicroPython, PikaScript.

## 5. Multimedia & Assets (`src/libs`)
Integration with external libraries for handling rich media.

*   **Image Decoders:**
    *   **Standard:** PNG (`libpng`, `lodepng`), JPEG (`libjpeg_turbo`, `tjpgd`), BMP, GIF.
    *   **Modern/Optimized:** WebP, LZ4 compression, RLE.
*   **Font Engines:**
    *   **`freetype`:** TTF/OTF support.
    *   **`tiny_ttf`:** Lightweight TrueType renderer.
    *   **`font_manager`:** Caching and recycling system for loaded fonts.
*   **Video:** FFmpeg and GStreamer interfaces.
*   **Vector/3D:** `thorvg` (SVG), `gltf` (3D Models), `rlottie`.
*   **File Systems:**
    *   **`fsdrv`:** Drivers for FATFS, LittleFS, POSIX/STDIO, and Windows.
    *   **`frogfs`:** Internal system for managing asset blobs.

## 6. Utilities & Internationalization
*   **XML UI Engine (`src/xml`):** Build UIs declaratively using XML at runtime.
*   **Internationalization (`src/font` & `src/others`):**
    *   **BiDi:** Full bidirectional text support (RTL/LTR) with Arabic/Persian shaping.
    *   **Translation:** Runtime language switching module.
    *   **IME:** Input Method Editors for complex scripts.
*   **Memory Management:** `stdlib` wrappers allowing usage of built-in TLSF allocator or system `malloc`.

## 7. Developer Tools & Debugging
Tools for performance tuning and validation.

*   **Profiling (`src/misc/profiler`):**
    *   **Performance Monitor:** Overlay showing FPS and CPU load.
    *   **Memory Monitor:** Overlay showing Heap usage and fragmentation.
    *   **Trace:** Instrumentation for Layout, Draw, and Input cycles.
*   **Testing:**
    *   **Snapshot:** Capture widgets/screen to image for regression testing.
    *   **Monkey Test:** Automated random input generation for stress testing.
*   **Pre-built Demos (`demos`):** `music` player, `benchmark` suite, `stress` test, and `widgets` showcase.