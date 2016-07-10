for i in $(find ./images -name *.jpg);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"

    if [[ "$#" == 4 ]];
    then
        if [[ $1 == "" || $1 == $base ]];
        then
            echo "Found: " $base.jpg "..."
            echo "bricks:" $base.jpg "=>" bricks_$base.jpg
            ./bin/bricks ./images/$base.jpg ./images2/bricks_$base.jpg $2 $3 $4
            echo ""
        fi
    fi

    if [[ "$#" == 3 ]];
    then
        echo "Found: " $base.jpg "..."
        echo "bricks:" $base.jpg "=>" bricks_$base.jpg
        ./bin/bricks ./images/$base.jpg ./images2/bricks_$base.jpg $1 $2 $3
        echo ""
    fi
done