#!/usr/bin/env bash

set -e

unset LDFLAGS
nix-shell -p glibc -I nixpkgs=https://github.com/NixOS/nixpkgs/archive/136a26be29a9daa04e5f15ee7694e9e92e5a028c.tar.gz --command 'export CC=clang && export CXX=clang++ && cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr && cmake --build build && DESTDIR=AppDir cmake --install build'

wget -q -nc https://github.com/linuxdeploy/linuxdeploy/releases/latest/download/linuxdeploy-x86_64.AppImage https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy-*.AppImage

# create fake desktop file
cat > AppDir/usr/share/applications/blobdrop.desktop <<'EOF'
#!/usr/bin/env xdg-open
[Desktop Entry]
Type=Application
Name=blobdrop
Exec=blobdrop
Categories=Utility;
Icon=blobdrop
EOF

QML_SOURCES_PATHS="$PWD/src/qml" QMAKE=/usr/bin/qmake6 ./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage
