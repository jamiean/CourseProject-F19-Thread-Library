make clean
make
./test-mutex-1;
./test-mutex-2;
echo "running style check 02"
./test-style-2;
echo "running style check 03"
./test-style-3;
echo "running style check 03 join"
./test-style-3-join;
echo "running style check 04"
./test-style-4;
echo "running style check 05"
./test-style-5;
echo "running style check 06"
./test-style-6;
make clean
