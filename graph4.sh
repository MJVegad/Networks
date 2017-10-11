pushd $PARENT >/dev/null

grep '[+-]*/NodeList/0/D*' $TRACE | cut -d ' ' -f 1,2 | grep '[+-].*' > q2.txt
cut -d ' ' -f 1 q2.txt > q3.txt

COUNT=0
echo 0 > queuesize.txt
while read p; do
if [ "$p" = "+" ]; then
    COUNT=$(expr $COUNT + 1);
    echo $COUNT >> queuesize.txt;
else
    COUNT=$(expr $COUNT - 1);
    echo $COUNT >> queuesize.txt;
fi
done < q3.txt

tail -n +2 "queuesize.txt" > queuesize1.txt
cut -d ' ' -f 2 q2.txt > q1.txt
paste <(cat q1.txt)  <(cat queuesize1.txt) | tac | awk '!x[$1]++' | tac > queuegraph.txt

gnuplot <<- EOF
set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'queueocc.eps'
set xlabel '{/Helvetica-Oblique Time}'
set ylabel '{/Helvetica-Oblique Queuesize}'
unset key
set title 'QUEUEOCCUPANCY'
plot 'queuegraph.txt' with lines
EOF

#Removing temporary files
rm q1.txt
rm q2.txt
rm q3.txt
rm queuesize1.txt

popd > /dev/null


 


