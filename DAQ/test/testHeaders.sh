#! /bin/bash
#
# Run the tests to create a file with dummy header objects and read it back
#

if [[ -e mark.1 ]]; then
  rm mark.1
fi

if [[ -e mark.2 ]]; then
  rm mark.2
fi

mu2e -c Offline/DAQ/test/generateTestHeaders.fcl | grep Mark > mark.1
echo "generate job status: " $?
echo " "

mu2e -c Offline/DAQ/test/analyzeTestHeaders.fcl | grep Mark > mark.2
echo "read job status:     " $?
echo " "

echo "Check sizes of output files: "
wc mark.1 mark.2

echo "Diff the two files: "
diff mark.1 mark.2
stat=$?
echo $stat

if [[ "$stat" == "0" ]]; then
  echo "Exact output match.  Tests pass."
  rm mark.1 mark.2
else
  echo "Output does not match: " $stat
fi
