///
/// @file KBit.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-05
/// @brief exactly mute K bits distinct
///

#include <optimization/metaheuristic/operator/mutation/KBit.h>

namespace stocos {

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
KBit<SOL, TYPE_FITNESS, TYPE_CELL>::KBit(std::mt19937& mt_rand, unsigned int k)
    : AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>(mt_rand),
      rid(std::make_unique<std::uniform_int_distribution<unsigned int>>(0, 1)), _k(k), N(1) {
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
KBit<SOL, TYPE_FITNESS, TYPE_CELL>::~KBit() {
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
void KBit<SOL, TYPE_FITNESS, TYPE_CELL>::operator()(SOL& s) {
    backup.clear();
    std::unique_ptr<std::vector<std::pair<unsigned int, TYPE_CELL>>> list = listOfMutations(s);
    for(unsigned int i = 0; i < list->size(); i++) {
        s((*list)[i].first, (*list)[i].second);
        backup.push_back((*list)[i].first);
    }
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
void KBit<SOL, TYPE_FITNESS, TYPE_CELL>::cancelMutations(SOL& s) const {
    for(unsigned int i = 0; i < backup.size(); i++) {
        if(s(backup[i]) == 1)
            s(backup[i], 0);
        else
            s(backup[i], 1);
    }
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
std::unique_ptr<std::vector<std::pair<unsigned int, TYPE_CELL>>>
    KBit<SOL, TYPE_FITNESS, TYPE_CELL>::listOfMutations(const SOL& s) {
    assert(_k <= s.sizeArray());

    std::unique_ptr<std::vector<std::pair<unsigned int, TYPE_CELL>>> list(
        std::make_unique<std::vector<std::pair<unsigned int, TYPE_CELL>>>());

    if(s.sizeArray() != N) {
        N = s.sizeArray();
        rid = std::make_unique<std::uniform_int_distribution<unsigned int>>(0, N);
    }

    while(list->size() != _k) {
        unsigned int element = rid->operator()(this->_mt_rand);
        bool findElement = false;
        for(unsigned int i = 0; i < list->size(); i++) {
            if((*list)[i].first == element) { findElement = true; }
        }

        if(!findElement) {
            if(s(element) == 1)
                list->push_back(std::pair<unsigned int, TYPE_CELL>(element, 0));
            else
                list->push_back(std::pair<unsigned int, TYPE_CELL>(element, 1));
        }
    }

    return std::move(list);
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
void KBit<SOL, TYPE_FITNESS, TYPE_CELL>::applyOperator(
    SOL& s, const std::vector<std::pair<unsigned int, TYPE_CELL>>& _listOfMutations) {
    backup.clear();
    for(unsigned int i = 0; i < _listOfMutations.size(); i++) {
        s(_listOfMutations[i].first, _listOfMutations[i].second);
        backup.push_back(_listOfMutations[i].first);
    }
}

template class KBit<Solution<double, double>, double, double>;
template class KBit<Solution<double, bool>, double, bool>;
template class KBit<Solution<int, bool>, int, bool>;
template class KBit<Solution<unsigned int, bool>, unsigned int, bool>;
template class KBit<Solution<double, unsigned int>, double, unsigned int>;
template class KBit<Solution<unsigned int, unsigned int>, unsigned int, unsigned int>;
template class KBit<Solution<int, int>, int, int>;
} // namespace stocos