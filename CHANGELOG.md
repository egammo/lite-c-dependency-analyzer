# Changelog

All notable changes to the Enhanced C/C++ Dependency Analyzer will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Include guard detection (`#ifndef` pattern analysis)
- Macro dependency tracking (`#define` usage across files)
- Template analysis for C++ template instantiation mapping
- Parallel processing for large projects
- Real-time IDE integration capabilities
- Database backend with SQLite for complex queries

## [1.3.0] - 2024-12-09

### Added
- **Enhanced Include Resolution Algorithm**: Improved file discovery with multiple search strategies
- **Advanced PRAGMA_PATH Support**: Better handling of complex path configurations with `%EXE_DIR%` variable expansion
- **Comprehensive Code Element Detection**: Extended pattern matching for various code constructs
- **Cross-Platform Path Compatibility**: Enhanced path separator handling and normalization
- **Detailed Performance Analysis**: Extended performance metrics and optimization recommendations
- **Memory Management Improvements**: Better static allocation and bounds checking
- **Enhanced Error Handling**: Improved error messages and graceful failure handling

### Enhanced
- **Pattern Matching Engine**: More sophisticated lexical analysis with context awareness
- **Output Generation**: Richer report format with detailed statistics and recommendations
- **File Discovery**: More robust directory traversal and file resolution
- **Documentation**: Comprehensive developer and user documentation

### Fixed
- **Compiler Compatibility**: Resolved all compilation warnings and errors
- **Memory Initialization**: Fixed uninitialized variable issues
- **Path Resolution**: Better handling of edge cases in include resolution
- **Error Recovery**: Improved stability when encountering malformed files

## [1.2.0] - 2024-11-15

### Added
- **🔄 Circular Include Detection**: Real-time cycle detection during analysis
  - Include stack management for dependency tracking
  - Detailed cycle reporting with full dependency chain
  - Non-blocking warnings (analysis continues after detection)
- **⚡ Performance Problem Detection**: Deep include chain analysis
  - Include depth warnings for chains deeper than 10 levels
  - Compilation performance impact analysis
  - Performance optimization recommendations
- **📊 Enhanced Reporting**: New output sections and statistics
  - Circular include warnings section
  - Performance problem reports
  - Multiple inclusion tracking
  - Enhanced summary statistics with recommendations

### Enhanced
- **Error Handling**: Fixed missing global variable declarations
- **Compiler Compatibility**: Improved Visual Studio compatibility
- **Memory Management**: Better initialization and cleanup
- **Analysis Pipeline**: More robust file processing with cycle prevention

### Technical Improvements
- Added `IncludeStackEntry` structure for cycle detection
- Enhanced `FileAnalysis` structure with performance and circular flags
- Improved include stack management
- Better duplicate detection and handling

### Output Format Changes
- **New Sections**: 
  - `CIRCULAR INCLUDE WARNINGS`
  - `PERFORMANCE WARNINGS` 
  - `RECOMMENDATIONS`
- **Enhanced File Details**: Performance and circular dependency flags
- **Extended Summary**: Additional statistics for cycles and performance issues
- **Backward Compatibility**: All v1.1 sections remain unchanged

### Performance
- Minimal memory overhead (~100KB additional for include stack)
- ~5% processing overhead for cycle detection
- Better scalability for large projects through early cycle detection
- Improved memory utilization through enhanced duplicate detection

## [1.1.0] - 2024-10-20

### Added
- **PRAGMA_PATH Support**: Full support for Lite-C PRAGMA_PATH definitions
  - Automatic path discovery and resolution
  - Global tracking across all files
  - Source file and line number tracking
  - `%EXE_DIR%` variable expansion support
- **Advanced Include Resolution**: Multi-stage include file discovery
  - Current directory search
  - Project root directory search
  - PRAGMA_PATH directory search
  - Recursive fallback search
- **Backslash Detection**: Cross-platform compatibility warnings
  - Detection in include statements
  - Detection in PRAGMA_PATH definitions
  - Compatibility recommendations
