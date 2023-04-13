#!/bin/bash

perl -pe 's/\n/ /g' ${1} | \
sed -E \
    -e 's/^ */frmsf="/g' \
    -e 's/ +/ /g' \
    -e 's/ *$/";/g' \
    > ${1%frmsf}js
