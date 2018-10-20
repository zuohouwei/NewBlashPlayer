#!/bin/sh

BUILD_DIR=`pwd`/build
if [ ! -r $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi
# change directory to build
cd $BUILD_DIR

# directories
AL_VERSION="1.19.1"
if [[ $OPENAL_VERSION != "" ]]; then
  AL_VERSION=$OPENAL_VERSION
fi
SOURCE="openal-soft-$AL_VERSION"

SCRATCH="openal-soft_scratch"
# must be an absolute path
INSTALL_PREFIX=`pwd`/"openal-soft_Android"

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
		# http://www.openal-soft.org/openal-releases/openal-soft-1.19.1.tar.gz
		echo 'libev source not found. Trying to download...'
		axel -n 16 -o $CWD/$SOURCE.tar.bz2 -v http://www.openal-soft.org/openal-releases/$SOURCE.tar.bz2
		tar zxvf $CWD/$SOURCE.tar.bz2
	fi

	for ARCH in $ARCHS
	do
		echo "building $ARCH..."
		
		rm -rf "SCRATCH/$ARCH"

		mkdir -p "$SCRATCH/$ARCH"
		cd "$SCRATCH/$ARCH"

		cmake \
			-DANDROID_ABI="$ARCHS" \
			-DANDROID_NDK="$NDK_ROOT" \
			-DCMAKE_TOOLCHAIN_FILE=../../../android.toolchain.cmake \
			-DCMAKE_BUILD_TYPE="Release" \
			-DCMAKE_C_FLAGS="-s" \
			-DALSOFT_DLOPEN=OFF \
			-DALSOFT_BACKEND_WAVE=OFF \
			-DALSOFT_TESTS=OFF \
			-DALSOFT_EXAMPLES=OFF \
			-DALSOFT_UTILS=OFF \
			-DALSOFT_AMBDEC_PRESETS=OFF \
			-DALSOFT_HRTF_DEFS=OFF \
			-DALSOFT_CONFIG=OFF \
			-DLIBTYPE="STATIC" \
			-DCMAKE_INSTALL_PREFIX=`pwd`/../../../openal-soft/$ARCH \
			../../$SOURCE

			# -DLIBTYPE="STATIC" \

		make -j8 install $EXPORT || exit 1
		cd $CWD
	done
fi

echo Done
