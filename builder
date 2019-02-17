#!/bin/bash
#author: Max Fruehmann
OK="[$(tput setaf 2)OK$(tput setaf 7)]"
INFO="[$(tput setaf 6)INFO$(tput setaf 7)]"
WARN="[$(tput setaf 3)WARNING$(tput setaf 7)]"
ERR="[$(tput setaf 1)ERROR$(tput setaf 7)]"
build_kernel(){
	mkdir kernel
	cd kernel
	echo "$INFO Downloading Source-Files..."
	wget -q -O kernel_tar.tar.xz $1
	wget -q -O kernel.tar.sign $2
	keys=$(gpg --list-keys)
	if [[ $keys != *"linus torvalds"* && $keys != *"Greg Kroah-Hartman"* ]] 
	then
		echo "$WARN No Public Keys... Downloading..." 
		gpg --keyserver http://keys.gnupg.net --recv-keys 678D1B42 6092693E
		echo "$OK Downloaded Public Keys"
	fi
	sign=$(unxz kernel_tar.tar.xz | gpg --verify kernel.tar.sign)
	if [ $sign == *"Can't check signatures"* ] then
		echo "$ERR Verifying Kernel Package failed. Aborting!"
		exit 2
	fi
	echo "$INFO Unpacking Source-Files"
	tar xaf kernel_tar.tar.xz
	rm kernel_tar.tar.xz
	dir="$(ls)"
	cd $dir
	echo "$OK Source-Files unpacked"
	path=/boot/config-$(uname -r)
	cp $path ./.config
	echo "$INFO Cleaning sources"
	make clean
	echo "$OK Done!"
	echo "$INFO Starting configuration..."
	make oldconfig
	echo "$OK Configured!"
	echo "$INFO Starting build-process... This may take a while!"
	make deb-pkg
	echo "$OK Finished building packages!"
	echo "$INFO Cleaning up..."
	cd ..
	rm -rf $dir
	echo "$OK Done!"
}
if [ "$(whoami)" != "root" ] 
then	
	echo "Run with Superuser Privileges!"
	exit 2
fi
if [ "$(dpkg -l | grep fakeroot)" == "" ]
then 
	apt install fakeroot -y
fi
if [ "$(dpkg -l | grep build-essential)" == "" ]
then 
	apt install build-essential -y
fi
case "$1" in
build)
	build_kernel $2 $3
	;;
install)
	build_kernel $2 $3
	echo "$INFO Installing Images"
	image="$(ls | grep -m 1 image)"
	headers="$(ls | grep headers)"
	libc="$(ls | grep libc)"
	dpkg -i $image $headers $libc
	echo "$OK Done!"
	echo "$INFO Cleaning up"
	cd ..
	rm -rf kernel
	echo "$OK Done."
	echo "$OK Kernel was upgraded successfully"

	;;
*)
	echo "Usage: builder <build|install> <link to kernel tar> <link to kernel sign>"
	echo "Run with Superuser Privileges!"
	exit 2
esac
