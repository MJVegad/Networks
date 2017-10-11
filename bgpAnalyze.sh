echo "Please wait......"
cut -d"|" -f 5,6 $1 > temp.txt
cut -d"|" -f 1 temp.txt | uniq > as.txt
cut -d"|" -f 2 temp.txt | uniq > prefix.txt

ASES=`wc -l as.txt | cut -d" " -f 1`
PRE=`wc -l prefix.txt | cut -d" " -f 1`

echo "No. of uniq ASes = $ASES"
echo "No. of ip prefix = $PRE"
rm temp.txt
rm as.txt
rm prefix.txt


