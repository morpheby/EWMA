#include "Ewma.h"

Ewma::Ewma(EWMA_FLOAT_TYPE tau) {
	this->tau_ = tau;
}

Ewma::Ewma(EWMA_FLOAT_TYPE tau, EWMA_FLOAT_TYPE initialOutput, uint64_t currentTimeUs) {
	this->tau_ = tau;
	this->output_ = initialOutput;
	this->hasInitial_ = true;
	this->lastTimeUs_ = currentTimeUs;
}

void Ewma::reset() {
	this->hasInitial_ = false;
}

EWMA_FLOAT_TYPE Ewma::filter(EWMA_FLOAT_TYPE input, uint64_t currentTimeUs) {
	if (hasInitial_) {
		output_ = alphaForPeriod(currentTimeUs - lastTimeUs_) * (input - output_) + output_;
	} else {
		output_ = input;
		hasInitial_ = true;
	}
	lastTimeUs_ = currentTimeUs;
	return output_;
}

EWMA_FLOAT_TYPE Ewma::filter(EWMA_FLOAT_TYPE input) {
	if (hasInitial_) {
		output_ = alpha() * (input - output_) + output_;
		if (std::isnan(output_)) {
			hasInitial_ = false;
		}
	} else {
		output_ = input;
		hasInitial_ = true;
	}
	return output_;
}
