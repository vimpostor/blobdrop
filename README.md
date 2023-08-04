# Blobdrop

[![Continuous Integration](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml/badge.svg)](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml)

Drag and drop your files directly from the terminal.

![Screenshot](https://user-images.githubusercontent.com/21310755/252060266-04120929-fa7b-4534-9b89-dd3fb7a2e605.png)
# Installation

Note: For Arch Linux users there is an [AUR package](https://aur.archlinux.org/packages/blobdrop-git), for Nix users there is a [flake](flake.nix) available.

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
- Shell completions

### Frontends

Blobdrop implements multiple frontends to drag the files from:

- [From a normal window](https://github.com/vimpostor/blobdrop/assets/21310755/d86f5039-05cd-4444-9e43-cc51cf4073db) (default)
- [Inside a desktop notification](https://github.com/vimpostor/blobdrop/assets/21310755/482b4bc1-2f15-43e3-b980-1f573c494a91), using the `x-kde-urls` Notifications extension
- As an [OSC8 hyper link](https://github.com/vimpostor/blobdrop/commit/3ba601c690571460fc8cd130abb57c7a15c67cf1) in the terminal emulator, but currently there exists no terminal emulator that can drag and drop OSC8 links

# Alternatives

- [dragon](https://github.com/mwh/dragon), A GTK implementation of the same concept
- [ripdrag](https://github.com/nik012003/ripdrag), Like dragon with GTK, but rewritten in Rust
