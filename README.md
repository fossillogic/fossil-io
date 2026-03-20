# **Fossil IO Library by Fossil Logic**

Fossil IO is a robust C library designed for seamless input, output, and error management across major operating systems. It delivers a unified API for secure data handling, efficient file operations, and comprehensive error diagnostics. With a focus on reliability and performance, Fossil IO empowers developers to build cross-platform applications with predictable I/O behavior.

- **Supported Platforms:** Windows, macOS, Linux
- **Language:** C
- **Purpose:** Consistent, secure, and high-performance I/O operations


## Key Features

1. **Cross-Platform Compatibility**: Consistent I/O operations across Windows, macOS, and Linux.
2. **Secure Input and Output**: Functions designed to prevent buffer overflows and format string vulnerabilities.
3. **Modular Architecture**: Separate modules for input, output, error handling, file streams, and SOAP, enabling flexible integration.
4. **Efficient File Handling**: Robust file stream support for secure and reliable file I/O.
5. **Comprehensive Error Reporting**: Extensive error codes and lookup functions for detailed diagnostics.
6. **High Performance**: Optimized for low-latency and high-throughput I/O operations.
7. **Thread Safety**: Designed to support safe concurrent operations in multithreaded environments.
8. **Customizable Logging**: Built-in logging facilities with configurable verbosity and output targets.
9. **Extensive Documentation**: Well-documented APIs and usage examples for easy onboarding.
10. **Test-Driven Examples**: Unit tests double as usage samples, demonstrating real-world scenarios.

## ***Prerequisites***

To get started, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson `1.8.0` or newer installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Dependency

#### Adding via Meson Git Wrap

To add a git-wrap, place a `.wrap` file in `subprojects` with the Git repo URL and revision, then use `dependency('fossil-io')` in `meson.build` so Meson can fetch and build it automatically.

#### Integrate the Dependency:

Add the `fossil-io.wrap` file in your `subprojects` directory and include the following content:

```ini
[wrap-git]
url = https://github.com/fossillogic/fossil-io.git
revision = v0.2.12

[provide]
dependency_names = fossil-io
```

**Note**: For the best experience, always use the latest releases. Visit the [releases](https://github.com/fossillogic/fossil-io/releases) page for the latest versions.

## Configure Options

Fossil IO offers configurable options to tailor the build process to your needs:

- **Running Tests**: To enable testing, configure the build with `-Dwith_test=enabled`.

Example:

```sh
meson setup builddir -Dwith_test=enabled
```

### Tests Double as Samples

The project is designed so that **test cases serve two purposes**:

- ✅ **Unit Tests** – validate the framework’s correctness.  
- 📖 **Usage Samples** – demonstrate how to use these libraries through test cases.  

This approach keeps the codebase compact and avoids redundant “hello world” style examples.  
Instead, the same code that proves correctness also teaches usage.  

This mirrors the **Meson build system** itself, which tests its own functionality by using Meson to test Meson.  
In the same way, Fossil Logic validates itself by demonstrating real-world usage in its own tests via Fossil Test.  

```bash
meson test -C builddir -v
```

Running the test suite gives you both verification and practical examples you can learn from.

## Contributing and Support

For contributions, issue reporting, or support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are appreciated.
