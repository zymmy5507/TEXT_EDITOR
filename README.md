# TEXT_EDITOR

A lightweight, cross-platform text editor built with FLTK (Fast Light Toolkit) and C++. This project provides essential text editing functionality with a clean, intuitive interface designed for everyday document editing tasks.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B11-orange.svg)

## 🚀 Features

### Core Functionality
- **Document Management**: Create, open, save, and manage text documents
- **Text Editing**: Full-featured text editing with cut, copy, paste operations
- **Search & Replace**: Find and replace text with support for batch operations
- **Recent Files**: Quick access to recently opened documents
- **Auto-Save**: Automatic backup system to prevent data loss

### User Experience
- **Dark Mode**: Toggle between light and dark themes
- **Fullscreen Mode**: Distraction-free editing environment
- **Status Bar**: Real-time cursor position and document information
- **Keyboard Shortcuts**: Standard shortcuts for improved productivity
- **Unsaved Changes Warning**: Protection against accidental data loss

### Technical Features
- **Cross-Platform**: Runs on Windows, Linux, and macOS
- **Lightweight**: Minimal resource usage and fast startup
- **Unicode Support**: Handle text in multiple languages and encodings
- **Large File Handling**: Efficient processing of large text documents

## 📸 Screenshots

### Light Mode
![Light Mode Interface](screenshots/light_mode.png)

### Dark Mode
![Dark Mode Interface](screenshots/dark_mode.png)

## 🛠️ Installation

### Prerequisites
- C++11 compatible compiler (GCC 4.9+, Clang 3.4+, or MSVC 2015+)
- FLTK 1.3.x development libraries
- CMake 3.10+ (optional, for build automation)

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt update
sudo apt install build-essential libfltk1.3-dev

# Clone and build
git clone https://github.com/zymmy5507/TEXT_EDITOR.git
cd TEXT_EDITOR
g++ -std=c++11 -O2 simple_editor.cpp -lfltk -o simple_editor
```

### macOS
```bash
# Install FLTK using Homebrew
brew install fltk

# Clone and build
git clone https://github.com/zymmy5507/TEXT_EDITOR.git
cd TEXT_EDITOR
g++ -std=c++11 -O2 simple_editor.cpp -lfltk -o simple_editor
```

### Windows
1. Download and install FLTK from [official website](https://www.fltk.org/software.php)
2. Clone the repository
3. Compile using MinGW or Visual Studio:
```bash
g++ -std=c++11 simple_editor.cpp -lfltk -lole32 -luuid -lcomctl32 -o simple_editor.exe
```

## 🚀 Quick Start

### Basic Usage
```bash
# Start the editor
./simple_editor

# Open a specific file
./simple_editor document.txt
```

### Creating Your First Document
1. Launch the application
2. Start typing your content
3. Save using `Ctrl+S` or File > Save
4. Choose your desired location and filename

## ⌨️ Keyboard Shortcuts

| Function | Shortcut | Description |
|----------|----------|-------------|
| **File Operations** |
| New File | `Ctrl+N` | Create a new document |
| Open File | `Ctrl+O` | Open existing document |
| Save | `Ctrl+S` | Save current document |
| Save As | `Ctrl+Shift+S` | Save with new name |
| Quit | `Ctrl+Q` | Exit application |
| **Editing** |
| Cut | `Ctrl+X` | Cut selected text |
| Copy | `Ctrl+C` | Copy selected text |
| Paste | `Ctrl+V` | Paste clipboard content |
| Select All | `Ctrl+A` | Select entire document |
| **Search** |
| Find | `Ctrl+F` | Open find dialog |
| Replace | `Ctrl+R` | Open replace dialog |
| **View** |
| Fullscreen | `F11` | Toggle fullscreen mode |
| Dark Mode | `Ctrl+D` | Toggle dark theme |

## 🏗️ Building from Source

### Standard Build
```bash
git clone https://github.com/zymmy5507/TEXT_EDITOR.git
cd TEXT_EDITOR
g++ -std=c++11 -O2 simple_editor.cpp -lfltk -o simple_editor
```

### Debug Build
```bash
g++ -std=c++11 -g -DDEBUG simple_editor.cpp -lfltk -o simple_editor_debug
```

### Static Build (Linux)
```bash
g++ -std=c++11 -static -O2 simple_editor.cpp -lfltk -lpthread -ldl -lX11 -o simple_editor_static
```

## 📁 Project Structure

```
TEXT_EDITOR/
├── simple_editor.cpp          # Main application source
├── README.md                  # Project documentation
├── LICENSE                    # MIT License
├── screenshots/               # Application screenshots
│   ├── light_mode.png
│   └── dark_mode.png
├── docs/                      # Additional documentation
│   ├── FEATURES.md
│   ├── BUILDING.md
│   └── CHANGELOG.md
└── .gitignore                 # Git ignore rules
```

## 🔧 Configuration

### Recent Files
The editor maintains a list of recently opened files in `recent.txt`. This file is automatically created and updated as you use the application.

### Auto-Save
Auto-save creates backup files with `.autosave` extension every 60 seconds for files with unsaved changes. These backups are automatically cleaned up when you save your work.

### Theme Settings
Theme preference persists between sessions. The editor remembers your last used theme (light/dark mode) and applies it on startup.

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes and test thoroughly**
4. **Commit your changes**: `git commit -m 'Add amazing feature'`
5. **Push to the branch**: `git push origin feature/amazing-feature`
6. **Open a Pull Request**

### Development Guidelines
- Follow C++11 standards
- Maintain compatibility with FLTK 1.3.x
- Add comments for complex logic
- Test on multiple platforms when possible
- Update documentation for new features

### Reporting Issues
Please use GitHub Issues to report bugs or request features. Include:
- Operating system and version
- FLTK version
- Steps to reproduce the issue
- Expected vs actual behavior

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2024 zymmy5507

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

## 📊 Performance

### System Requirements
- **Minimum RAM**: 64 MB
- **Disk Space**: 5 MB
- **CPU**: Any modern processor (x86/x64/ARM)

### Benchmarks
- **Startup Time**: < 0.5 seconds
- **File Loading**: 100MB+ files supported
- **Memory Usage**: ~10MB baseline
- **Search Performance**: 1M+ character documents

## 🌟 Acknowledgments

- **FLTK Team** - For providing an excellent cross-platform GUI toolkit
- **Contributors** - Everyone who has contributed to this project
- **Community** - Users who provide feedback and suggestions

## 🔗 Related Projects

- [FLTK](https://www.fltk.org/) - The GUI toolkit powering this editor
- [Notepad++](https://notepad-plus-plus.org/) - Advanced text editor for Windows
- [Gedit](https://wiki.gnome.org/Apps/Gedit) - GNOME text editor

## 📞 Support

Having issues? Here are ways to get help:

- **Documentation**: Check the `docs/` directory for detailed guides
- **Issues**: Report bugs on [GitHub Issues](https://github.com/zymmy5507/TEXT_EDITOR/issues)
- **Discussions**: Join project discussions on GitHub
- **Email**: Contact the maintainer for private inquiries

---

**⭐ If you find this project useful, please consider giving it a star on GitHub! ⭐**

---

*Last updated: August 2024*
