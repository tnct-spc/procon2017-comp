devicename=$(scanimage -L | sed -r '/scanner/!d'| sed -r "s/(^.+\`)|('.+$)//g")

if [ "${devicename}" = "" ]; then
	echo デバイス名の取得に失敗しました
	exit 2
else
	echo ${devicename} > devicename.txt
fi

