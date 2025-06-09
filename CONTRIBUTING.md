# Contributing to Enhanced C/C++ Dependency Analyzer

Thank you for considering contributing to the Enhanced C/C++ Dependency Analyzer! This tool is specifically designed for Lite-C/C++ codebases and we welcome contributions from the community.

## 🤝 How to Contribute

### Reporting Bugs

Before creating a bug report, please check the [existing issues](../../issues) to avoid duplicates.

**Please include:**
- Operating System and version
- Visual Studio/Compiler version
- Sample code that reproduces the issue
- Expected vs actual behavior
- Full error message or analyzer output

**Use this template:**
```
**Environment:**
- OS: Windows 10/11
- Compiler: Visual Studio 2019/2022
- Analyzer Version: v1.3

**Description:**
A clear description of the bug.

**Steps to Reproduce:**
1. Run analyzer with: `analyzer.exe .\project main.c`
2. Observe behavior...

**Expected Behavior:**
What should happen.

**Actual Behavior:**
What actually happens.

**Sample Code:**
```c
// Minimal code that reproduces the issue
```

### Suggesting Features

We welcome feature suggestions! Please:

1. Check if the feature already exists in newer versions
2. Search existing [feature requests](../../issues?q=is%3Aissue+label%3Aenhancement)
3. Provide a clear use case for the feature
4. Consider if it fits the tool's scope (Lite-C/C++ dependency analysis)

**Feature areas we're interested in:**
- Enhanced Lite-C language support
- Performance optimizations
- Better error reporting
- IDE integrations
- Cross-platform compatibility
- Additional code element detection

### Code Contributions

#### Development Setup

1. **Prerequisites:**
   - Windows 10/11
   - Visual Studio 2019+ or compatible C compiler
   - Git

2. **Setup:**
   ```bash
   git clone https://github.com/your-username/lite-c-dependency-analyzer.git
   cd lite-c-dependency-analyzer
   ```

3. **Build:**
   ```bash
   # Using Visual Studio
   cl analyzer_v1-3.c /Fe:analyzer.exe
   
   # Or use build script
   build.bat
   ```

4. **Test:**
   ```bash
   # Test with sample project
   analyzer.exe .\examples\sample_project main.c test_output.txt
   ```

#### Development Guidelines

**Code Style:**
- Follow existing C code style (K&R style with modifications)
- Use descriptive variable names
- Add comments for complex algorithms
- Maintain consistent indentation (4 spaces)

**Architecture Principles:**
- Static memory allocation for predictable performance
- Modular pattern matching engine
- Clear separation between analysis and output generation
- Maintain backward compatibility when possible

**Key Files to Understand:**
```
analyzer_v1-3.c          # Main analyzer source
├── Core Data Structures  # FileAnalysis, IncludeElement, etc.
├── Pattern Matching     # Function/struct/include detection
├── Path Resolution      # PRAGMA_PATH and include resolution
├── Circular Detection   # Cycle detection algorithm
├── Performance Monitor  # Deep include chain detection
└── Output Generation    # Report formatting
```

#### Making Changes

1. **Fork the repository**
2. **Create a feature branch:**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b bugfix/issue-number
   ```

3. **Make your changes:**
   - Keep commits focused and atomic
   - Write clear commit messages
   - Add tests if applicable

4. **Test thoroughly:**
   - Test with various Lite-C projects
   - Verify all analysis modes work
   - Check for memory leaks
   - Test edge cases (circular includes, deep hierarchies)

5. **Update documentation:**
   - Update developer docs if changing core algorithms
   - Update user guide if changing CLI or output format
   - Add examples for new features

6. **Submit a pull request:**
   - Reference related issues
   - Explain your changes clearly
   - Include before/after examples if applicable

#### Pull Request Guidelines

**Title Format:**
- `feat: Add support for XYZ feature`
- `fix: Resolve circular include detection bug`
- `docs: Update installation instructions`
- `perf: Optimize include resolution algorithm`

