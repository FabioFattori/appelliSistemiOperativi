#!/bin/bash

for fileName in `find /usr/include/linux/ -name "*.h" -type f -print`; do
	
		grep int "${fileName}" | cut -b -3
	
done
