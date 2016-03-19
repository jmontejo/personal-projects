#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include "TGraph.h"

class Result : public TObject {

	public:
		std::vector<float> best_cuts;
		float best_metric;
		TGraph performance;

};

#endif
