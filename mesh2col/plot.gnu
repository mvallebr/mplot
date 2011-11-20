set terminal png small color
set output 'saida.png'
set title "Distribucion de Impedancia Electrica"
set dgrid3d 80,80,4
set hidden3d
#set data style lines
set data style points
set grid
set cntrparam levels 20 
set cntrparam bspline
set contour
set xlabel "x
set ylabel "y
#set xrang [-.15:.15]
#set yrang [-0.15:.15]
#set logscale z
splot "data.dat"
