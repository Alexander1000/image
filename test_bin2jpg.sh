for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"
    images+=($base)

    echo "bin2jpg:" $base.bin "=>" bw_$base.jpg
    ./bin/bin2jpg ./images/$base.bin ./images2/bw_$base.jpg
done