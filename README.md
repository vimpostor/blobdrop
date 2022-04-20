# Blobdrop

Drag and drop your files directly from the terminal.

![Screenshot](https://user-images.githubusercontent.com/21310755/148108749-f472f9ef-d039-48ba-8dd8-4fe820ecb654.png)
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

For more options see `blobdrop -h`.

# Alternatives

[dragon](https://github.com/mwh/dragon) - A GTK implementation of the same concept
