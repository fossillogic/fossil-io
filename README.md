# **Fossil IO Library by Fossil Logic**

Fossil IO is an extensive and versatile library meticulously crafted to manage input, output, and error handling across a multitude of platforms. This library offers a comprehensive array of functions dedicated to secure data handling, efficient file stream management, and detailed error reporting. Developed in the C programming language, Fossil IO is engineered to support high-performance and reliable I/O operations, ensuring uniform and predictable behavior across various operating systems, including Windows, macOS, and Linux. By leveraging Fossil IO, developers can achieve consistent and secure I/O operations, making it an indispensable tool for cross-platform software development.

## Key Features

1. **Cross-Platform Compatibility**: Provides consistent I/O operations across major operating systems, including Windows, macOS, and Linux.
2. **Secure Input and Output**: Includes functions for secure data handling, preventing common vulnerabilities such as buffer overflows and format string attacks.
3. **Modular Design**: Comprises separate modules for error handling, output, input, file streams, and SOAP functions, allowing for easy integration and customization.
4. **Efficient File Handling**: Supports file stream operations with a focus on secure and reliable file I/O.
5. **Error Reporting**: Includes a comprehensive set of error codes and a lookup function to provide detailed error messages.

## ***Prerequisites***

To get started, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson `1.8.0` or newer installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).
- **Conan Package Manager**: If you prefer using Conan, ensure it is installed by following the instructions on the official [Conan website](https://docs.conan.io/en/latest/installation.html).

### Adding Dependency

#### Adding via Meson Git Wrap

To add a git-wrap, place a `.wrap` file in `subprojects` with the Git repo URL and revision, then use `dependency('fossil-io')` in `meson.build` so Meson can fetch and build it automatically.

#### Integrate the Dependency:

Add the `fossil-io.wrap` file in your `subprojects` directory and include the following content:

```ini
[wrap-git]
url = https://github.com/fossillogic/fossil-io.git
revision = v0.2.6

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
