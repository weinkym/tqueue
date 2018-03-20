basepath=$(cd `dirname $0`; pwd)
echo basepath=${basepath}
cd ${basepath}

CMAKE_CHECK=$(which cmake)

if [ ! ${CMAKE_CHECK} ]; then
  echo "cmake is not found"
  exit
else
  echo "cmake is found ${CMAKE_CHECK}"
fi 
# exit


P_PROGRAM_NAME_LIST+=(program1)
P_PROGRAM_NAME_LIST+=(program2)

for path in ${P_PROGRAM_NAME_LIST[@]}; do
	CHECK_FILE_PATH_LIST+=(${basepath}/${path}/CMakeLists.txt)
done


CHECK_FILE_PATH_LIST+=(${P_PROGRAM1_CMAKE_FILE_PATH})
CHECK_FILE_PATH_LIST+=(${P_PROGRAM2_CMAKE_FILE_PATH})


P_CHECK_PARAM=-f
for P_CHECK_PATH in ${CHECK_FILE_PATH_LIST[@]}; do
	if [ ! $P_CHECK_PARAM "$P_CHECK_PATH" ]; then
		echo "$P_CHECK_PATH is not exist" | tee -a ${LOG_FILE_PATH}
		exit
	else
		echo "$P_CHECK_PATH is ok" | tee -a ${LOG_FILE_PATH}
	fi
done

 
BUILD_DIR_PREFIX_NAME=build
for path in ${P_PROGRAM_NAME_LIST[@]}; do
	BUILD_PATH=${basepath}/${BUILD_DIR_PREFIX_NAME}_${path}
	if [ -d "${BUILD_PATH}" ]; then
		rm -rf ${BUILD_PATH}
	fi
	mkdir ${BUILD_PATH}
	cd ${BUILD_PATH}
	cmake ../${path}
	make
done

