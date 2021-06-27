reset     
set xtics font ", 6"
set xlabel 'load factor (n/b)'
set ylabel 'rate (%)'
set style fill solid
set grid
set title 'Scalability Compare'
set term png enhanced font 'Verdana,10'
set output 'fig/scalability_cmp.png'

plot [:][0:100] 'dat/scalability_cmp.dat' using 2:xtic(1) with linespoints title 'shift 22 bits', \
'' using 3:xtic(1) with linespoints title 'shift 20 bits', \
'' using 4:xtic(1) with linespoints title 'shift 18 bits', \
'' using 5:xtic(1) with linespoints title 'shift 16 bits', \
'' using 6:xtic(1) with linespoints title 'shift 14 bits', \
'' using 7:xtic(1) with linespoints title 'shift 12 bits', \
'' using 8:xtic(1) with linespoints title 'shift 10 bits', \
'' using 9:xtic(1) with linespoints title 'shift 8 bits', \
'' using 10:xtic(1) with linespoints title 'shift 6 bits', \
'' using 11:xtic(1) with linespoints title 'shift 4 bits', \
'' using 12:xtic(1) with linespoints title 'shift 2 bits', \
'' using 13:xtic(1) with linespoints title 'shift 0 bits'