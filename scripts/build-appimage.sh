#!/usr/bin/env bash

set -e

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
DESTDIR=AppDir cmake --install build

wget -q -nc https://github.com/linuxdeploy/linuxdeploy/releases/latest/download/linuxdeploy-x86_64.AppImage https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy-*.AppImage

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

QML_SOURCES_PATHS="$PWD/src/qml" QMAKE=/usr/bin/qmake6 ./linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage
