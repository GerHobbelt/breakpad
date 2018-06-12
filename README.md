# Blacknut Breakpad changes
* Compil a basic client library for Windows, Linux (Android) and Mac
* Crosscompilation for Windows
* add CMake generation process
* **Warning** windows code won't build on MSVC
* linux dump_syms can dump symbols from ELF and PECOFF (mingw-w64)
* **Warning** autotools are not updated to build the code
* add lls third party as git submodule

All PECOFF source code come from https://github.com/jon-turney/google-breakpad

I made minor adjustments by handling the MINIDUMP_HEADER to read the streams offset

I fix the code to use an incomplete codeview record in `simple_symbol_supplier.cc` method `SimpleSymbolSupplier::GetSymbolFileAtPathFromRoot`.

The `build-id` compiler option generate an incomplete codeview record. The pdb file part of the record is empty. This lead to `debug_file` member to be empty. 

## Generate symbols
You must build your binaries with mingw-w64 and use the options `-Wl,--build-id` on the linker.

`dump_syms` tool to dump the symbol file

    %> dump_syms crash.exe > crash.sym


## Resolve symbols
`minidump_stackwalk` can understand and resolve the symbols. You cannot use `minidump_stackwalk` from bugsplat or other repo to solve symbols.

Open the crash.sym

    MODULE windows x86 <GUID><AGE> crash.exe
    ...

`<GUID><AGE>` is the identifier for the file crash.exe

To succeed in resolving the symbol, you have to
* create a symbol folder (./symbols)
* create for each binary a folder named as the binary (crash.exe) -> ./symbols/crash.exe
* create a subfolder with the identifier read from the MODULE line of the sym file
* copy the sym file into this subfolder

Commmand line to resolve the symbols
    
    %> minidump_stackwalk crash.dmp ./symbols


# Breakpad

Breakpad is a set of client and server components which implement a
crash-reporting system.

* [Homepage](https://chromium.googlesource.com/breakpad/breakpad/)
* [Documentation](https://chromium.googlesource.com/breakpad/breakpad/+/master/docs/)
* [Bugs](https://bugs.chromium.org/p/google-breakpad/)
* Discussion/Questions: [google-breakpad-discuss@googlegroups.com](https://groups.google.com/d/forum/google-breakpad-discuss)
* Developer/Reviews: [google-breakpad-dev@googlegroups.com](https://groups.google.com/d/forum/google-breakpad-dev)
* Tests: [![Build Status](https://travis-ci.org/google/breakpad.svg?branch=master)](https://travis-ci.org/google/breakpad) [![Build status](https://ci.appveyor.com/api/projects/status/eguv4emv2rhq68u2?svg=true)](https://ci.appveyor.com/project/vapier/breakpad)
* Coverage [![Coverity Status](https://scan.coverity.com/projects/9215/badge.svg)](https://scan.coverity.com/projects/google-breakpad)

## Getting started (from master)

1.  First, [download depot_tools](http://dev.chromium.org/developers/how-tos/install-depot-tools)
    and ensure that they’re in your `PATH`.

2.  Create a new directory for checking out the source code (it must be named
    breakpad).

    ```sh
    mkdir breakpad && cd breakpad
    ```

3.  Run the `fetch` tool from depot_tools to download all the source repos.

    ```sh
    fetch breakpad
    cd src
    ```

4.  Build the source.

    ```sh
    ./configure && make
    ```

    You can also cd to another directory and run configure from there to build
    outside the source tree.

    This will build the processor tools (`src/processor/minidump_stackwalk`,
    `src/processor/minidump_dump`, etc), and when building on Linux it will
    also build the client libraries and some tools
    (`src/tools/linux/dump_syms/dump_syms`,
    `src/tools/linux/md2core/minidump-2-core`, etc).

5.  Optionally, run tests.

    ```sh
    make check
    ```

6.  Optionally, install the built libraries

    ```sh
    make install
    ```

If you need to reconfigure your build be sure to run `make distclean` first.

To update an existing checkout to a newer revision, you can
`git pull` as usual, but then you should run `gclient sync` to ensure that the
dependent repos are up-to-date.

## To request change review

1.  Follow the steps above to get the source and build it.

2.  Make changes. Build and test your changes.
    For core code like processor use methods above.
    For linux/mac/windows, there are test targets in each project file.

3.  Commit your changes to your local repo and upload them to the server.
    http://dev.chromium.org/developers/contributing-code
    e.g. `git commit ... && git cl upload ...`
    You will be prompted for credential and a description.

4.  At https://chromium-review.googlesource.com/ you'll find your issue listed;
    click on it, then “Add reviewer”, and enter in the code reviewer. Depending
    on your settings, you may not see an email, but the reviewer has been
    notified with google-breakpad-dev@googlegroups.com always CC’d.
