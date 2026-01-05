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

void Ewma::reset(uint64_t currentTimeUs) {
	this->hasInitial_ = false;
	this->lastTimeUs_ = currentTimeUs;
}

double Ewma::filter(double input, uint64_t currentTimeUs) {
	lastTimeUs_ = currentTimeUs;
	if (hasInitial_) {
		output_ = alphaForPeriod(currentTimeUs - lastTimeUs_) * (input - output_) + output_;
	} else {
		output_ = input;
		hasInitial_ = true;
	}
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
