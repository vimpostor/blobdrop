# Blobdrop

[![Continuous Integration](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml/badge.svg)](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml)

Drag and drop your files directly from the terminal.

![Screenshot](https://user-images.githubusercontent.com/21310755/190696396-51a2d827-53fc-4988-88f1-2770a23750f4.png)
# Installation

Arch users can simply install the [blobdrop-git](https://aur.archlinux.org/packages/blobdrop-git) AUR package.

## Building from source

Make sure you have the latest Qt with the Declarative and Svg modules installed.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# install the build/blobdrop binary
cmake --install build
```

# Usage

```bash
blobdrop [files-to-drag]
```

For more options see `blobdrop -h` or the man page `blobdrop(1)`.

# Alternatives

- [dragon](https://github.com/mwh/dragon), A GTK implementation of the same concept
- [ripdrag](https://github.com/nik012003/ripdrag), Like dragon, but rewritten in Rust
