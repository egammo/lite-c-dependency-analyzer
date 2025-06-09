# Enhanced C/C++ Code Dependency Analyzer v1.3 - Developer Documentation

## Overview

The Enhanced C/C++ Code Dependency Analyzer v1.3 is a sophisticated static analysis tool designed specifically for analyzing Lite-C/C++ codebases with complex include dependencies. It provides comprehensive dependency tracking, circular include detection, and detailed code structure analysis.

## Architecture

### Core Data Structures

#### `FileAnalysis` Structure
```c
typedef struct {
    char filename[MAX_PATH_LEN];           // Base filename
    char filepath[MAX_PATH_LEN];           // Full file path  
    char relative_dir[MAX_PATH_LEN];       // Relative directory
    int is_used;                           // Usage flag
    int include_depth;                     // Nesting level
    int is_performance_problem;            // Performance warning flag
    int is_circular_include;               // Circular dependency flag
    int multiple_include_count;            // Multiple inclusion count
    
    // Code elements arrays
    IncludeElement includes[MAX_ITEMS_PER_FILE];
    CodeElement void_functions[MAX_ITEMS_PER_FILE];
    CodeElement functions[MAX_ITEMS_PER_FILE];
    CodeElement actions[MAX_ITEMS_PER_FILE];
    CodeElement structs[MAX_ITEMS_PER_FILE];
    CodeElement typedef_structs[MAX_ITEMS_PER_FILE];
    CodeElement static_vars[MAX_ITEMS_PER_FILE];
    PragmaPath pragma_paths[MAX_PRAGMA_PATHS];
    
    // Counters
    int include_count;
    int void_function_count;
    int function_count;
    // ... other counters
} FileAnalysis;
```

#### `IncludeElement` Structure
```c
typedef struct {
    char name[MAX_NAME_LEN];               // Include name
    char resolved_path[MAX_PATH_LEN];      // Resolved file path
    char found_in_directory[MAX_PATH_LEN]; // Discovery directory
    int line;                              // Line number
    char type[16];                         // Include type
    int has_backslash;                     // Path separator flag
    int is_system_include;                 // System vs local include
    int found_via_pragma;                  // Found via PRAGMA_PATH
} IncludeElement;
```

#### `PragmaPath` Structure
```c
typedef struct {
    char path[MAX_PATH_LEN];               // PRAGMA_PATH value
    char source_file[MAX_PATH_LEN];        // Source file location
    int line;                              // Line number
    int has_backslash;                     // Path separator flag
} PragmaPath;
```

## Core Algorithms

### 1. Include Resolution Algorithm

The analyzer implements a sophisticated include resolution strategy:

```c
char* find_include_file_enhanced(const char* include_name, const char* current_dir, char* found_directory)
```

**Resolution Priority:**
1. Current directory
2. Base/root directory  
3. All PRAGMA_PATH directories (in order)

**Special Features:**
- Supports `%EXE_DIR%` variable expansion
- Handles both forward slashes and backslashes
- Tracks which method found each include

### 2. Circular Dependency Detection

```c
int check_circular_include(const char* filepath)
```

**Algorithm:**
- Maintains an include stack during traversal
- Compares normalized paths at each level
- Detects cycles by checking if current file is already in stack
- Provides detailed cycle reporting with depth information

### 3. Pattern Matching Engine

The analyzer uses specialized pattern matching for different code elements:

#### Include Patterns
```c
int match_include_enhanced(const char* line, IncludeElement* include_elem)
```
- Supports both `"filename"` and `<filename>` formats
- Detects backslash usage in paths
- Distinguishes system vs local includes

#### Function Patterns
```c
int match_function_declaration(const char* line, const char* type, char* func_name)
```
- Recognizes `void`, `function`, and `action` declarations
- Extracts function names accurately
- Handles various declaration formats

#### PRAGMA_PATH Patterns
```c
int match_pragma_path(const char* line, char* path_value, int* has_backslash_ptr)
```
- Parses `#define PRAGMA_PATH "path"` statements
- Tracks backslash usage for path compatibility warnings

## Analysis Modes

### 1. Include Tracking Mode
```bash
analyzer.exe <source_path> <main_file> [output_file] [header_file]
```

**Behavior:**
- Starts from specified main file
- Follows include dependencies recursively
- Tracks only files that are actually included
- Provides dependency depth information

### 2. All Files Mode  
```bash
analyzer.exe <source_path> "" [output_file] [header_file]
```

**Behavior:**
- Scans entire directory tree
- Analyzes all `.c` and `.h` files found
- Provides complete project overview
- Identifies independent files

## Key Features

