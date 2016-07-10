images=()


for i in $(find ./images -name *.jpg);
do
    filename="${i##*/images/}"
    base="${filename%.[^.]*}"
    images+=($base)
done

for ((i=0; i < ${#images[@]}; i++))
do
    echo ${images[$i]}
    ./bin/gauge $1 ./images/${images[$i]}.k
done
