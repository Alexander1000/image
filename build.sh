files=(binfilter bin2jpg buildcostmap gauge pixelator)

for ((i=0; i < ${#files[@]}; i++))
do
    if [[ $1 == "" || $1 == ${files[$i]} ]];
    then
        echo "Build: " ${files[$i]}
        ./build/build_${files[$i]}.sh
    fi
done