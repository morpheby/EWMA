#ifndef EWMAT_H_
#define EWMAT_H_

/*
 * Filter template that allows restriction to only one specific data type, such as uint32_t. Avoiding floating point arithmetics 
 * can significantly decrease code footprint, especially in embeded devices, such as Arduino or STM32.
 * 
 * If you want to create a filter for integers, with an alpha value of 0.03, you will just do the following:
 *
 *    EwmaT <int> filter(3, 100)
 * 
 */
template <typename T>
class EwmaT {
public:
    /*
    * Creates a filter with a defined initial output.
    */
    EwmaT(T alpha, unsigned int alphaScale);
    /*
    * Creates a filter with a defined initial output.
    */
    EwmaT(T alpha, unsigned int alphaScale, T initialOutput);

    void reset();

    T output();

    /*
    * Specifies a reading value.
    * @returns current output
    */
    T filter(T input);

    T alpha() const;
    unsigned int alphaScale() const;
    void setAlpha(T alpha, unsigned int alphaScale);

private:
    void init(T alpha, unsigned int alphaScale, T initialOutput);

    /*
     * Smoothing factor, in range [0,alphaScale]. Higher the value - less smoothing (higher the latest reading impact).
     */
    T _alpha;
    T outputScaled;
    unsigned int _alphaScale;
    bool hasInitial;
};

template <typename T>
EwmaT<T>::EwmaT(T alpha, unsigned int alphaScale) {
    init(alpha, alphaScale, 0);
    this->hasInitial = false;
}

template <typename T>
EwmaT<T>::EwmaT(T alpha, unsigned int alphaScale, T initialOutput) {
    init(alpha, alphaScale, initialOutput);
}

template <typename T>
void EwmaT<T>::init(T alpha, unsigned int alphaScale, T initialOutput) {
    this->_alpha = alpha;
    this->_alphaScale = alphaScale;
    this->outputScaled = initialOutput * alphaScale;
    this->hasInitial = true;
}

template <typename T>
void EwmaT<T>::reset() {
    this->hasInitial = false;
}

template <typename T>
T EwmaT<T>::filter(T input) {
    if (hasInitial) {
        outputScaled = _alpha * input + (_alphaScale - alpha) * outputScaled / _alphaScale;
    } else {
        outputScaled = input * _alphaScale;
        hasInitial = true;
    }
    return output();
}

template <typename T>
T EwmaT<T>::output() {
    return (outputScaled + _alphaScale / 2) / _alphaScale;
}

template <typename T>
T EwmaT<T>::alpha() const {
    return _alpha;
}

template <typename T>
void EwmaT<T>::setAlpha(T alpha, unsigned int alphaScale) {
    outputScaled = ((outputScaled + _alphaScale / 2) / _alphaScale) * alphaScale;
    _alphaScale = alphaScale;
    _alpha = alpha;
}

template <typename T>
unsigned int EwmaT<T>::alphaScale() const {
    return _alphaScale;
}

#endif /* EWMAT_H_ */
