FROM gentoo/stage3:latest

LABEL platform agnostic container

RUN emerge --sync

RUN echo 'CFLAGS="-march=native -mtune=native -O3 -pipe"' > /etc/make.conf
RUN echo 'CXXFLAGS="${CFLAGS}"' >> /etc/make.conf

RUN echo "dev-libs/boost **" > /etc/portage/package.accept_keywords/boost
RUN echo "dev-util/boost-build **" >> /etc/portage/package.accept_keywords/boost

RUN echo "dev-libs/boost test" > /etc/portage/package.use/boost
RUN echo "dev-libs/icu static-libs" > /etc/portage/package.use/icu

RUN emerge \
    bash-completion \
    clang \
    eix \
    htop \
    app-misc/mc \
    kde-frameworks/extra-cmake-modules \
    sys-process/htop \
    dev-vcs/git \
    -j`nproc`

RUN eix-update

RUN emerge -j`nproc` leveldb

RUN emerge \
    dev-libs/icu \
    sys-process/htop \
    sys-libs/zlib \
    x11-libs/libX11 \
    x11-libs/libxcb \
    x11-libs/libXScrnSaver \
    -j`nproc`

RUN echo "dev-libs/icu examples static-libs" > /etc/portage/package.use/icu
RUN emerge \
    dev-libs/icu \
    -j`nproc`
    
RUN emerge \
    gdb \
    -j`nproc`

RUN emerge -j`nproc` =dev-libs/boost-1.79.0
RUN sed -i 's/image_read_info< targa_tag >()/image_read_info()/g' /usr/include/boost/gil/extension/io/targa/tags.hpp

RUN echo 'ACCEPT_LICENSE="*"' >> /etc/make.conf
RUN emerge -j`nproc` dev-util/intel-ocl-sdk

