images=(derevo car profile iphone iphone2 duhi)

for ((i=0; i < ${#images[@]}; i++))
do
    echo "binfilter: " ${images[$i]}.jpg " => " ${images[$i]}.bin
    ./bin/binfilter ./images/${images[$i]}.jpg ./images/${images[$i]}.bin
    echo "buildcostmap: " ${images[$i]}.bin " => " ${images[$i]}.k
    ./bin/buildcostmap ./images/${images[$i]}.bin ./images/${images[$i]}.k
done