if [ $# -eq 1 ]; then
  filter=$1
else
  filter="/"
fi
echo "###- Check por hline instead or xrule"
grep hline -l */*tex | grep $filter
echo "###- Check for cross section without macro"
          grep ross.section -l */*tex | grep -v Resumen | grep $filter
echo "###- Check for commas and dots after formulae"
echo "###- Check for thesis -> dissertation"
          grep " thesis" -l */*tex | grep $filter
echo "###- parameterize: Dominant modern spelling in US."
          grep "arameteris" -l */*tex | grep $filter
          grep "arametris" -l */*tex | grep $filter
echo "###- Check for systematics -> systematic uncertainties"
          grep "systematics" -l */*tex | grep $filter
echo "###- Check for centre -> center"
          grep "centre" -l */*tex | grep $filter
          grep "Centre" -l */*tex | grep $filter
echo "###- Check for FIXME"
          grep "FIXME" -l */*tex | grep $filter
echo "###- Check for Z and W"
          grep " Z " -l */*tex | grep -v ATLAS | grep -v Stop2 | grep $filter 
          grep " W " -l */*tex | grep $filter
echo "###- Check for Monte Carlo"
          grep "Monte Carlo" -l */*tex | grep -v MCsimulation | grep $filter
echo "###- Check for 'allows to' "
          grep "allows to" -l */*tex | grep $filter
echo "###- Check space before ref"
          grep " \\ref{" -l */*tex | grep $filter
echo "###- Check space before cite"
          grep " \\cite{" -l */*tex | grep $filter
echo "###- Check for duuble space"
          grep " ~" -l */*tex | grep -v Theory | grep $filter
echo "###- Check for 't"
          grep "'t" -l */*.tex  | grep $filter
echo "###- Check for heavy' 'flavor"
          grep "eavy flavor" -l */*.tex  | grep $filter
echo "###- SPELL CHECK!"
