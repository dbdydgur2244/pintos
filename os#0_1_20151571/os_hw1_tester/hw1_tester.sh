#/bin/sh
if [ $# -ne 1 ]
then
	echo "How to Use) \$sh hw1_tester.sh [path/of/binary/to/test]"
	exit 1;
fi

EXEC=$1
INPUTS=$(ls *\.in)
TARGETDIR=$(dirname $1)
TOTAL=0
ANSWER=0
rm -f *.output Score.txt
echo "Tesing: $1"
for input in $INPUTS
do
	trimmed=$(echo $input | sed 's/\./-/' | sed 's/-in//')
	echo -n "Running test: $trimmed ... "
	$($1 < $input > $trimmed.output) 
	diff -Bbi $trimmed.out $trimmed.output > $trimmed.result
	if [ $? -eq 0 ]
	then
		echo "pass"
		ANSWER=`expr $ANSWER + 1`
	else
		echo "fail"
	fi
	TOTAL=`expr $TOTAL + 1`
done
echo "Result: $ANSWER / $TOTAL"
echo "$ANSWER" > Score.txt


