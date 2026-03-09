#include "elementlisty.h"
#include <memory>


ElListyInt::ElListyInt(const Lot& kllot, std::shared_ptr<ElListyInt> nastepny) : lot(kllot), nastepny(std::move(nastepny)) {}