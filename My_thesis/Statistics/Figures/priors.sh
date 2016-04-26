shopt -s expand_aliases
reset_expansion(){
   trap "set +f; exit" INT TERM EXIT
  #CMD="$1"; shift; ARG="$@";
  #$CMD $ARG; set +f
  ARG="$@";
  $ARG; set +f
  trap - INT TERM EXIT
}
for i in 0.05 0.1 0.15 0.2 0.3 0.4; do root -q -b -l priors.C"("$i")"; done
set -f; reset_expansion python $MYSVN/scripts/renamer/rename.py -f prior0.*eps prior0_*eps; set +f
