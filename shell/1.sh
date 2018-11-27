#########################################################################
# File Name: 1.sh
# Author: liuguangsheng
# mail: ###############
# Created Time: 2018年11月27日 星期二 17时12分17秒
#########################################################################
#!/bin/bash
echo "start"
FileName=`find ./ -maxdepth 1 -name "*.txt"`
str=ro.product.tt
ret=1
for file in $FileName
do
	((ret++))
	str1=$str$ret
	echo $str1
	echo  $str1 >> $file
done
echo "done"
