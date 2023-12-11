#!/bin/bash

for file in `find  /usr/include/linux/ -maxdepth 1 -name "*f*.h" -print`;do
	NRighe=`wc -l ${file}`;
	NRighe=${NRighe%% *};
	if [[ ${NRighe} -ge 10 && ${NRighe} -le 100  ]]; then
		tail -n 1 ${file} | wc -c;
	fi
done
