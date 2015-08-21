vars = {
        "extHFtype"    : ("((extHFtype-(extHFtype%100))/100.)",         (100,-49.5,50.5), "HF type"   ),
        #"jet_n"       : ("jet_n",                   (20,0,20), "jet_n"        ),
        #"jet_n25"       : ("Sum$(jet_pt>25e3)",                   (20,0,20), "jet_n25"        ),
}

categories = {
        "realHFbb": ("(abs(extHFtype)>=100)",2,"inclusive"),
        #"realHFbbNoMPI": ("(extHFtype>=100)",2,"inclusive"),
}