**Description Template:**
```markdown
## Changes Made
- Brief description of changes

## Testing Done
- [ ] Tested on sample Lite-C projects
- [ ] All analysis modes work correctly
- [ ] No memory leaks detected
- [ ] Edge cases handled properly

## Related Issues
Fixes #123

## Screenshots/Examples
(If applicable)
```

### Testing Guidelines

#### Required Tests
1. **Basic Functionality:**
   ```bash
   analyzer.exe .\test_project main.c output.txt
   # Verify normal analysis works
   ```

2. **Circular Include Detection:**
   ```bash
   # Test with project containing circular includes
   analyzer.exe .\circular_test main.c circular_output.txt
   # Should detect and report cycles without crashing
   ```

3. **Performance Analysis:**
   ```bash
   # Test with deep include hierarchies
   analyzer.exe .\deep_hierarchy main.c perf_output.txt
   # Should flag performance problems
   ```

4. **PRAGMA_PATH Resolution:**
   ```bash
   # Test projects with complex path setups
   analyzer.exe .\pragma_test main.c pragma_output.txt
   # Should resolve all includes correctly
   ```

#### Test Data
Create test projects that cover:
- Simple single-file projects
- Multi-directory structures
- Circular include scenarios
- Deep include hierarchies (>10 levels)
- Missing include files
- PRAGMA_PATH configurations
- Mixed path separator styles

## 🎯 Development Priorities

### High Priority
- Bug fixes for critical issues
- Memory leak fixes
- Crash prevention
- Windows compatibility improvements

### Medium Priority
- Performance optimizations
- Enhanced error messages
- Additional Lite-C language features
- Better path handling

### Low Priority
- UI improvements
- Additional output formats
- IDE integrations
- Cross-platform support

## 📋 Development Process

### Issue Lifecycle
1. **Reported** → Issue created with template
2. **Triaged** → Labeled and prioritized
3. **Assigned** → Developer assigned (optional)
4. **In Progress** → Development started
5. **Review** → Pull request submitted
6. **Testing** → Community testing
7. **Merged** → Included in next release

### Release Process
1. **Development** → Feature/bugfix branches
2. **Integration** → Merge to main branch
3. **Testing** → Comprehensive testing phase
4. **Documentation** → Update docs and changelog
5. **Release** → Tagged release with binaries

## 🔧 Technical Areas

### Core Algorithm Improvements
- Pattern matching engine enhancements
- Circular include detection optimizations
- Performance analysis accuracy
- Memory usage optimizations

### New Feature Areas
- **Enhanced Lite-C Support:** Better action/function detection
- **Template Analysis:** C++ template dependency tracking
- **Macro Tracking:** #define usage analysis
- **Include Guard Detection:** Analyze header protection patterns

### Code Quality
- Add unit tests
- Improve error handling
- Enhance debugging output
- Better memory management

## 📞 Getting Help

### Questions?
- Open a [Discussion](../../discussions) for general questions
- Check [existing issues](../../issues) for known problems
- Review the [Developer Documentation](docs/dev_documentation.md)

### Community Resources
- **EGAMMO Community:** [https://board.egammo.eu/](https://board.egammo.eu/)
- **Lite-C Resources:** Documentation and examples
- **C Programming:** General C development help

### Contact
- **Project Maintainer:** EGAMMO.gaming
- **Website:** [https://egammo.eu/](https://egammo.eu/)
- **Community Forum:** [https://board.egammo.eu/](https://board.egammo.eu/)

## 📜 Code of Conduct

We are committed to providing a welcoming and inclusive experience for everyone. Please:

- **Be respectful** in all communications
- **Be constructive** when providing feedback
- **Be patient** with new contributors
- **Focus on the code and ideas**, not the person
- **Help others learn** and grow

### Enforcement
Violations of this code of conduct should be reported to the project maintainers. We reserve the right to remove content or ban users who violate these guidelines.

---

**Thank you for contributing to the Enhanced C/C++ Dependency Analyzer!**

Together we can make Lite-C development more efficient and maintainable for the entire community.