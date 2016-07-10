for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"

    if [[ "$#" == 4 ]];
    then
        if [[ $1 == "" || $1 == $base ]];
        then
            echo "Found: " $base.jpg "..."
            echo "rgbmul:" $base.jpg "=>" rgbmul_$base.jpg
            ./bin/rgbmul ./images/$base.jpg ./images2/rgbmul_$base.jpg $2 $3 $4
            echo ""
        fi
    fi
done