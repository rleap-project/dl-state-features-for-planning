#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include <sstream>
#include <memory>

#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
template<typename T>
class CountNumerical;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::CountNumerical<T>& numerical, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::CountNumerical<T>* numerical, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::CountNumerical<T>* numerical, const unsigned int version);
}

namespace dlplan::core {

template<typename T>
class CountNumerical : public Numerical {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot, int& result) const {
        result = denot.size();
    }

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        int denotation;
        compute_result(
            *m_element->evaluate(state, caches),
            denotation);
        return denotation;
    }

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        NumericalDenotations denotations;
        denotations.reserve(states.size());
        auto element_denotations = m_element->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            int denotation;
            compute_result(
                *(*element_denotations)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, CountNumerical<T_>& numerical, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive& ar, const CountNumerical<T_>* numerical, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive& ar, CountNumerical<T_>* numerical, const unsigned int version);

protected:
    const std::shared_ptr<const T> m_element;

public:
    CountNumerical(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const T> element)
    : Numerical(vocabulary_info, index, element->is_static()), m_element(element) { }

    int evaluate(const State& state) const override {
        int result;
        compute_result(
            m_element->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_element->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        int score = m_element->compute_evaluate_time_score();
        if (std::is_same<T, Concept>::value) {
            score += SCORE_LINEAR;
        } else if (std::is_same<T, Role>::value) {
            score += SCORE_QUADRATIC;
        } else {
            throw std::runtime_error("Inclusion::compute_evaluate_time_score - unknown template parameter.");
        }
        return score;
    }

    static std::string get_name() {
        return "n_count";
    }
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::CountNumerical<dlplan::core::Concept>, "dlplan::core::CountNumerical<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_KEY2(dlplan::core::CountNumerical<dlplan::core::Role>, "dlplan::core::CountNumerical<dlplan::core::Role>")

#endif