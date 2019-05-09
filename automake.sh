CPP_FLAG="-pthread"

do_make() {
	src=$1
	dst=${src%.cpp*}
	
	valid=true
	if [ ! -f "${src}" ]; then
		valid=false
		echo "error: no a source file."
	fi
	
	if [ ! "${valid}" = true ]; then
		echo "usage: $0 <srcfile>"
		exit 1
	fi
	
	echo "g++ -g -Wall -std=c++14 -o ${dst} ${src} ${CPP_FLAG}"
	g++ -g -Wall -std=c++14 -o ${dst} ${src} ${CPP_FLAG}
}

do_clean() {
	for var in $(ls)
	do
		if [ $1 != $var -a -x $var ]; then
			echo "rm $var"
			rm $var
		fi
	done
}

if [ $# -lt 1 ]; then
	echo "error: wrong param."
	echo "usage: $0 <srcfile>"
	exit 1
elif [ $1 != "clean" ]; then
	do_make $*
else
	do_clean ${0#*/}
fi
