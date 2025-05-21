cc=clang++
cflags='-std=c++17 -Wall -Wextra -pipe -fsanitize=address -fsanitize=undefined'

required_cflags='-fno-strict-aliasing -fwrapv -fno-exceptions -fno-rtti'
mode=$([ -z "$1" ] && echo "debug" || echo "$1" )

set -eu

echo "Build mode: $mode"
case $mode in
	"debug") $cc -std=c++17 -g -O0 $cflags $required_cflags main.cpp lib.cpp -o Example.exe ;;
	"release") $cc -std=c++17 -O2 -s $cflags $required_cflags main.cpp lib.cpp -o Example.exe ;;
esac

./Example.exe
