#include "Ewma.h"

Ewma::Ewma(double tau) {
	this->tau_ = tau;
}

Ewma::Ewma(double tau, double initialOutput, uint64_t currentTimeUs) {
	this->tau_ = tau;
	this->output_ = initialOutput;
	this->hasInitial_ = true;
	this->lastTimeUs_ = currentTimeUs;
}

void Ewma::reset() {
	this->hasInitial_ = false;
}

double Ewma::filter(double input, uint64_t currentTimeUs) {
	if (hasInitial_) {
		output_ = alphaForPeriod(currentTimeUs - lastTimeUs_) * (input - output_) + output_;
	} else {
		output_ = input;
		hasInitial_ = true;
	}
	lastTimeUs_ = currentTimeUs;
	return output_;
}

double Ewma::filter(double input) {
	if (hasInitial_) {
		output_ = alpha() * (input - output_) + output_;
	} else {
		output_ = input;
		hasInitial_ = true;
	}
	return output_;
}
