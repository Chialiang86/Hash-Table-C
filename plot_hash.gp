reset
set xlabel 'value'
set ylabel 'hash value'
set style fill solid
set grid
set title 'hash result'
set term png enhanced font 'Verdana,10'
set output 'fig/hash_2048_original.png'

plot [:2200][:2200] 'dat/generate.dat' with points title 'hash'