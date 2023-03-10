# Get the next filename
count=`cat count.txt`
filename="./results/image${count}.ppm"
echo $((count+1)) > count.txt

echo "Starting"
./main 1> $filename
echo "Finished"
make clean