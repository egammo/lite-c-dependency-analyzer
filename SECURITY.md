# Security Policy

## Supported Versions

The Enhanced C/C++ Dependency Analyzer follows a security-focused development approach. We provide security updates for the following versions:

| Version | Supported          | Security Updates |
| ------- | ------------------ | ---------------- |
| 1.3.x   | ✅ **Current**     | ✅ Full Support  |
| 1.2.x   | ✅ **LTS**         | ✅ Critical Only |
| 1.1.x   | ⚠️ **Legacy**      | 🔶 Critical Only |
| 1.0.x   | ❌ **Deprecated**  | ❌ No Support    |

### Version Support Policy
- **Current (1.3.x)**: Full security support with regular updates
- **LTS (1.2.x)**: Long-term support for critical security issues
- **Legacy (1.1.x)**: Limited support for severe vulnerabilities only
- **Deprecated (1.0.x)**: No security updates - upgrade recommended

## Security Considerations

### Static Analysis Tool Security

As a static analysis tool, the Enhanced C/C++ Dependency Analyzer processes source code files. While it does not execute analyzed code, several security considerations apply:

#### 🔍 **Code Analysis Security**
- **File System Access**: The analyzer reads files from specified directories
- **Path Traversal**: Processes include statements and PRAGMA_PATH definitions  
- **Memory Safety**: Handles potentially malformed source files
- **Resource Consumption**: Manages memory and processing time for large codebases

#### ⚠️ **Potential Security Risks**
1. **Malicious Source Files**: Crafted source files could potentially cause:
   - Buffer overflows in pattern matching
   - Infinite loops in circular include detection
   - Excessive memory consumption
   - Path traversal attacks via include statements

2. **Include Resolution**: Processing PRAGMA_PATH and includes could:
   - Access files outside intended directories
   - Follow symbolic links to sensitive areas
   - Process files with excessive nesting depth

3. **Report Generation**: Output files could:
   - Overwrite existing files if permissions allow
   - Expose sensitive path information
   - Create files in unintended locations

## Reporting Security Vulnerabilities

We take security vulnerabilities seriously. Please help us maintain the security of the project by reporting issues responsibly.

### 🚨 **How to Report**

**For security vulnerabilities, please do NOT use public GitHub issues.**

Instead, report security issues through one of these private channels:

#### **Preferred: Private Security Report**
1. Go to the [Security tab](../../security) of this repository
2. Click "Report a vulnerability"  
3. Fill out the security advisory form
4. Submit for private review

