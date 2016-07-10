for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"

    if [[ $1 == "" || $1 == $base ]];
    then
        echo "Found: " $base.jpg "..."
        echo "bricks:" $base.jpg "=>" bricks_$base.jpg
        ./bin/bricks ./images/$base.jpg ./images2/bricks_$base.jpg 5 5 1
        echo ""
    fi
done