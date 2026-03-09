#ifndef ELEMENTLISTY_H
#define ELEMENTLISTY_H

#include "klasalot.h"


class ElListyInt {
public:
    Lot lot;
    std::shared_ptr<ElListyInt> nastepny;

    ElListyInt(const Lot& lot, std::shared_ptr<ElListyInt> nastepny = nullptr);
};

template<typename U>
class ElListyLInt {
public:
    U lot;
    std::shared_ptr<ElListyLInt> nastepny;

    ElListyLInt(const U& kllot, std::shared_ptr<ElListyLInt> nastepny = nullptr) : lot(kllot), nastepny(nastepny) {}
};

#endif

