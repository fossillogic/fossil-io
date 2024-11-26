# **Fossil IO Library by Fossil Logic**

Fossil IO is a comprehensive and portable library designed to handle input, output, and error management across various platforms. It provides a robust suite of functions for secure data handling, file stream management, and error reporting. Implemented in C, Fossil IO aims to facilitate high-performance and reliable I/O operations while ensuring consistent behavior across different operating systems. 

## Key Features

1. **Cross-Platform Compatibility**: Provides consistent I/O operations across major operating systems, including Windows, macOS, and Linux.
2. **Secure Input and Output**: Includes functions for secure data handling, preventing common vulnerabilities such as buffer overflows and format string attacks.
3. **Modular Design**: Comprises separate modules for error handling, output, input, file streams, and SOAP functions, allowing for easy integration and customization.
4. **Efficient File Handling**: Supports file stream operations with a focus on secure and reliable file I/O.
5. **Error Reporting**: Includes a comprehensive set of error codes and a lookup function to provide detailed error messages.

## Prerequisites

Before using Fossil IO, ensure you have the following installed:

- **C Compiler**: A C compiler compatible with your platform.
- **Build System**: Meson or CMake for building the library.

## Adding Dependency

1. **Install Build Tools**: Ensure that Meson or CMake is installed on your system. You can install Meson using pip:

   ```sh
   python -m pip install meson           # to install Meson
   python -m pip install --upgrade meson # to upgrade Meson
   ```

2. **Adding Wrap File**: If using Meson, create a `.wrap` file in the `subprojects` directory, such as `fossil-io.wrap`:

   ```ini
   # ======================
   # Git Wrap package definition
   # ======================
   [wrap-git]
   url = https://github.com/fossillogic/fossil-io.git
   revision = v0.1.1

   [provide]
   fossil-io = fossil_io_dep
   ```

3. **Integrate the New Dependency**: Add the following line to your `meson.build` file to integrate the dependency:

   ```ini
   dep = dependency('fossil-io')
   ```

## Configure Options

Fossil IO offers configurable options to tailor the build process to your needs:

- **Running Tests**: To enable testing, configure the build with `-Dwith_test=enabled`.

Example:

```sh
meson setup builddir -Dwith_test=enabled
```

## Contributing and Support

For contributions, issue reporting, or support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are appreciated.
