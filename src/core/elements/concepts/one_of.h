#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"


#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class OneOfConcept : public Concept {
private:
    const Constant m_constant;

    void compute_result(const State& state, ConceptDenotation& result) const {
        bool found = false;
        for (const auto& object : state.get_instance_info()->get_objects()) {
            if (object.get_name() == m_constant.get_name()) {
                result.insert(object.get_index());
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name() + ")");
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            state,
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                states[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    OneOfConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Constant& constant)
        : Concept(vocabulary_info, index, true), m_constant(constant) { }

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const OneOfConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_constant == other_derived.m_constant;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_constant);
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(state, result);
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_one_of" << "(" << m_constant.get_name() << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::OneOfConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::OneOfConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::OneOfConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::OneOfConcept>
    {
        std::size_t operator()(const dlplan::core::OneOfConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