#### **Alternative: Direct Contact**
- **Email**: Contact EGAMMO.gaming directly
- **Subject Line**: `[SECURITY] Enhanced C/C++ Dependency Analyzer - <Brief Description>`
- **Community Forum**: Private message to administrators on [EGAMMO Board](https://board.egammo.eu/)

### 📋 **What to Include in Your Report**

Please provide as much information as possible:

#### **Vulnerability Details**
- **Description**: Clear explanation of the vulnerability
- **Impact**: Potential security impact and affected systems
- **Severity**: Your assessment of the risk level
- **Affected Versions**: Which versions are impacted

#### **Reproduction Information**
- **Steps to Reproduce**: Detailed steps to trigger the vulnerability
- **Sample Files**: Minimal test case that demonstrates the issue
- **Environment**: Operating system, compiler version, analyzer version
- **Command Line**: Exact analyzer command that triggers the issue

#### **Technical Details**
- **Root Cause**: If known, explanation of the underlying cause
- **Code Location**: File and line numbers where the issue occurs
- **Suggested Fix**: If you have ideas for resolution

### **Example Report Template**
```
Subject: [SECURITY] Buffer Overflow in Include Path Processing

Description:
A buffer overflow occurs when processing include statements with paths longer than MAX_PATH_LEN.

Impact:
- Potential code execution via crafted source files
- Denial of service through application crash
- Information disclosure via memory corruption

Affected Versions:
- 1.3.0 and earlier
- Confirmed on Windows 10 with Visual Studio 2022

Steps to Reproduce:
1. Create source file with include statement containing 600+ character path
2. Run: analyzer.exe .\test malicious.c output.txt
3. Observe crash in include_resolution() function

Suggested Fix:
Add bounds checking in find_include_file_enhanced() function
```

## Security Response Process

### ⏱️ **Response Timeline**

| Stage | Timeline | Description |
|-------|----------|-------------|
| **Acknowledgment** | 24-48 hours | Confirm receipt of security report |
| **Initial Assessment** | 3-5 days | Evaluate severity and impact |
| **Investigation** | 1-2 weeks | Analyze root cause and develop fix |
| **Fix Development** | 1-3 weeks | Implement and test security patch |
| **Release** | 1-7 days | Deploy fix and security advisory |
| **Public Disclosure** | 30-90 days | Coordinate responsible disclosure |

### 🔄 **Process Steps**

1. **Receipt & Acknowledgment**
   - Security team acknowledges report within 48 hours
   - Initial risk assessment and triage
   - Request additional information if needed

2. **Investigation**  
   - Reproduce the vulnerability
   - Analyze impact and affected versions
   - Develop proof of concept if necessary

3. **Fix Development**
   - Design and implement security fix
   - Comprehensive testing across supported versions
   - Code review by security-focused developers

4. **Quality Assurance**
   - Verify fix resolves the vulnerability
   - Ensure no regression in functionality
   - Test edge cases and error conditions

5. **Release Coordination**
   - Prepare security update release
   - Draft security advisory
   - Coordinate disclosure timeline with reporter

6. **Public Disclosure**
   - Release security update
   - Publish security advisory
   - Credit reporter (if desired)

## Security Best Practices

### 🛡️ **For Users**

#### **Safe Usage Guidelines**
- **Trusted Sources**: Only analyze code from trusted sources
- **Sandboxed Environment**: Run analyzer in isolated environments for untrusted code
- **Output Validation**: Review generated reports before sharing
- **Path Restrictions**: Use relative paths and avoid absolute paths when possible
- **Regular Updates**: Keep analyzer updated to latest secure version

#### **File System Security**
- **Limited Permissions**: Run analyzer with minimal required permissions
- **Read-Only Access**: Make source directories read-only when possible
- **Output Directory**: Use dedicated, isolated output directories
- **Backup Strategy**: Backup important files before analysis

### 🔧 **For Developers**

#### **Secure Development Practices**
- **Input Validation**: Validate all file paths and include statements
- **Buffer Safety**: Use safe string functions and bounds checking
- **Memory Management**: Proper allocation and deallocation
- **Error Handling**: Graceful handling of malformed inputs
- **Resource Limits**: Implement timeouts and resource consumption limits

#### **Code Review Focus Areas**
- **File I/O Operations**: Review all file reading and writing code
- **Path Processing**: Validate path manipulation and resolution logic
- **Pattern Matching**: Check regex and string parsing for edge cases
- **Memory Operations**: Verify buffer bounds and pointer arithmetic

## Vulnerability Disclosure Policy

### 🤝 **Responsible Disclosure**

We follow responsible disclosure principles:

#### **For Security Researchers**
- **Private Reporting**: Report vulnerabilities privately first
- **Reasonable Timeline**: Allow reasonable time for fixes (typically 90 days)
- **Good Faith**: Act in good faith to help improve security
- **No Harmful Actions**: Don't access data, modify systems, or disrupt services

#### **Our Commitments**
- **Acknowledgment**: Prompt acknowledgment of security reports
- **Communication**: Regular updates on investigation and fix progress  
- **Credit**: Public credit for reporters (if desired)
- **No Legal Action**: No legal action against good faith security research

### 📢 **Public Disclosure**

After a security fix is released:

1. **Security Advisory**: Published on GitHub Security tab
2. **Release Notes**: Include security fix information
3. **Community Notification**: Announce on community forums
4. **Documentation Update**: Update security documentation as needed

## Security Architecture

### 🏗️ **Security Design Principles**

#### **Defense in Depth**
- **Input Validation**: Multiple layers of input validation and sanitization
- **Bounds Checking**: Comprehensive buffer and array bounds checking
- **Error Handling**: Graceful degradation and error recovery
- **Resource Limits**: Built-in limits for memory and processing time

#### **Least Privilege**
- **Minimal Permissions**: Request only necessary file system permissions
- **Limited Scope**: Analyze only specified directories and files
- **Safe Defaults**: Default to secure configuration options
- **User Control**: Allow users to restrict analysis scope

### 🔒 **Security Features**

#### **Current Security Measures**
- **Static Memory Allocation**: Predictable memory usage patterns
- **Path Normalization**: Consistent path handling across platforms
- **Include Stack Limits**: Prevention of infinite include loops
- **File Type Restrictions**: Only process `.c` and `.h` files
- **Depth Limits**: Maximum include depth to prevent resource exhaustion

#### **Planned Security Enhancements**
- **Path Validation**: Enhanced path traversal protection
- **Resource Monitoring**: Real-time resource usage monitoring
- **Sandboxing Support**: Integration with sandboxing technologies
- **Audit Logging**: Security-relevant event logging
- **Configuration Validation**: Enhanced configuration file validation

## Contact Information

### 🆘 **Emergency Security Contact**
For critical security issues requiring immediate attention:
- **Severity**: Remote code execution, data corruption, or privilege escalation
- **Contact**: Use GitHub Security Advisory or direct email to maintainers
- **Response**: 24-hour acknowledgment guarantee

### 📞 **General Security Contact**
- **GitHub Security**: [Repository Security Tab](../../security)
- **Project Maintainers**: EGAMMO.gaming
- **Community**: [EGAMMO Board](https://board.egammo.eu/) (for non-critical issues)

### 🌐 **Additional Resources**
- **Project Website**: [https://egammo.eu/](https://egammo.eu/)
- **Community Forum**: [https://board.egammo.eu/](https://board.egammo.eu/)
- **Documentation**: [Developer Documentation](docs/dev_documentation.md)

---

**Thank you for helping keep the Enhanced C/C++ Dependency Analyzer secure!**

Your responsible disclosure of security issues helps protect all users of this tool.