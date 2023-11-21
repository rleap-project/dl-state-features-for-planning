#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class IdentityRole : public Role {
private:
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const ConceptDenotation& denot, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    IdentityRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override;

    size_t hash() const;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::IdentityRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::IdentityRole>& left_role,
            const std::shared_ptr<const dlplan::core::IdentityRole>& right_role) const;
    };

    template<>
    struct hash<dlplan::core::IdentityRole>
    {
        std::size_t operator()(const dlplan::core::IdentityRole& role) const;
    };
}

#endif
