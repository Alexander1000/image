files=()

for i in $(find ./build -name build_*.sh);
do
    filename="${i##*/}"
    base="${filename%.[^.]*}"
    build="${base##build_}"
    files+=($build)
done

for ((i=0; i < ${#files[@]}; i++))
do
    if [[ $1 == "" || $1 == ${files[$i]} ]];
    then
        echo "Build: " ${files[$i]}
        ./build/build_${files[$i]}.sh
    fi
done