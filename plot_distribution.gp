reset
set xlabel 'bucket num'
set ylabel 'num of key in bucket'
set style fill solid
set grid
set title 'distribution'
set term png enhanced font 'Verdana,10'
set output 'fig/dist_1024_original.png'

plot [0:1024][:5] 'dat/distribution.dat' with histogram title 'key count'