#!/bin/bash

sed -e '1i frmsf="' -e '$a ";' ${1} | \
    perl -pe 's/\n/ /g' | \
    sed -E -e 's/ +/ /g' -e 's/" /"/g' -e 's/ "/"/g' > \
        ${1%frmsf}js
