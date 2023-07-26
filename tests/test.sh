#/bin/bash
res=`cat ${1:-"ip_filter.tsv"} | ${2:-"../build/ip_filter"} | md5sum`
echo "$res `[[ $res == "24e7a7b2270daee89c64d3ca5fb3da1a"* ]] && echo "YES" || echo "NO"`"