shopt -s expand_aliases

echo "Setting up: ATLAS, voms, panda, ROOT..."
setupATLAS --quiet
voms-proxy-init -voms atlas -q 2>&1 | grep -v Error
localSetupPandaClient --quiet  --noAthenaCheck
localSetupROOT 5.34.17-x86_64-slc6-gcc4.7 --quiet --skipConfirm
