# Code Dependency Analyzer - User Guide

## What is the Code Dependency Analyzer?

The Enhanced C/C++ Code Dependency Analyzer is a static analysis tool designed to analyze Lite-C/C++ codebases and reveal how your files are interconnected. It identifies issues like circular dependencies, tracks include resolution paths, and generates comprehensive reports about your code structure and potential problems.

## Usage

### Basic Commands

**Include Tracking (Recommended):**
```bash
analyzer.exe .\my_project main.c analysis.txt declarations.h
```

**Complete Project Scan:**
```bash
analyzer.exe .\my_project "" full_analysis.txt all_declarations.h
```

**Parameters:**
- `source_path`: Root directory of your project
- `main_file`: Starting file for dependency tracking (use `""` for all files)
- `output_file`: Analysis report filename (optional, defaults to `code_analysis.txt`)
- `header_file`: Generated declarations file (optional, defaults to `declarations.h`)

## Understanding the Analysis Report

### 1. Header Information

```
=== ENHANCED C/C++ CODE DEPENDENCY ANALYSIS v1.3 FIXED ===
Generated: 21:25:33

Analysis Mode: Include Tracking
Total Files Found: 178
Actually Used Files: 178
Global PRAGMA_PATH entries found: 24
Files with Circular Includes: 178
Files with Performance Problems: 0
```

**Interpretation:**
- **Analysis Mode:** Shows whether dependency tracking or complete scan was performed
- **Total Files Found:** Number of .c/.h files discovered in the project
- **Actually Used Files:** Files that are actually included in the dependency chain
- **Global PRAGMA_PATH entries:** Count of path definitions for include resolution
- **Circular Includes:** Files participating in circular dependency chains (⚠️ Critical Issue!)
- **Performance Problems:** Files with excessive include depth (>10 levels)

### 2. Warning Sections

#### Circular Include Warnings
```
=== CIRCULAR INCLUDE WARNINGS ===
WARNING: main.c (depth 0) - Part of circular include chain
WARNING: cmd.h (depth 1) - Part of circular include chain
WARNING: winter_fog.h (depth 1) - Part of circular include chain
```

**Analysis:**
- ⚠️ **Circular Dependencies:** Files that include each other directly or indirectly
- **Depth:** Include nesting level in the dependency chain
- **Impact:** Can cause infinite compilation loops and build failures
- **Severity:** Critical - requires immediate attention

#### PRAGMA_PATH Definitions
```
=== PRAGMA_PATH DEFINITIONS ===
0042  #define PRAGMA_PATH "Src" (in main.c)
0043  #define PRAGMA_PATH "Src\ui" (in main.c) [BACKSLASH]
0044  #define PRAGMA_PATH "Src\actor" (in main.c) [BACKSLASH]
```

**Analysis:**
- **PRAGMA_PATH:** Directories searched for include file resolution
- **[BACKSLASH]:** Compatibility warning for path separators
- **Line Number:** Source location of the definition
- **Impact:** Affects include resolution and cross-platform compatibility

### 3. Detailed File Analysis

#### File Header Information
```
=== main.c ===
Directory: .\main.c
Include Depth: 0 [CIRCULAR INCLUDE DETECTED]
Status: USED
Multiple Includes: 2 times
```

**Interpretation:**
- **Directory:** Relative path location of the file
- **Include Depth:** Nesting level (0 = root/main file)
- **Status:** USED = part of dependency chain, UNUSED = orphaned file
- **Multiple Includes:** File is included multiple times (potential optimization)
- **Flags:** Special conditions like circular includes or performance issues

#### Include Resolution Details
```
INCLUDES:
0081  #include "cmd.h"
0082  #include "winter_fog.h" is "winter_fog.h"
0086  #include "Shaders\ppSsao.h" [BACKSLASH]
0087  #include "World/water_effects.h" is "World/water_effects.h"
```

**Analysis:**
- **Line Number:** Source location of the include statement
- **Include Name:** File being included
- **"is" Resolution:** Shows the actual resolved path
- **[BACKSLASH]:** Path separator compatibility warning
- **Resolution Method:** Tracks whether found via current directory, root, or PRAGMA_PATH

#### Code Element Categories

**Void Functions:**
```
VOID FUNCTIONS:
0074  void um_destroy()
0075  void um_open()
0076  void um_popup_update()
```
Functions with no return value.

**Lite-C Functions:**
```
FUNCTIONS:
0600  function objMtlDeferredSwitch_ev()
0610  function mtlSsaoParticle_ev()
0642  function mtlSsaoCombine_ev()
```
Lite-C specific function declarations.

