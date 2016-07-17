for i in $(find ./images -name *.jpg);
do
    filename="${i##*/images/}"
    base="${filename%.[^.]*}"
    echo "Found:" ./images/$base.jpg "=>" ./images2/hsv/$base.jpg
    ./bin/hsvmul ./images/$base.jpg ./images2/hsv/$base.jpg 1.0 100.0 255.0
    #images+=($base)
done