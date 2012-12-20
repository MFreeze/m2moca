#/bin/sh
if [ ! -f target/main.pdf ]; then
	echo 'pdf not found, compiling...'
	./build.sh
fi
zathura target/main.pdf&
