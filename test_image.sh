for i in $(find ./images -name *.jpg);
do
    filename="${i##*/images/}"
    base="${filename%.[^.]*}"
    echo "Found:" ./images/$base.jpg "=>" ./images2/hsv/$base.jpg
    # saturation + brightness
    ./bin/hsvmul ./images/$base.jpg ./images2/exp/$base.jpg 1.0 100.0 255.0

    ./bin/rgbmul ./images2/exp/$base.jpg ./images2/exp/$base.r.jpg 1.0 0.0 0.0
    ./bin/rgbmul ./images2/exp/$base.jpg ./images2/exp/$base.g.jpg 0.0 1.0 0.0
    ./bin/rgbmul ./images2/exp/$base.jpg ./images2/exp/$base.b.jpg 0.0 0.0 1.0

    ./bin/binfilter ./images2/exp/$base.jpg ./images2/exp/$base.hsv.bin
    ./bin/binfilter ./images2/exp/$base.r.jpg ./images2/exp/$base.r.bin
    ./bin/binfilter ./images2/exp/$base.g.jpg ./images2/exp/$base.g.bin
    ./bin/binfilter ./images2/exp/$base.b.jpg ./images2/exp/$base.b.bin

    ./bin/bin2jpg ./images2/exp/$base.hsv.bin ./images2/exp/$base.bw.hsv.jpg
    ./bin/bin2jpg ./images2/exp/$base.r.bin ./images2/exp/$base.bw.r.jpg
    ./bin/bin2jpg ./images2/exp/$base.g.bin ./images2/exp/$base.bw.g.jpg
    ./bin/bin2jpg ./images2/exp/$base.b.bin ./images2/exp/$base.bw.b.jpg
    ./bin/bin2jpg ./images/$base.bin ./images2/exp/$base.bw.jpg

    ./bin/buildcostmap ./images2/exp/$base.hsv.bin ./images2/exp/$base.hsv.k
    ./bin/buildcostmap ./images2/exp/$base.r.bin ./images2/exp/$base.r.k
    ./bin/buildcostmap ./images2/exp/$base.g.bin ./images2/exp/$base.g.k
    ./bin/buildcostmap ./images2/exp/$base.b.bin ./images2/exp/$base.b.k

    ./bin/cost2jpg ./images2/exp/$base.hsv.k ./images2/exp/$base.cost.hsv.jpg
    ./bin/cost2jpg ./images2/exp/$base.r.k ./images2/exp/$base.cost.r.jpg
    ./bin/cost2jpg ./images2/exp/$base.g.k ./images2/exp/$base.cost.g.jpg
    ./bin/cost2jpg ./images2/exp/$base.b.k ./images2/exp/$base.cost.b.jpg
done