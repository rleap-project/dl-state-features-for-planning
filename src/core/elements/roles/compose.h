#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_

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
class ComposeRole : public Role {
private:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
        // Compute sparse representation.
        PairsOfObjectIndices left_pairs = left_denot.to_vector();
        PairsOfObjectIndices right_pairs = right_denot.to_vector();
        for (const auto& left_pair : left_pairs) {  // source
            for (const auto& right_pair : right_pairs) {  // target
                if (left_pair.second == right_pair.first) {
                    result.insert(std::make_pair(left_pair.first, right_pair.second));
                }
            }
        }
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role_left->evaluate(state, caches),
            *m_role_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                denotation);
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    ComposeRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
        : Role(vocabulary_info, index, role_left->is_static() && role_right->is_static()), m_role_left(role_left), m_role_right(role_right)  { }

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const ComposeRole&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role_left == other_derived.m_role_left
                && m_role_right == other_derived.m_role_right;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role_left, m_role_right);
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_compose" << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::ComposeRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::ComposeRole>& left_role,
            const std::shared_ptr<const dlplan::core::ComposeRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::ComposeRole>
    {
        std::size_t operator()(const dlplan::core::ComposeRole& role) const {
            return role.hash();
        }
    };
}

#endif
