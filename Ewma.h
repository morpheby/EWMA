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
 #include <cmath>

#ifndef EWMA_H_
#define EWMA_H_

#ifndef EWMA_FLOAT_TYPE
#define EWMA_FLOAT_TYPE double 
#endif

class Ewma {
public:
	/*
	 * Current data output
	 */
	inline EWMA_FLOAT_TYPE output() const {
		return output_;
	}

	/*
	 * Smoothing factor time value (loosely related to time constant)
	 */
	inline EWMA_FLOAT_TYPE tauValue() const {
		return tau_;
	}

	inline void setTauValue(EWMA_FLOAT_TYPE tau) {
		tau_ = tau;
	}

	inline EWMA_FLOAT_TYPE alpha() const {
		return EWMA_FLOAT_TYPE(1.0) - std::exp(EWMA_FLOAT_TYPE(-1.0) / tau_);
	}

	inline EWMA_FLOAT_TYPE setAlpha(EWMA_FLOAT_TYPE alpha) {
		tau_ = -EWMA_FLOAT_TYPE(1.0) / std::log(EWMA_FLOAT_TYPE(1.0) - alpha);
		return tau_;
	}

	inline EWMA_FLOAT_TYPE alphaForPeriod(uint64_t periodUs) const {
		EWMA_FLOAT_TYPE value = std::exp(-((EWMA_FLOAT_TYPE)periodUs) / tau_);
		if(std::isnan(value))
			value = 0;
			
		return EWMA_FLOAT_TYPE(1.0) - value;
	}

	/*
	 * Creates a filter without a defined initial output. The first output will be equal to the first input.
	 */
	Ewma(EWMA_FLOAT_TYPE tau);

	/*
	 * Creates a filter with a defined initial output.
	 */
	Ewma(EWMA_FLOAT_TYPE tau, EWMA_FLOAT_TYPE initialOutput, uint64_t currentTimeUs);

	void reset();

	/*
	 * Specifies a reading value.
	 * @returns current output
	 */
	EWMA_FLOAT_TYPE filter(EWMA_FLOAT_TYPE input, uint64_t currentTimeUs);

	EWMA_FLOAT_TYPE filter(EWMA_FLOAT_TYPE input);

private:
	bool hasInitial_ = false;
	uint64_t lastTimeUs_ = 0;
	EWMA_FLOAT_TYPE output_ = 0;
	EWMA_FLOAT_TYPE tau_ = 0;
};

#endif /* EWMA_H_ */
