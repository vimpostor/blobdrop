# Build with: podman build -t blobdrop .
# Run with: podman run -v "$PWD:/output" blobdrop
FROM docker.io/vimpostor/appimage-qt6

RUN apt-get -y install libxcb-ewmh-dev

ADD . /build
WORKDIR /build
RUN scripts/build-appimage.sh
CMD cp *-x86_64.AppImage /output
