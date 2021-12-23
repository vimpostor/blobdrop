# Blobdrop

Drag and drop your files directly from the terminal.

# Installation

Arch users can simply install the `blobdrop-git` AUR package.

## Building from source

Make sure you have Qt 6.2 with the Declarative and Svg modules installed.

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
