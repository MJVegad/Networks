echo "Please wait......"
RIB=$1

echo "Please wait...."

#cut -d"|" -f 6,7 $RIB > temp.txt
#cut -d"|" -f 1 temp.txt | uniq > as.txt
#cut -d"|" -f 1 temp.txt | uniq > prefix.txt
#cut -d"|" -f 2 temp.txt | uniq > aspaths.txt

echo "Entering the while loop..."

while read p; do
        for i in $p; do
		echo $i
		echo $i >> as.txt
	done
        
done < aspaths.txt

ASES=`sort -n as.txt | uniq | wc -l | cut -d" " -f 1`
PRE=`wc -l prefix.txt | cut -d" " -f 1`

#echo "No. of uniq ASes = $ASES"
#echo "No. of ip prefix = $PRE"
#rm temp.txt


