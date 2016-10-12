#!/bin/bash

CURDIR=`pwd`

# This expects that this is place as a first level folder relative to the other
# OP-TEE folder in a setup using default repo configuration as described by the
# documentation in optee_os (README.md)
ROOT=${PWD}
ROOT=`dirname $ROOT`

# Path to the toolchain
export PATH=${ROOT}/toolchains/aarch32/bin:$PATH

# Path to the TA-DEV-KIT coming from optee_os
#ELF32# export TA_DEV_KIT_DIR=${ROOT}/optee_os/out/arm/export-ta_arm32
export TA_DEV_KIT_DIR=/home/optee/RPi3/build/../optee_os/out/arm/export-ta_arm64

# Path to the client library (GP Client API)
export TEEC_EXPORT=${ROOT}/optee_client/out/export

export PLATFORM=rpi3
#export PLATFORM_FLAVOR=qemu_virt

# Toolchain prefix for user space code (normal world)
HOST_CROSS_COMPILE=arm-linux-gnueabihf-
# Build the host application
cd $CURDIR/host
make CROSS_COMPILE=$HOST_CROSS_COMPILE $@

# Toolchain prefix for the Trusted Applications
#ELF32# TA_CROSS_COMPILE=arm-linux-gnueabihf-
CROSS_COMPILE_TA="/home/optee/RPi3/build/../toolchains/aarch64/bin/aarch64-linux-gnu-"
# Build the Trusted Application
cd $CURDIR/ta
make CROSS_COMPILE=$CROSS_COMPILE_TA $@
