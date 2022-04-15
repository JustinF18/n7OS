FROM alpine:latest
RUN apk update
RUN apk upgrade
RUN apk add curl gcc libc-dev g++ gmp-dev mpc1-dev mpfr-dev make
# export PREFIX="/usr/local/i386elfgcc"
# export TARGET=i386-elf
# export PATH="$PREFIX/bin:$PATH"

# Binutils
RUN mkdir /tmp/src
WORKDIR /tmp/src
RUN curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.gz
RUN tar xf binutils-2.24.tar.gz
RUN mkdir binutils-build
WORKDIR /tmp/src/binutils-build
RUN ../binutils-2.24/configure --target=i386-elf --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=/usr/local 2>&1 | tee configure.log
RUN make all install 2>&1

# GCC
WORKDIR /tmp/src
RUN curl -O https://ftp.gnu.org/gnu/gcc/gcc-11.1.0/gcc-11.1.0.tar.gz
RUN tar xf gcc-11.1.0.tar.gz
RUN mkdir gcc-build
WORKDIR /tmp/src/gcc-build
RUN ../gcc-11.1.0/configure --target=i386-elf --prefix=/usr/local --disable-nls --disable-libssp --enable-languages=c --without-headers
RUN make all-gcc
RUN make all-target-libgcc
RUN make install-gcc
RUN make install-target-libgcc
