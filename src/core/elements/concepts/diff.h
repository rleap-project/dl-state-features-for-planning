#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_DIFF_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class DiffConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::DiffConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::DiffConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::DiffConcept* t, const unsigned int version);
}


namespace dlplan::core {
class DiffConcept : public Concept {
private:
    const std::shared_ptr<const Concept> m_concept_left;
    const std::shared_ptr<const Concept> m_concept_right;

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

    DiffConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
        : Concept(vocabulary_info, index, concept_1->is_static() && concept_2->is_static()), m_concept_left(concept_1), m_concept_right(concept_2) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, DiffConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const DiffConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, DiffConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const DiffConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_concept_left == other_derived.m_concept_left
                && m_concept_right == other_derived.m_concept_right;
        }
        return false;
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
        out << "c_diff" << "(";
        m_concept_left->compute_repr(out);
        out << ",";
        m_concept_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
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
    ::new(t)dlplan::core::DiffConcept(index, vocabulary, concept_left, concept_right);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::DiffConcept, "dlplan::core::DiffConcept")

#endif
