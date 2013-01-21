#!/bin/bash
#
# Build the Cortex programming tools
#
# Author: Philipp.Sommer@csiro.au

ROOT_DIR=`pwd`
TMP=${ROOT_DIR}/tmp
BUILD_DIR=${ROOT_DIR}/build
MAKE_OPTS="-j 5"

# assert function for a specific file
function assert_file {
	if [ -f "$1" ]; then
		echo "Verified file $1";
	else
		echo "File $1 does not exist after build, error!";
		exit -1;
	fi
}

# setup build environment
function set_up {
	echo ''
	echo '############################'
	echo 'Initializing build environment'
	echo '############################'
	echo ''
	# put directories in place
	mkdir -p ${TMP}
	mkdir -p ${BUILD_DIR}
}

# build bossa
function build_bossa {

	# extract bossa source code
	echo ''
	echo '#############################################'
	echo 'Extracting bossa source'
	echo '#############################################'
	echo ''

	cd ${TMP}
        tar xvf ${ROOT_DIR}/src/bossa/bossa*.tar.gz

	# apply patches
	echo ''
	echo '#############################################'
	echo 'Applying patches to bossa'
	echo '#############################################'
	echo ''

	patch -p0 < ${ROOT_DIR}/src/bossa/*.patch

	# build bossa
	echo ''
	echo '#############################'
	echo 'Building bossa'
	echo '#############################'
	echo ''


	make 

        # copy binaries
        cp bin/* ${BUILD_DIR}
	
	# clean up
	cd ${ROOT_DIR}
	rm -rf ${TMP}
	
	# a few assertions that we have what we want
	assert_file ${BUILD_DIR}/bossac

}


# build opal tools
function build_opal {

	# build bossa
	echo ''
	echo '#############################'
	echo 'Building Opal debug tools'
	echo '#############################'
	echo ''

        cd src/opal

	make 

        # move binaries
        cp bin/* ${BUILD_DIR}/
        rm -rf bin

	# a few assertions that we have what we want
	assert_file ${BUILD_DIR}/opal-reset
	assert_file ${BUILD_DIR}/opal-erase-and-reset
	assert_file ${BUILD_DIR}/opal-init
	assert_file ${BUILD_DIR}/opal-start
	assert_file ${BUILD_DIR}/opal-stop

}

# remove build items
function clean_all {
	echo ''
	echo '#####################'
	echo 'Clean all build files'
	echo '#####################'
	echo ''
	rm -rf ${BUILD_DIR}
}

# build using the functions
clean_all
set_up
build_bossa
build_opal

exit 0
