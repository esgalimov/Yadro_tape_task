project_dir=$(dirname "$(realpath $0)")/..

obj=${project_dir}/build/Yadro_tape_task

test_folder=${project_dir}/tests/sort_test/

echo "UNIT TESTS:"
echo
for ((i = 0; i <= 11; i++)) do
    echo test $i
    time ${obj} -nb ${test_folder}$i/$i.in ${test_folder}$i/$i.out ${test_folder}$i/config_$i.txt
    echo $ans

    ans=`diff ${test_folder}$i/$i.out ${test_folder}$i/$i.ans`

    if [ "$ans" == "" ]
    then
    echo -e "\033[0;32mCORRECT\033[0m"
    else
    echo -e "\033[0;31mERROR\033[0m"
    fi
    echo
    echo
    echo
done