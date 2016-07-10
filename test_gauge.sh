images=()

for i in $(find ./images -name *.jpg);
do
    filename="${i##*/images/}"
    base="${filename%.[^.]*}"
    images+=($base)
done

for ((i=0; i < ${#images[@]}; i++))
do
    if [[ $1 == "" || $1 == ${images[$i]} ]];
    then
        echo ${images[$i]}

        for ((j=0; j < ${#images[@]}; j++))
        do
            echo ${images[$i]} "=" ${images[$j]}
            ./bin/gauge ./images/${images[$i]}.bin ./images/${images[$j]}.k
        done

        echo ""
    fi
done
