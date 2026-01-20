/*
 * EWMA Filter - Exponentially Weighted Moving Average filter used for smoothing data series readings.
 *
 *     output = alpha * reading + (1 - alpha) * lastOutput
 *
 * Where:
 *  -   alpha = factor greater than 0 and less or equal to 1
 *  -   reading = current input value
 *  -   lastOutput = last filter output value
 *  -   output = filter output value after the last reading
 *
 */

 #include <stdint.h>
 #include <math.h>

#ifndef EWMA_H_
#define EWMA_H_

class Ewma {
public:
	/*
	 * Current data output
	 */
	inline double output() const {
		return output_;
	}

	/*
	 * Smoothing factor time value (loosely related to time constant)
	 */
	inline double tauValue() const {
		return tau_;
	}

	inline void setTauValue(double tau) {
		tau_ = tau;
	}

	inline double alpha() const {
		return 1.0 - exp(-1.0 / tau_);
	}

	inline double setAlpha(double alpha) {
		tau_ = -1.0 / log(1.0 - alpha);
		return tau_;
	}

	inline double alphaForPeriod(uint64_t periodUs) const {
		double value = exp(-((double)periodUs) / tau_);
		if(isnan(value))
			value = 0;
			
		return 1.0 - value;
	}

	/*
	 * Creates a filter without a defined initial output. The first output will be equal to the first input.
	 */
	Ewma(double tau);

	/*
	 * Creates a filter with a defined initial output.
	 */
	Ewma(double tau, double initialOutput, uint64_t currentTimeUs);

	void reset();

	/*
	 * Specifies a reading value.
	 * @returns current output
	 */
	double filter(double input, uint64_t currentTimeUs);

	double filter(double input);

private:
	bool hasInitial_ = false;
	uint64_t lastTimeUs_ = 0;
	double output_ = 0;
	double tau_ = 0;
};

#endif /* EWMA_H_ */