### 1. Circular Include Detection
- Real-time detection during traversal
- Detailed cycle reporting with file depths
- Performance impact warnings
- Visual markers in output

### 2. PRAGMA_PATH Support
- Global tracking of all PRAGMA_PATH definitions
- Source file and line number tracking
- Path resolution integration
- Compatibility warnings for backslash usage

### 3. Code Element Extraction
- **Functions:** `void`, `function`, `action` types
- **Structures:** `struct` and `typedef struct` declarations  
- **Static Variables:** Including function context tracking
- **Includes:** With resolution path tracking

### 4. Performance Analysis
- Include depth warnings (depth > 10)
- Multiple inclusion detection
- Heavy dependency identification
- Independent file detection

### 5. Cross-Platform Compatibility
- Windows path handling with `GetFileAttributes()`
- Path separator normalization
- Relative path calculation
- Directory traversal with `WIN32_FIND_DATA`

## Memory Management

### Static Arrays
```c
FileAnalysis files[MAX_FILES];                    // Main file storage
PragmaPath global_pragma_paths[MAX_PRAGMA_PATHS * MAX_FILES];
DirectoryUsage used_directories[MAX_FILES];
IncludeStackEntry include_stack[50];               // Circular detection
```

### Hash Tables for Duplicate Detection
```c
NameCounter function_names[MAX_FILES * 5];         // Function name tracking
NameCounter static_var_names[MAX_FILES * 5];       // Static variable tracking
```

## Output Generation

### Analysis Report Structure
1. **Header Information**
   - Generation timestamp
   - Analysis mode and file counts
   - Global statistics

2. **Warning Sections**
   - Circular include warnings
   - Performance problems
   - PRAGMA_PATH definitions

3. **Detailed File Analysis**
   - Per-file breakdown of all code elements
   - Include resolution details
   - Relative directory information

4. **Summary Statistics**
   - Total files analyzed
   - Issue counts by category
   - Structure analysis results

## Error Handling

### File Access Errors
```c
FILE* file = fopen(filepath, "r");
if (!file) {
    printf("Warning: Could not read file: %s\n", filepath);
    return;
}
```

### Stack Overflow Protection
```c
if (depth > 10) {
    printf("Warning: Include depth limit reached for %s\n", filepath);
    return;
}
```

### Array Bounds Checking
```c
if (file_count >= MAX_FILES) return;
if (analysis->include_count < MAX_ITEMS_PER_FILE) {
    // Safe to add element
}
```

## Configuration Constants

```c
#define MAX_PATH_LEN 512           // Maximum path length
#define MAX_LINE_LEN 1024          // Maximum line length  
#define MAX_NAME_LEN 128           // Maximum name length
#define MAX_FILES 1000             // Maximum files to analyze
#define MAX_ITEMS_PER_FILE 200     // Maximum items per file
#define MAX_INCLUDES 200           // Maximum includes
#define MAX_PRAGMA_PATHS 50        // Maximum PRAGMA_PATH entries
```

## Compilation

### Visual Studio 2019+ Compatible
```bash
cl analyzer_v1-3.c /Fe:analyzer.exe
```

### Dependencies
- Windows API (`windows.h`, `direct.h`)
- Standard C library
- Visual Studio C Runtime

## Usage Examples

### Basic Include Tracking
```bash
analyzer.exe .\source main.c analysis.txt declarations.h
```

### Complete Project Scan  
```bash
analyzer.exe .\source "" full_analysis.txt all_declarations.h
```

### Custom Output Files
```bash
analyzer.exe .\my_project main.c my_analysis.txt my_headers.h
```

## Extension Points

### Adding New Code Element Types
1. Add pattern matching function
2. Extend `CodeElement` structure if needed
3. Add extraction logic in main analysis loop
4. Update output generation

### Supporting New Include Formats
1. Extend `match_include_enhanced()` function
2. Add new include type constants
3. Update resolution logic if needed

### Custom Path Resolution
1. Extend `find_include_file_enhanced()` function
2. Add new resolution strategies
3. Update tracking mechanisms

## Performance Considerations

### Time Complexity
- **Include Tracking:** O(n * d) where n = files, d = average depth
- **All Files Mode:** O(n) where n = total files in directory tree
- **Circular Detection:** O(d²) where d = maximum include depth

### Memory Usage
- Static allocation for predictable memory usage
- Configurable limits via constants
- No dynamic allocation in core analysis

### Optimization Strategies
- Early termination on circular includes
- Duplicate file detection to avoid reprocessing
- Efficient path normalization
- Minimal string operations during analysis

This analyzer is specifically designed for Lite-C projects but can be adapted for general C/C++ codebase analysis with minor modifications.