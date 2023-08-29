#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_

#include <sstream>
#include <memory>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/serialization.hpp>

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class DiffConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::DiffConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::DiffConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::DiffConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class DiffConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
        result = left_denot;
        result -= right_denot;
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_concept_left->evaluate(state, caches),
            *m_concept_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto concept_left_denotations = m_concept_left->evaluate(states, caches);
        auto concept_right_denotations = m_concept_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*concept_left_denotations)[i],
                *(*concept_right_denotations)[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, DiffConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const DiffConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, DiffConcept* concept, const unsigned int version);

protected:
    const std::shared_ptr<const Concept> m_concept_left;
    const std::shared_ptr<const Concept> m_concept_right;

public:
    DiffConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
    : Concept(vocabulary_info, index, concept_1->is_static() && concept_2->is_static()), m_concept_left(concept_1), m_concept_right(concept_2) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("DiffConcept::DiffConcept - at least one child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept_left->evaluate(state),
            m_concept_right->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept_left->compute_repr(out);
        out << ",";
        m_concept_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
    }

    static std::string get_name() {
        return "c_diff";
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::DiffConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::DiffConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::DiffConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::DiffConcept(vocabulary, index, concept_left, concept_right);
}

}

#endif
