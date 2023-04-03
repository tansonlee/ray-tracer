# Get the next filename
count=`cat count.txt`
temp_file=`mktemp`
full_temp_file="${temp_file}.ppm"
filename="../results/image${count}.png"
echo $((count+1)) > count.txt

echo "Starting"
./main 1> $full_temp_file
ffmpeg -i $full_temp_file $filename 2> /dev/null
echo "Finished"
rm $full_temp_file