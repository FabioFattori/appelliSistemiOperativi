#!/bin/bash
mostRecentFile=0;
for file in `find /usr/include/linux/ -mindepth 2 -name "*" -print`; do
	if [[ ${mostRecentFile} == 0 ]]; then
		mostRecentFile=${file};
	elif [[ ${mostRecentFile} -ot ${file} ]]; then
		mostRecentFile=${file};
	fi
done

echo "il file più recente è => ${mostRecentFile}";