**Actions:**
```
ACTIONS:
0089  action enemy_dynamic_std()
0109  action enemy_dynamic_dumb()
0120  action enemy_riesenrad()
```
Lite-C action definitions for entity behaviors.

**Structure Definitions:**
```
TYPEDEF STRUCTS:
0568  typedef struct SsaoObjMtl
0614  typedef struct technique t {pass p {pixelShader = compile ps_2_0 PS()
```
Data structure declarations and typedefs.

**Static Variables:**
```
STATIC VARIABLES:
1016  static pre (global)
0187  static grpIdx (in function: LfpAddLight)
0188  static subIdx (in function: LfpAddLight)
```
Static variable declarations with scope context (global vs function-local).

### 4. Summary Statistics

```
=== SUMMARY ===
Total Files Analyzed: 178
Files with Circular Includes: 178
Files with Performance Problems: 0
Global PRAGMA_PATH entries: 24
```

## Common Issues and Solutions

### 🔴 Critical: Circular Include Dependencies
**Symptoms:** `Files with Circular Includes: X` where X > 0

**Root Cause:**
- File A includes File B
- File B includes File A (directly or through intermediate files)
- Creates infinite compilation loops

**Solutions:**
1. **Forward Declarations:** Use forward declarations instead of full includes
   ```c
   // Instead of #include "entity.h"
   struct ENTITY; // Forward declaration
   ```

2. **Common Header Approach:** Create shared header files
   ```c
   // common_types.h
   typedef struct ENTITY ENTITY;
   typedef struct PLAYER PLAYER;
   ```

3. **Dependency Inversion:** Restructure include hierarchy
   - Move common dependencies to lower levels
   - Use interfaces/abstract headers

4. **Include Guards:** Ensure proper include guards
   ```c
   #ifndef HEADER_NAME_H
   #define HEADER_NAME_H
   // ... header content ...
   #endif
   ```

### 🟡 Warning: Performance Issues
**Symptoms:** `Files with Performance Problems: X` where X > 0

**Root Cause:**
- Include chains deeper than 10 levels
- Excessive compilation times
- Complex dependency graphs

**Solutions:**
1. **Flatten Include Hierarchy:** Reduce nesting levels
2. **Direct Includes:** Include files directly rather than through chains
3. **Remove Redundant Includes:** Eliminate unnecessary includes
4. **Precompiled Headers:** Use for frequently included headers

### 🟡 Warning: Path Compatibility Issues
**Symptoms:** `[BACKSLASH]` markers in output

**Root Cause:**
- Paths use backslashes (\) instead of forward slashes (/)
- Cross-platform compatibility concerns
- PRAGMA_PATH definitions with Windows-specific separators

