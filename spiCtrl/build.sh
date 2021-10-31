rm -rf ./build
rm -rf ./lib

test -e build/ || mkdir build
echo "mkdir build/"

test -e lib/ || mkdir lib
echo "mkdir lib/"

cd build/
cmake ..
make
