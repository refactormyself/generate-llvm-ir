
CURR_DIR=$PWD
mkdir "build"
cd build || exit

cmake ..
make
make test

./cli_app_basic
./test/common_tests

cd $CURR_DIR