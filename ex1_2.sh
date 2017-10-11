#cut -d"|" -f 6 $1 > aw.txt

while read p; do
	TEMP=`grep -F "$p" aw.txt | wc -l`
	if [ $TEMP -ne 0 ]; then
		echo "$p $TEMP"
		echo "$p $TEMP" >> update.txt
	fi 
done < prefix.txt
