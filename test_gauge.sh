images=(car derevo iphone iphone2 duhi profile)

for ((i=0; i < ${#images[@]}; i++))
do
    echo ${images[$i]}

    for ((j=0; j < ${#images[@]}; j++))
    do
        echo ${images[$i]} "=" ${images[$j]}
        ./bin/gauge ./images/${images[$i]}.bin ./images/${images[$j]}.k
    done

    echo ""
done
