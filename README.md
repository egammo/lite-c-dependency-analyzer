# Enhanced C/C++ Code Dependency Analyzer v1.3

A sophisticated static analysis tool designed specifically for analyzing Lite-C/C++ codebases with complex include dependencies. This tool provides comprehensive dependency tracking, circular include detection, and detailed code structure analysis.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

## 🚀 Features

### Core Analysis Capabilities
- **Dependency Tracking**: Follow include chains from main files or scan entire projects
- **Circular Include Detection**: Real-time detection with detailed cycle reporting
- **PRAGMA_PATH Support**: Global tracking of include search paths
- **Code Element Extraction**: Functions, actions, structures, static variables
- **Performance Analysis**: Include depth warnings and optimization suggestions
- **Cross-Platform Compatibility**: Windows path handling with compatibility warnings

### Supported Code Elements
- **Functions**: `void`, `function`, `action` types
- **Structures**: `struct` and `typedef struct` declarations
- **Static Variables**: With function context tracking
- **Include Resolution**: Tracks resolution paths and methods
- **PRAGMA_PATH Definitions**: Source tracking and compatibility analysis

### Analysis Modes
1. **Include Tracking Mode**: Start from a main file and follow dependencies
2. **All Files Mode**: Scan entire directory tree for complete project overview

## 📋 Requirements

- **Platform**: Windows (Visual Studio 2019+ compatible)
- **Dependencies**: Windows API, Standard C library
- **File Types**: `.c` and `.h` files (Lite-C/C++ projects)

## 🛠️ Installation

### Download Release
1. Go to [Releases](../../releases)
2. Download the latest `analyzer.exe`
3. Place in your project directory or PATH

### Compile from Source
```bash
# Using Visual Studio
cl analyzer_v1-3.c /Fe:analyzer.exe

# Or use the included build script
build.bat
```

## 🚀 Usage

### Basic Commands

**Include Tracking (Recommended):**
```bash
analyzer.exe .\my_project main.c analysis.txt declarations.h
```

**Complete Project Scan:**
```bash
analyzer.exe .\my_project "" full_analysis.txt all_declarations.h
```

### Parameters
- `source_path`: Root directory of your project
- `main_file`: Starting file for dependency tracking (use `""` for all files)
- `output_file`: Analysis report filename (optional, defaults to `code_analysis.txt`)
- `header_file`: Generated declarations file (optional, defaults to `declarations.h`)

## 📊 Understanding Analysis Reports

### Report Structure
1. **Header Information**: Analysis mode, file counts, global statistics
2. **Warning Sections**: Circular includes, performance issues, PRAGMA_PATH definitions
3. **Detailed File Analysis**: Per-file breakdown of code elements
4. **Summary Statistics**: Issue counts and structure analysis

### Critical Issues to Address
- 🔴 **Circular Dependencies**: Files that include each other (breaks builds)
- 🟡 **Performance Problems**: Include chains deeper than 10 levels
- 🟡 **Path Compatibility**: Backslash usage in include paths
- 🔵 **Missing Files**: Includes that cannot be resolved

## 🎯 Example Analysis Output

```
=== ENHANCED C/C++ CODE DEPENDENCY ANALYSIS v1.3 ===
Generated: 21:25:33

Analysis Mode: Include Tracking
Total Files Found: 178
Actually Used Files: 178
Files with Circular Includes: 12  ⚠️ CRITICAL
Files with Performance Problems: 0

=== CIRCULAR INCLUDE WARNINGS ===
WARNING: main.c (depth 0) - Part of circular include chain
WARNING: engine.h (depth 1) - Part of circular include chain
```

## 🔧 Best Practices

### Fixing Circular Dependencies
1. **Forward Declarations**: Use forward declarations instead of full includes
2. **Common Headers**: Create shared header files for common types
3. **Dependency Inversion**: Restructure include hierarchy
4. **Include Guards**: Ensure proper include guards in all headers

### Path Compatibility
- Use forward slashes (`/`) instead of backslashes (`\`)
- Update PRAGMA_PATH definitions for cross-platform compatibility
- Avoid platform-specific path constructs

## 📁 Project Structure

```
├── src/
│   ├── analyzer_v1-3.c      # Main analyzer source
│   └── build.bat            # Build script
├── docs/
│   ├── dev_documentation.md # Developer documentation
│   └── user_guide.md        # User guide
├── examples/
│   ├── sample_project/      # Example Lite-C project
│   └── analysis_reports/    # Sample analysis outputs
└── README.md
```

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

### Development Setup
1. Clone the repository
2. Ensure Visual Studio 2019+ is installed
3. Open project in Visual Studio or use command line build
4. Run tests with sample projects

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

**Copyright © 2024 EGAMMO.gaming**
- Website: [https://egammo.eu/](https://egammo.eu/)
- Community: [https://board.egammo.eu/](https://board.egammo.eu/)

## 🐛 Bug Reports & Feature Requests

Please report bugs and request features through [GitHub Issues](../../issues).

### Before Reporting
- Check if the issue already exists
- Provide sample code and analysis output
- Include system information (Windows version, compiler)

## 📈 Roadmap

- [ ] Support for additional C++ features
- [ ] Integration with popular IDEs
- [ ] JSON/XML output formats
- [ ] Web-based report viewer
- [ ] Linux/Mac compatibility

## 🙏 Acknowledgments

- Built for the Lite-C/Acknex community
- Inspired by static analysis best practices
- Special thanks to beta testers and contributors

---

**Made with ❤️ by EGAMMO.gaming - Indie Game Development & Publishing**
