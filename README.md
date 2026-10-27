## Description
This repository features a very simple logger. You can easily integrate it with your own project(s) either by using [Git submodules](https://git-scm.com/docs/gitsubmodules). It only supports [POSIX](https://en.wikipedia.org/wiki/POSIX) conformant
platforms. Support for non-POSIX conformant platforms is tentative.

The logger produces a log line in the following format<sup>[1]</sup>:
```txt
[YYYY-MM-DD HH:MM:SS][FILE_NAME:LINE_NUMBER][SEVERITY]: Your message goes here.
```

> 1. **Note** A new line is not included. So, you must include an explicit `'\n'` in your
message.

If you dislike this format, you can fork this repository and make changes on your own. The source is extremely small very easy to navigate.

The logger only logs the messages in debug mode (i.e. when `NDEBUG` is **not** defined).
You can change this by removing the `#ifndef NDEBUG` pre-processor conditionals in
the header and the source files.

## Obtaining sources
It is completely self-contained, except some POSIX-specific source and the standard C library, of course.
Clone it with
```sh
git clone https://codeberg.org/nilabhro/log.git
```
If you prefer GitHub:
```sh
git clone https://github.com/goldbarley/log.git
```

## Building
It uses [CMake](https://cmake.org/) as its build system. So, for a simple build, just run:
```sh
cmake -S . -B build
```
On the top level, it compiles to an executable including the `tests/` directory. On any other level, it
compiles to a static library. To configure these options yourself, refer to the **Build options**
section.

### Prerequisites
- POSIX.1-2008 conformant.
- ISO C11 or higher.
- [CMake version 3.15.7](https://cmake.org/files/v3.15/) or [higher](https://cmake.org/download/).

### Build options<sup>[2]</sup>
|Option|Type|Description|
|:--:|:--:|:--|
|[`CMAKE_BUILD_TYPE`](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)|String|Defines the type of build. If a build type is not set then it will default to `Release`.
|`LOG_LIB_TYPE`|String|This defines the type of library that the project compiles to. It can either be a shared library, a static library, or an executable. The valid values for this option are: `STATIC`, `SHARED`, or `EXEC`. If a value for this option is not set, it will default to either `STATIC` if the project is **not** at the top level or `EXEC` if the project **is** at the top level.|

> 2. **Note** The CMake build automatically creates a `compile_commands.json` file which can be used with
your [Language Server Protocol](https://en.wikipedia.org/wiki/Language_Server_Protocol) (LSP).
Unfortunately, there is no option to toggle this, however, you can either remove it manually after
the build or edit the `CMakeLists.txt` file directly to not create one at all.

To compile the project, run:
```sh
cmake --build build
```

## Specification

### File descriptors
The logger uses file descriptors instead of file pointers (`FILE *`). While you can use your own,
the logger defines a few file descriptors for convenience:
- `LOG_FD_FILE`: A file is created with the name `log[unix-epoch].txt`. Here `[unix-epoch]` is the 
number of seconds passed since 00:00:00 UTC on January 1, 1970 (See [Unix time](https://en.wikipedia.org/wiki/Unix_time)). So, the file name stays unique,
unless you run two instances of the logger in the same second.
- `LOG_FD_DEFAULT`: This sets a default stream for each of the log severity levels.
- `LOG_FD_UNDEFINED`: Same as `LOG_FD_DEFAULT`.
- `LOG_FD_STDIN`: The [standard input stream](https://en.wikipedia.org/wiki/Standard_streams#Standard_input_(stdin)).
- `LOG_FD_STDOUT`: The [standard output stream](https://en.wikipedia.org/wiki/Standard_streams#Standard_output_(stdout)).
- `LOG_FD_STDERR`: The [standard error stream](https://en.wikipedia.org/wiki/Standard_streams#Standard_error_(stderr)).

### Severities
The logger features 5 types of severities under the enumeration `enum log_sev`. The following lists all the severities from the lowest to the highest order:
- **Information**: Shortened to `INF`, is the lowest level of severity. This is what you may want to use
this to write any general information. If the file descriptor is set to `LOG_FD_UNDEFINED` or
`LOG_FD_DEFAULT`, then information logs are written into the standard output stream (stdout).

- **Debug**: Shortened to `DBG`. You may want to use this to output debug-specific messages. If the
file descriptor is set to `LOG_FD_UNDEFINED` or `LOG_FD_DEFAULT`. Then the debug logs are written into
the standard output stream(stdout).
- **Warning**: Shortened to `WRN`. This severity level is used for warnings, something that technically
is unintended behaviour, but not catastrophic enough to break your code and *may* be ignored (but it
is not often a good idea). If the file descriptor is set to `LOG_FD_UNDEFINED` or `LOG_FD_DEFAULT`, 
then the warning logs are written into the standard error stream (stderr).
- **Error**: Shortened to `ERR`. This is used to indicate an error in your code or a part of your code.
Some errors may cause your code to crash, but some errors may be "non-fatal" and your application
may continue to run but with a few issues due to the failed part. If the file descriptor is set to
`LOG_FD_UNDEFINED` or `LOG_FD_DEFAULT`, then the error logs are written into the standard error stream
(stderr).
- **Fatal**: Shortened to `FTL`, this is the highest level of severity. It is mainly used when
you encounter an error or a bug, past which your application cannot run at all. If the file descriptor
is set to `LOG_FD_UNDEFINED` or `LOG_FD_DEFAULT`, then the error logs are written into the standard
error stream (stderr).

### Identifiers
Everything inside the header begins with the prefix `log` (for regular identifiers) and `LOG` (for
pre-processor macros). The following are some of the identifier formats<sup>[3][4]</sup> used in the header:
> 3. **Note** [IDENTIFIER] refers to the identifier without the `log` or `LOG` prefix

> 4. **Note** the following just uses capital letters, but the same goes for non-capital letters as well.

- `[IDENTIFIER]V`: The suffix `V` implies that `LOG_[IDENTIFIER]V` directly uses variadic arguments.
- `[IDENTIFIER]F`: The suffix `F` implies that `LOG_[IDENTIFIER]F` uses variadic arguments which
is translated into a variable argument list.
- `D[IDENTIFIER]`: The prefix `D` implies that `LOG_D[IDENTIFIER]` takes the
file descriptor as its argument.
- `D[IDENTIFIER]F`: The prefix `D` and the suffix `F` implies that `LOG_D[IDENTIFIER]F` takes
the file descriptor as one of its arguments and also uses variadic arguments which is translated into
a variable argument list.

## Usage
The following shows a very simple usage of the logger:
```c
#include "log/log.h"

int main(void)
{
	/*
	 * Create a file using LOG_FD_FILE.
	 *
	 * Alternatively, provide your own file descriptor.
	 */
	log_open(LOG_FD_FILE);

	LOG_DINFF(LOG_FD_STDOUT, "Yin Yang.\n");
	LOG_WRN("Little bit of bitch inside the nice.\n");
	LOG_ERRF("%s", "A little bit of nice inside the bitch.\n");

	/*
	 * Will only close if LOG_FD_FILE. Otherwise,
	 * you are responsible for any file descriptor
	 * that you explicitly opened yourself.
	 */
	log_close();

	return 0;
}
```

## Repositories
- [**Codeberg**](https://codeberg.org/nilabhro/log)
- [**GitHub**](https://github.com/goldbarley/log)

### *This project is licensed under the terms of the [MIT license](https://en.wikipedia.org/wiki/MIT_License).*
