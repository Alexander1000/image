Example usage:
# compiling jpg to bin-format
buildfilter derevo.jpg drevo.bin

# export bin-format to black-white jpg-image (for debug bin compile)
bin2jpg derevo.bin bw_derevo.jpg

# build cost map for bin-file in file with koefficients
buildcostmap derevo.bin derevo.k

# gauge for bin-file by k-koefficients
gauge derevo.bin derevo.k