- **Code Element Analysis**: Comprehensive code structure detection
  - Function declarations (`void`, `function`, `action`)
  - Structure definitions (`struct`, `typedef struct`)
  - Static variable declarations with scope tracking
  - Function overload detection
  - Static variable duplicate detection

### Enhanced
- **Duplicate Detection**: Hash-table based deduplication
- **Path Normalization**: Better cross-platform path handling
- **Output Generation**: Detailed analysis reports with statistics
- **Memory Management**: Efficient static allocation patterns

### Fixed
- Include resolution edge cases
- Path separator handling
- Memory bounds checking
- Error handling for missing files

## [1.0.0] - 2024-09-15

### Added
- **Core Analysis Engine**: Static analysis for C/C++ and Lite-C codebases
- **Two Analysis Modes**:
  - Include Tracking Mode: Follow dependencies from main file
  - All Files Mode: Scan entire directory tree
- **Basic Code Element Detection**:
  - Include statements with path resolution
  - Function declarations
  - Basic structure detection
- **File Discovery**: Recursive directory traversal for `.c` and `.h` files
- **Report Generation**: Detailed analysis output with file statistics
- **Command Line Interface**: Simple and intuitive CLI
  ```bash
  analyzer.exe <source_path> <main_file> [output_file] [header_file]
  ```

### Core Features
- **Include Dependency Tracking**: Build dependency trees from include statements
- **Directory Structure Analysis**: Understand project organization
- **Basic Pattern Matching**: Detect common code patterns
- **Text Report Output**: Human-readable analysis reports
- **Header File Generation**: Extract declarations for documentation

### Platform Support
- **Windows**: Full support with Visual Studio compatibility
- **File Types**: `.c` and `.h` file analysis
- **Path Handling**: Windows path separator support

---

## Development Milestones

### Version 1.0 → 1.1: Foundation Enhancement
- **Focus**: PRAGMA_PATH support and code element detection
- **Impact**: Made tool practical for real Lite-C projects
- **Key Innovation**: Multi-stage include resolution

### Version 1.1 → 1.2: Quality & Performance
- **Focus**: Circular dependency detection and performance analysis
- **Impact**: Prevented build failures and identified performance bottlenecks
- **Key Innovation**: Real-time cycle detection algorithm

### Version 1.2 → 1.3: Robustness & Scalability
- **Focus**: Enhanced algorithms and better error handling
- **Impact**: Production-ready tool for large projects
- **Key Innovation**: Advanced pattern matching and path resolution

## Future Roadmap

### Short Term (v1.4 - v1.5)
- **Include Guard Analysis**: Detect and analyze header protection patterns
- **Macro Dependency Tracking**: Follow `#define` usage across files
- **Enhanced Template Support**: Better C++ template analysis
- **Performance Optimizations**: Multi-threaded processing for large projects

### Medium Term (v2.0)
- **Database Backend**: SQLite storage for complex queries and caching
- **Web Dashboard**: Interactive dependency visualization
- **Real-time Analysis**: Live analysis during development
- **Plugin Architecture**: Extensible analysis modules

### Long Term (v3.0+)
- **Multi-Language Support**: Support for additional languages
- **Cloud Integration**: Distributed analysis capabilities
- **AI-Powered Insights**: Machine learning for code quality recommendations
- **Enterprise Features**: Advanced reporting and integration capabilities

---

## Breaking Changes

### v1.2.0
- **None**: Fully backward compatible with v1.1
- **Output Format**: New sections added, existing sections unchanged
- **CLI**: No changes to command line interface

### v1.1.0
- **None**: Fully backward compatible with v1.0
- **Output Format**: Enhanced but backward compatible
- **CLI**: No changes to command line interface

---

## Contributors

- **EGAMMO.gaming**: Project creator and maintainer
- **Community Contributors**: Bug reports, feature requests, and testing

## Support

For questions, bug reports, or feature requests:
- **GitHub Issues**: [Repository Issues](../../issues)
- **Community Forum**: [EGAMMO Board](https://board.egammo.eu/)
- **Website**: [EGAMMO.eu](https://egammo.eu/)

---

**Note**: This changelog focuses on user-facing changes. For detailed technical changes, see the [Developer Documentation](docs/dev_documentation.md).