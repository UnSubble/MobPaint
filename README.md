# MobPaint

**MobPaint** is a lightweight, minimal paint application built with **C** and **SDL2** for Linux systems.  
It is designed to be fast, responsive, and dependency-light — ideal for simple drawing tasks without the overhead of bloated graphics software.

---

## 🚀 Features

- Lightweight and fast (written in C)
- Clean and minimal UI
- Mouse-based drawing with adjustable brush size (coming soon)
- Basic drawing tools (planned: pencil, eraser, fill, shape tools)
- Support for color selection (planned)
- Save drawings to file (planned)
- Logging for debug and stability tracking

---

## 📦 Requirements

| Dependency     | Purpose          |
|----------------|------------------|
| SDL2           | Main graphics/input library |
| SDL2_gfx (optional) | For shapes and advanced rendering (future) |
| gcc / clang    | C compiler |

---

## 🛠 Installation

```bash
git clone https://github.com/yourusername/MobPaint.git
cd MobPaint
chmod +x install.sh
./install.sh
make
./out/mobpaint

```

---

## 📁 Project Structure

```makefile
MobPaint/
├── src/            # Source files
├── include/        # Header files
├── logs/           # Runtime logs
├── assets/         # Icons, brushes, etc. (future)
├── install.sh      # Dependency installer (for Arch)
├── Makefile        # Build system
├── README.md       # Project documentation
├── LICENSE         # MIT license

```

---

## 🧑‍💻 Author

MobPaint is developed and maintained by [UnSubble](https://github.com/unsubble).

## 📜 License

This project is licensed under the **MIT License**. See [LICENSE](#LICENSE) for more details.