#!/bin/sh

# this request set NDK_ROOT and version recommend r13b
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64

BUILD_DIR=`pwd`/build
if [ ! -r $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi
# change directory to build
cd $BUILD_DIR

# directories
URL_VERSION="7.46.0"
if [[ $CURL_VERSION != "" ]]; then
  URL_VERSION=$CURL_VERSION
fi
SOURCE="curl-$URL_VERSION"
# must be an absolute path
INSTALL_PREFIX=`pwd`/"curl-Android"

SCRATCH="curl_scratch"

CONFIGURE_FLAGS="--enable-cross-compile \
                 --enable-pic \
                 --enable-shared=no \
                 --enable-static=yes \
                 --enable-ipv6 \
                 --enable-threaded-resolver \
                 --disable-manual \
                 --disable-verbose \
		 --disable-crypto-auth \
                 --disable-ldap \
                 --disable-soname-bump \
		 --disable-versioned-symbols \
		 --disable-sspi \
		 --disable-tls-srp \
		 --disable-unix-sockets \
		 --disable-cookies \
		 --disable-ntlm-wb \
                 --disable-ftp \
                 --disable-file \
                 --disable-ldap \
                 --disable-ldaps \
                 --disable-rtsp \
                 --disable-proxy \
                 --disable-dict \
                 --disable-telnet \
                 --disable-tftp \
                 --disable-pop3 \
                 --disable-imap \
                 --disable-smb \
                 --disable-smtp \
                 --disable-gopher \
		 --without-zlib"

#CONFIGURE_FLAGS="--enable-ipv6 \
#		--enable-static \
#		--enable-threaded-resolver \
#		--disable-dict \
#		--disable-gopher \
#		--disable-ldap \
#		--disable-ldaps \
#		--disable-manual \
#		--disable-pop3 \
#		--disable-smtp \
#		--disable-imap \
#		--disable-rtsp \
#		--disable-shared \
#		--disable-smb \
#		--disable-telnet \
#		--disable-verbose"


#ARCHS="arm64 armv7 x86_64 i386"
ARCHS="armeabi-v7a"

COMPILE="y"
LIPO=""

if [ "$*" ]
then
	if [ "$*" = "lipo" ]
	then
		# skip compile
		COMPILE=
	else
		ARCHS="$*"
		if [ $# -eq 1 ]
		then
			# skip lipo
			LIPO=
		fi
	fi
fi

if [ "$COMPILE" ]
then
	CWD=`pwd`
	if [ ! -r $SOURCE ]
	then
		echo 'curl source not found. Trying to download...'
		axel -n 16 -o $CWD/$SOURCE.tar.bz2 -v https://curl.haxx.se/download/$SOURCE.tar.bz2
		tar zxvf $CWD/$SOURCE.tar.bz2
		# curl http://www.ffmpeg.org/releases/$SOURCE.tar.bz2 | tar xj \
		# 	|| exit 1
	fi


	for ARCH in $ARCHS
	do
		echo "building $ARCH..."
		mkdir -p "$SCRATCH/$ARCH"
		cd "$SCRATCH/$ARCH"

		if [ "$ARCH" = "armeabi-v7a" ]
		then
			TARGET_ARCH="arm"
		fi

		export CC="$TOOLCHAIN/bin/arm-linux-androideabi-gcc --sysroot=$SYSROOT"
		export CXX="$TOOLCHAIN/bin/arm-linux-androideabi-g++ --sysroot=$SYSROOT"
		export LINK="$CXX"
		export LD=$TOOLCHAIN/bin/arm-linux-androideabi-ld
		export AR=$TOOLCHAIN/bin/arm-linux-androideabi-ar
		export RANLIB=$TOOLCHAIN/bin/arm-linux-androideabi-ranlib
		export STRIP=$TOOLCHAIN/bin/arm-linux-androideabi-strip

		CXXFLAGS="$CFLAGS"
		LDFLAGS="$CFLAGS"

		TMPDIR=${TMPDIR/%\/} $CWD/$SOURCE/configure \
		    --host=$TARGET_ARCH-linux-androideabi \
		    $CONFIGURE_FLAGS \
		    CFLAGS="$CFLAGS" \
		    LDFLAGS="$LDFLAGS" \
		    --prefix="$INSTALL_PREFIX/$ARCH" \
		|| exit 1

		make -j8 install $EXPORT || exit 1
		cd $CWD
	done
fi

cp -rf $INSTALL_PREFIX `pwd`/../curl

echo Done
