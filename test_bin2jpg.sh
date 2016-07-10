for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"

    if [[ $1 == "" || $1 == $base ]];
    then
        echo "bin2jpg:" $base.bin "=>" bw_$base.jpg
        ./bin/bin2jpg ./images/$base.bin ./images2/bw_$base.jpg
    fi
done