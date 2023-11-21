#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ALL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ALL_H_

#include "../../../../../src/core/elements/utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class AllConcept : public Concept {
private:
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    AllConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override;

    size_t hash() const;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::AllConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::AllConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::AllConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::AllConcept>
    {
        std::size_t operator()(const dlplan::core::AllConcept& concept_) const;
    };
}

#endif
