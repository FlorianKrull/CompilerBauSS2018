# Guidelines

Deviate from these guidelines only if you have valid reasons to do so.

## General

- *implementing* also implies that it is *properly* (unit) tested, otherwise it is not considered implemented!
- don't be a git -- use [Git](https://git-scm.com/)!
- files are UTF-8 encoded and use Unix line-endings (`\n`)
- files contain *one* newline at the end
- lines do not contain trailing white-space
- enable (`-Wall -Wextra`) and minimise compiler warnings
- do not waste time or space (memory leaks)
- check for leaks using `valgrind`, especially in error cases
- keep design and development principles in mind, especially KISS and DRY
- always state the sources of non-original content
    - use persistent links when possible
    - this also applies to work taken from other teams
    - ideas / inspirations should be referenced too

> Credit where credit is due.

## Plain-Text Files

- lines must not exceed 80 columns
- content is properly formatted
    - well structured
    - paragraphs are clearly separated
    - headings stand out
- files may contain *ASCII art* (not restricted to ASCII, UTF-8 characters are okay)
- files should be *self-contained* (ie no references to other files like images or so)

See issues of the [Phrack magazine](http://www.phrack.org/) as reference.

## Markdown Files

- use newlines to separate sentences (works well when using Git)
- try to be compatible with [Pandoc](https://pandoc.org/)'s interpretation of Markdown

## C/C++ Files

- use a formatting tool, like [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html)
- stick to the formatting used in the provided code-base (similar to [Linux Kernel Coding Style])
- lines should not exceed 80 columns
- use the following order for your includes:
    - corresponding header (`ast.c` -> `ast.h`)
    - system headers
    - other library headers
    - other headers of the same project
- the structure of a source file should be similar to its corresponding header file
    - separators can be helpful, but they should not distract the reader

Also, keep the following in mind, taken from [Linux Kernel Coding Style]:

> Functions should be short and sweet, and do just one thing.
> They should fit on one or two screenfuls of text (the ISO/ANSI screen size is 80x24, as we all know), and do one thing and do that well.
>
> The maximum length of a function is inversely proportional to the complexity and indentation level of that function.
> So, if you have a conceptually simple function that is just one long (but simple) case-statement, where you have to do lots of small things for a lot of different cases, it's OK to have a longer function.
>
> However, if you have a complex function, and you suspect that a less-than-gifted first-year high-school student might not even understand what the function is all about, you should adhere to the maximum limits all the more closely.
> Use helper functions with descriptive names (you can ask the compiler to in-line them if you think it's performance-critical, and it will probably do a better job of it than you would have done).
>
> Another measure of the function is the number of local variables.
> They shouldn't exceed 5-10, or you’re doing something wrong.
> Re-think the function, and split it into smaller pieces.
> A human brain can generally easily keep track of about 7 different things, anything more and it gets confused.
> You know you’re brilliant, but maybe you'd like to understand what you did 2 weeks from now.

[Linux Kernel Coding Style]: https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## System

I'll be using a virtualised, updated Debian Testing (Buster) (64 bit) to examine your submissions.
The submitted code has to compile and run on this system.
This information tells you which software versions I'll be using.

## Build System

You may use either [Meson](http://mesonbuild.com/), [CMake](https://cmake.org/), Autotools, or plain Makefiles as build system (generator).
Yet, the dependencies between your source files must be modelled correctly.
Talk to me if you *really* want to use a different build system.

The default configuration should be a *release build*.
Switching to a *debug build* configuration should be possible via a commandline switch.

## Unit Testing

I highly recommend using [GoogleTest](https://github.com/google/googletest).
But you can choose a different unit testing framework if you want.
Just make sure it is present as package in my examination system's default package repository, or can be easily installed via your build system.

All of your unit tests should be run issuing only a single command (given the tests have been built).
I am expecting something like:

    $ meson test

## Integration Testing

My recommendation is to build yourself an integration test runner.
Otherwise, the same conditions as for unit testing frameworks apply.

## README

Always provide a README listing pre-requisites and containing instructions for building, and running tests.
