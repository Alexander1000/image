rm -f ./images/*.bin
rm -f ./images/*.k

for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"

    echo "Found: " $base.jpg "..."
    echo "binfilter: " $base.jpg " => " $base.bin
    ./bin/binfilter ./images/$base.jpg ./images/$base.bin
    echo "buildcostmap: " $base.bin " => " $base.k
    ./bin/buildcostmap ./images/$base.bin ./images/$base.k
    echo ""
done