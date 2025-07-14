# 🎨 MobPaint

**MobPaint** is a lightweight, minimal paint application built in **C** using **SDL2**, designed for Linux systems.  
It provides a simple, responsive interface for basic drawing tasks without the bloat of complex graphic editors.

---

## 🚀 Features

- ⚡ **Lightweight & Fast** — Built in C with minimal dependencies
- 🖱️ **Mouse-based Drawing** — Responsive freehand brush support
- 🧰 **Tool System** — Modular tools (currently implemented: brush, eraser, line, circle, fill)
- 🎨 **Color Selection** — Palette-based color picking (UI planned)
- ↩️ **Undo/Redo System** — Maintain drawing history with cache-based recovery
- 🗂️ **Session Logging** — Separate logs for errors and session history
- 🖼️ **Planned Features**
  - Adjustable brush size
  - Save/export to image file
  - Shape tools (rectangle, ellipse)
  - Text rendering

---

## 📦 Requirements

| Dependency       | Description                          |
|------------------|--------------------------------------|
| `SDL2`           | Main graphics/input handling         |
| `SDL2_gfx`       | (Optional) Advanced shape rendering  |
| `cJSON`          | Configuration parsing and I/O logs   |
| `gcc` / `clang`  | C compiler                           |
| `make` / `cmake` | Build system support                 |

---

## 🛠 Installation

MobPaint supports multiple major Linux distributions.

### ✅ Automatic Installation

```bash
git clone https://github.com/UnSubble/MobPaint.git
cd MobPaint
chmod +x install.sh
./install.sh
make
./out/mobpaint

```

### ⚙️ Or build with CMake

```bash
mkdir build && cd build
cmake ..
make
./mobpaint

```

---

## 🗂️ Project Structure


MobPaint/

├── src/             # Application source code

│   ├── context/     # Logging, config, paint state

│   ├── tools/       # Brush, eraser, etc.

├── include/         # Public headers

├── logs/            # Runtime logs (errors + history)

├── assets/          # UI assets (future support)

├── install.sh       # Cross-distro dependency installer

├── Makefile         # Simple build system

├── CMakeLists.txt   # CMake build system

├── LICENSE          # MIT License

└── README.md        # This file


---

## 🧪 Development Notes

Uses SDL2 rendering pipeline directly (no GUI toolkit dependency)

Modular tool API enables easy integration of new drawing tools

Log files:

- logs/errors.log — Errors, crashes
- logs/YYYY-MM-DD-history.log — Session actions (for undo/redo and audit)

---

## 👤 Author

MobPaint is developed and maintained by [UnSubble](https://github.com/UnSubble).

Feel free to reach out with feature requests or bug reports!

## 📜 License

This project is licensed under the [MIT License](LICENSE).

See the LICENSE file for details.
