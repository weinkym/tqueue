basepath=$(cd `dirname $0`; pwd)
echo basepath=${basepath}
cd ${basepath}

#重复测试次数
IN_REPEAT_TIMES=30

#测试时间，单位：秒
IN_TEST_SECS_LIST+=(1)
IN_TEST_SECS_LIST+=(5)
IN_TEST_SECS_LIST+=(10)
IN_TEST_SECS_LIST+=(30)
IN_TEST_SECS_LIST+=(60)

LOG_FILE_PATH=${basepath}/log.txt

CHECK_FILE_PATH_LIST+=(${basepath}/program1)
CHECK_FILE_PATH_LIST+=(${basepath}/program2)

echo "===" | tee ${LOG_FILE_PATH}

P_CHECK_PARAM=-f
for P_CHECK_PATH in ${CHECK_FILE_PATH_LIST[@]}; do
	if [ ! $P_CHECK_PARAM "$P_CHECK_PATH" ]; then
		echo "$P_CHECK_PATH is not exist" | tee -a ${LOG_FILE_PATH}
		exit
	else
		echo "$P_CHECK_PATH is ok" | tee -a ${LOG_FILE_PATH}
	fi
done

for secs in ${IN_TEST_SECS_LIST[@]}; do
	result1=0
	result2=0

	for ((i=1; i<=${IN_REPEAT_TIMES}; i ++)); do  
		REST=`./program1 ${secs} | grep -o '[0-9]\+'`
		echo program1运行${secs}秒,完成次数=${REST} | tee -a ${LOG_FILE_PATH}
		result1=$[$REST+$result1]
	done
	for ((i=1; i<=${IN_REPEAT_TIMES}; i ++)); do  
		REST=`./program2 ${secs} | grep -o '[0-9]\+'`
		echo program2运行${secs}秒,完成次数=${REST} | tee -a ${LOG_FILE_PATH}
		result2=$[$REST+$result2]
	done  
	# rate=$(echo "${result1}*1.0/${result2}"|bc)
	rate=$(echo "scale=4;${result1}/${result2}" | bc)
	echo 运行${secs}秒 program1 平均次数=$[$result1/$IN_REPEAT_TIMES],program2 平均次数=$[$result2/$IN_REPEAT_TIMES] 比例：${rate} | tee -a ${LOG_FILE_PATH}

done


# MAJOR=`grep ${REST} | grep -o '[0-9]\+'`



