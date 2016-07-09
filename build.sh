files=(binfilter bin2jpg buildcostmap gauge pixelator)

for ((i=0; i < ${#files[@]}; i++))
do
    echo "Build: " ${files[$i]}
    ./build/build_${files[$i]}.sh
done