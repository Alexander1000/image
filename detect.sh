images=()
rm -f ./tmp/*.bin

for i in $(find ./detect -name *.jpg);
do
    images+=($i)
done

var=$( IFS=$'\n'; echo "${images[*]}" )
php ./gauge.php $var