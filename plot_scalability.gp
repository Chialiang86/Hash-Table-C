reset     
set xtics font ", 6"
set xlabel 'load factor (n/b)'
set ylabel 'rate'
set style fill solid
set grid
set title 'Scalability'
set term png enhanced font 'Verdana,10'
set output 'fig/scalability.png'

plot [:][0:1] 'dat/scalability.dat' using 2:xtic(1) with linespoints title 'non-used rate', \
'' using 3:xtic(1) with linespoints title 'exactly-one rate', \
'' using 4:xtic(1) with linespoints title 'more-than-one rate', \
'' using 5:xtic(1) with linespoints title 'collision rate'