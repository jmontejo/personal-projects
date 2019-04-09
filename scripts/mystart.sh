shopt -s expand_aliases

echo "Setting up: ATLAS, voms, git"
setupATLAS
lsetup git
voms-proxy-init -voms atlas < $MYGIT/scripts/voms.pwd
echo "done"
