FROM vimpostor/arch-qt6

# default compiler
ARG cxx=g++
ENV CXX=$cxx

ADD . /build
WORKDIR /build
RUN pacman -Syu --noconfirm imagemagick
RUN scripts/build.sh
