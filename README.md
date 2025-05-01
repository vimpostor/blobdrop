# Blobdrop

[![Continuous Integration](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml/badge.svg)](https://github.com/vimpostor/blobdrop/actions/workflows/ci.yml)

Drag and drop your files directly from the terminal.

https://github.com/vimpostor/blobdrop/assets/21310755/1957b6a7-475c-4930-80b9-18564ef39eb9


# Installation

Note: For Arch Linux users there is an [AUR package](https://aur.archlinux.org/packages/blobdrop-git), for Nix users there is a [flake](flake.nix) available and an official package in [nixpkgs](https://github.com/NixOS/nixpkgs/blob/master/pkgs/by-name/bl/blobdrop/package.nix).
You can also download an AppImage from the [latest release](https://github.com/vimpostor/blobdrop/releases/latest).

## Building from source

Make sure you have a C++23 compiler and the latest Qt with the Declarative and Svg modules installed.

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

- Start drag automatically without a GUI
- Hide the parent terminal emulator while dragging
- Automatically quit once all paths have been dragged
- Auto-hide the GUI while dragging
- Show mime icons and thumbnails for media
- Drag all files at once
- Preview files with a single click
- Shell completions
- Act as a sink and print dropped files to the terminal
- Pipe filenames asynchronously into stdin

### Frontends

Blobdrop implements multiple frontends to drag the files from:

- [From a normal window](https://github.com/vimpostor/blobdrop/assets/21310755/d86f5039-05cd-4444-9e43-cc51cf4073db)
- [Inside a desktop notification](https://github.com/vimpostor/blobdrop/assets/21310755/482b4bc1-2f15-43e3-b980-1f573c494a91), using the `x-kde-urls` Notifications extension
- [As an immediate drop to a click location](https://user-images.githubusercontent.com/21310755/266832800-519773b6-d154-4fd7-9faf-dfb25217055c.mp4), this works on all EWMH-compliant window managers
- As clipboard content, allowing to paste the file URIs to other programs
- As an [OSC8 hyper link](https://github.com/vimpostor/blobdrop/commit/3ba601c690571460fc8cd130abb57c7a15c67cf1) in the terminal emulator, but currently there exists no terminal emulator that can drag and drop OSC8 links

# Alternatives

- [dragon](https://github.com/mwh/dragon) - A GTK implementation of the same concept
- [clidrag](https://github.com/rkevin-arch/CLIdrag) - A CLI-only implementation
- [ripdrag](https://github.com/nik012003/ripdrag) - Like dragon with GTK, but rewritten in Rust

Note that none of these alternatives provide a workflow similar to blobdrop's immediate frontend (except clidrag, but it lacks many UX improvements).
