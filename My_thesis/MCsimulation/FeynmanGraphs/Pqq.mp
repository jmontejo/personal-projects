% Pqq.mp -- do not edit, generated automatically by FeynmanGraphGen.tex
input feynmp
require_RCS_revision "1.30";
beginchar(1, 62*1.0pt#, 62*1.0pt#, 0);
"feynmf: 1";
LaTeX_unitlength:=1.0pt;
subgraph (0, 0, w, h);
vinit;
pickup pencircle scaled thin;
vleft (__i1);
vright (__o1, __o2);
vlabel ("$f_{q}(x)$", __i1);
vlabel ("$f_{q}(y)$", __o2);
vlabel ("$f_{g}(x-y)$", __o1);
vlabel ("$P_{qq}(z) = \frac {4}{3}\frac {1+z^2}{1-z}$", __v1);
vconnect ("fermion", __i1, __v1);
vconnect ("fermion", __v1, __o2);
vconnect ("gluon", __v1, __o1);
vfreeze;
vdraw;
endsubgraph;
endchar;
% the end.
end.
endinput;
