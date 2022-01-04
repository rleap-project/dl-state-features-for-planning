#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Rule {
public:
    NullaryBoolean() : Rule("b_nullary") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        const std::vector<core::Predicate>& predicates = data.m_factory->get_vocabulary_info()->get_predicates();
        for (const auto& predicate : predicates) {
            if (predicate.get_arity() == 0) {
                th.submit([&](){
                    auto result = data.m_factory->make_nullary_boolean(predicate);
                    add_boolean(*this, iteration, std::move(result), states, data);
                });
            }
        }
    }
};

}

#endif
