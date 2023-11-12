#!/usr/bin/env bash

set -e

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
DESTDIR=AppDir cmake --install build

# create fake desktop file
mkdir -p AppDir/usr/share/applications
cat > AppDir/usr/share/applications/blobdrop.desktop <<'EOF'
#!/usr/bin/env xdg-open
[Desktop Entry]
Type=Application
Name=blobdrop
Exec=blobdrop
Categories=Utility;
Icon=blobdrop
EOF

# install placeholder icon
mkdir -p AppDir/usr/share/icons/hicolor/scalable/apps
cp assets/blobdrop.svg AppDir/usr/share/icons/hicolor/scalable/apps/

QML_SOURCES_PATHS="$PWD/src/qml" linuxdeploy --appdir AppDir --plugin qt --output appimage
