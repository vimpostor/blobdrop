# Blobdrop

[![Continuous Integration](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml/badge.svg)](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml)

Drag and drop your files directly from the terminal.

![Screenshot](https://user-images.githubusercontent.com/21310755/190696396-51a2d827-53fc-4988-88f1-2770a23750f4.png)
# Installation

Note: For Arch Linux users there is an [AUR package](https://aur.archlinux.org/packages/blobdrop-git), for Nix users there is a flake in this repo.

## Building from source

Make sure you have the latest Qt with the Declarative and Svg modules installed.

```bash
cmake -B build
cmake --build build

# install the build/blobdrop binary
cmake --install build
```

# Usage

```bash
blobdrop [files-to-drag]
```

For more options see `blobdrop -h` or the man page `blobdrop(1)`.

## Features

- Drag files to other applications
- Pass filenames as trailing optargs
- Pipe filenames asynchronously into stdin
- Automatically quit once all paths have been dragged
- Auto-hide while dragging
- Show mime icons and thumbnails for media
- Drag all files at once
- Act as a sink and print dropped files to the terminal
- Optionally keep the window on top
- Quickly open files with a single click
- Respect system dark mode
- Efficient implementation, can scroll through a list of several thousand files without dropping frames

# Alternatives

- [dragon](https://github.com/mwh/dragon), A GTK implementation of the same concept
- [ripdrag](https://github.com/nik012003/ripdrag), Like dragon with GTK, but rewritten in Rust