**Solutions:**
1. **Use Forward Slashes:** Replace `\` with `/` in all paths
   ```c
   // Good
   #include "src/graphics/renderer.h"
   #define PRAGMA_PATH "src/common"
   
   // Avoid
   #include "src\graphics\renderer.h"
   #define PRAGMA_PATH "src\common"
   ```

2. **Update PRAGMA_PATH Definitions:**
   ```c
   #define PRAGMA_PATH "src/ui"      // ✓ Cross-platform
   #define PRAGMA_PATH "src\ui"      // ✗ Windows-only
   ```

### 🔵 Info: Missing Include Files
**Symptoms:** `"NOT FOUND"` in include lists

**Root Cause:**
- Include file cannot be located
- Incorrect paths or missing PRAGMA_PATH definitions
- File doesn't exist or is misnamed

**Solutions:**
1. **Verify File Existence:** Check if files exist at specified paths
2. **Update PRAGMA_PATH:** Add missing search directories
3. **Fix Path References:** Correct include statements
4. **Copy Missing Files:** Ensure all required files are present

## Best Practices for Clean Code

### ✅ Recommended Practices

**Include Management:**
- **Minimal Include Depth:** Keep to 5-6 levels maximum
- **Forward Declarations:** Reduce coupling between components
- **Logical Organization:** Group related functionality
- **Cross-Platform Paths:** Use forward slashes consistently

**Dependency Design:**
- **Acyclic Dependencies:** Ensure no circular relationships
- **Interface Segregation:** Use thin interface headers
- **Dependency Injection:** Pass dependencies as parameters
- **Layered Architecture:** Maintain clear architectural layers

**File Organization:**
```
project/
├── src/
│   ├── core/          # Core functionality
│   ├── graphics/      # Rendering system
│   ├── physics/       # Physics simulation
│   └── ui/           # User interface
├── include/          # Public headers
└── external/         # Third-party libraries
```

### ❌ Anti-Patterns to Avoid

**Dependency Issues:**
- **Circular Includes:** File A → File B → File A
- **Deep Nesting:** More than 10 include levels
- **Monolithic Headers:** Large headers included everywhere
- **Platform-Specific Paths:** Using backslashes in includes

**Code Organization:**
- **God Headers:** Headers that include everything
- **Tight Coupling:** Files that depend on implementation details
- **Scattered Dependencies:** Random include patterns
- **Inconsistent Naming:** Mixed naming conventions

## Workflow Example

### 1. Initial Analysis
```bash
analyzer.exe .\my_project main.c initial_analysis.txt
```

### 2. Report Review
**Priority Assessment:**
- 🔴 **Circular Includes:** Fix immediately (breaks builds)
- 🟡 **Performance Issues:** Address during refactoring
- 🟡 **Path Compatibility:** Fix for cross-platform support
- 🔵 **Missing Files:** Verify and fix path issues

### 3. Code Improvements

**Phase 1: Critical Issues**
1. Identify circular dependency cycles
2. Break cycles using forward declarations
3. Restructure problematic include chains
4. Verify build functionality

**Phase 2: Optimization**
1. Remove unnecessary includes
2. Optimize include paths
3. Standardize path separators
4. Update PRAGMA_PATH definitions

### 4. Verification
```bash
analyzer.exe .\my_project main.c improved_analysis.txt
```

**Success Metrics:**
- `Files with Circular Includes: 0`
- `Files with Performance Problems: 0`
- No `[BACKSLASH]` warnings
- All includes resolved successfully

## Advanced Features

### Include Resolution Priority
1. **Current Directory:** Same directory as including file
2. **Root Directory:** Project base directory
3. **PRAGMA_PATH Directories:** In definition order
4. **Special Variables:** `%EXE_DIR%` expansion support

### Analysis Modes Comparison

| Feature | Include Tracking | All Files Scan |
|---------|------------------|----------------|
| **Scope** | Dependency chain only | Entire project |
| **Performance** | Fast, focused | Comprehensive |
| **Use Case** | Build analysis | Project overview |
| **Orphan Detection** | No | Yes |
| **Depth Analysis** | Yes | Limited |

### Output Format Reference

| Section | Description | Priority |
|---------|-------------|----------|
| `CIRCULAR INCLUDE WARNINGS` | Dependency cycles | 🔴 Critical |
| `PRAGMA_PATH DEFINITIONS` | Include search paths | 📁 Info |
| `INCLUDES` | File dependencies | 📄 Reference |
| `VOID FUNCTIONS` | C-style functions | ⚙️ Catalog |
| `FUNCTIONS` | Lite-C functions | ⚙️ Catalog |
| `ACTIONS` | Lite-C actions | 🎬 Catalog |
| `TYPEDEF STRUCTS` | Data structures | 🏗️ Catalog |
| `STATIC VARIABLES` | Static declarations | 📊 Catalog |

## Troubleshooting

### Common Error Messages

**"Warning: Include depth limit reached"**
- Cause: Include chain exceeds 10 levels
- Solution: Reduce include hierarchy depth

**"Warning: Could not read file"**
- Cause: File permissions or missing file
- Solution: Check file existence and permissions

**"CIRCULAR INCLUDE DETECTED"**
- Cause: Files include each other in a cycle
- Solution: Use forward declarations or restructure

### Performance Optimization

**Large Projects:**
- Use include tracking mode for faster analysis
- Focus on specific subsystems
- Run analysis on clean, minimal builds

**Memory Constraints:**
- Check `MAX_FILES` and `MAX_ITEMS_PER_FILE` limits
- Increase constants if needed and recompile
- Consider project segmentation for very large codebases

## Integration and Automation

### CI/CD Integration
```bash
# Build check script
analyzer.exe .\source main.c build_analysis.txt
if grep -q "Files with Circular Includes: [^0]" build_analysis.txt; then
    echo "Build failed: Circular dependencies detected"
    exit 1
fi
```

### Pre-commit Hooks
```bash
# Git hook to check dependencies
analyzer.exe . main.c temp_analysis.txt
# Parse results and fail if critical issues found
```

### IDE Integration
Many IDEs can run external tools and display results in the output window, making dependency analysis part of the development workflow.

This tool is essential for maintaining clean, scalable Lite-C/C++ projects and preventing common dependency-related build issues.