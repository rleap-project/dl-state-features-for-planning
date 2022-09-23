#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../rule.h"
#include "../../../core/elements/concepts/bot.h"


namespace dlplan::generator::rules {

class BotConcept : public Rule {
public:
    BotConcept() : Rule() { }

    void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) override {
        assert(target_complexity == 1);
        core::SyntacticElementFactory& factory = data.m_factory;
        auto element = factory.make_bot_concept();
        auto denotations = element.get_element_ref().evaluate(states, caches);
        if (data.m_concept_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }

    std::string get_name() const override {
        return core::element::BotConcept::get_name();
    }
};

}

#endif
