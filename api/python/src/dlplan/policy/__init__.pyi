from typing import List, Overload, Union, MutableSet, Tuple

from ..core import State, DenotationsCaches, Boolean, Numerical, SyntacticElementFactory


class NamedBaseElement:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def get_key(self) -> str: ...


class NamedBoolean(NamedBaseElement):
    def get_boolean(self) -> Boolean: ...


class NamedNumerical(NamedBaseElement):
    def get_numerical(self) -> Numerical: ...


class BaseCondition:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @overload
    def evaluate(self, state: State) -> bool: ...
    @overload
    def evaluate(self, state: State, caches: DenotationsCaches) -> bool: ...
    def get_index(self) -> int: ...
    def get_boolean(self) -> Union[None, Boolean]: ...
    def get_numerical(self) -> Union[None, Numerical]: ...


class BaseEffect:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @overload
    def evaluate(self, source_state: State, target_state: State) -> bool: ...
    @overload
    def evaluate(self, source_state: State, target_state: State, caches: DenotationsCaches) -> bool: ...
    def get_index(self) -> int: ...
    def get_boolean(self) -> Union[None, Boolean]: ...
    def get_numerical(self) -> Union[None, Numerical]: ...


class Rule:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @overload
    def evaluate_conditions(self, state: State) -> bool: ...
    @overload
    def evaluate_conditions(self, state: State, caches: DenotationsCaches) -> bool: ...
    @overload
    def evaluate_effects(self, source_state: State, target_state: State) -> bool: ...
    @overload
    def evaluate_effects(self, source_state: State, target_state: State, caches: DenotationsCaches) -> bool: ...
    def get_index(self) -> int: ...
    def get_conditions(self) -> MutableSet[BaseCondition]: ...
    def get_effects(self) -> MutableSet[BaseEffect]: ...


class Policy:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    @overload
    def evaluate(self, source_state: State, target_state: State) -> Union[None, Rule]: ...
    @overload
    def evaluate(self, source_state: State, target_state: State, caches: DenotationsCaches) -> Union[None, Rule]: ...
    @overload
    def evaluate_conditions(self, state: State) -> List[Rule]: ...
    @overload
    def evaluate_conditions(self, state: State, caches: DenotationsCaches) -> List[Rule]: ...
    @overload
    def evaluate_effects(self, source_state: State, target_state: State, rules: List[Rule]) -> bool: ...
    @overload
    def evaluate_effects(self, source_state: State, target_state: State, rules: List[Rule], caches: DenotationsCaches) -> bool: ...
    def get_rules(self) -> MutableSet[Rule]: ...
    def get_booleans(self) -> MutableSet[Boolean]: ...
    def get_numericals(self) -> MutableSet[Numerical]: ...


class PolicyFactory:
    def __init__(self, element_factory: SyntacticElementFactory) -> None: ...
    def make_boolean(self, key: str, boolean: Boolean) -> NamedBoolean: ...
    def make_numerical(self, key: str, numerical: Numerical) -> NamedNumerical: ...
    def make_pos_condition(self, boolean: NamedBoolean) -> BaseCondition: ...
    def make_neg_condition(self, boolean: NamedBoolean) -> BaseCondition: ...
    def make_gt_condition(self, numerical: NamedNumerical) -> BaseCondition: ...
    def make_eq_condition(self, numerical: NamedNumerical) -> BaseCondition: ...
    def make_pos_effect(self, boolean: NamedBoolean) -> BaseEffect: ...
    def make_neg_effect(self, boolean: NamedBoolean) -> BaseEffect: ...
    def make_inc_effect(self, bnumerical: NamedNumerical) -> BaseEffect: ...
    def make_dec_effect(self, numerical: NamedNumerical) -> BaseEffect: ...
    @overload
    def make_bot_effect(self, boolean: NamedBoolean) -> BaseEffect: ...
    @overload
    def make_bot_effect(self, numerical: NamedNumerical) -> BaseEffect: ...
    def make_rule(self, conditions: MutableSet[BaseCondition], effects: MutableSet[BaseEffect]) -> Rule: ...
    def make_policy(self, rules: MutableSet[Rule]) -> Policy: ...
    def parse_policy(self, description: str, filename: str = "") -> Policy: ...


class PolicyMinimizer:
    def __init__(self) -> None: ...
    @overload
    def minimize(self, policy: Policy, policy_factory: PolicyFactory) -> Policy: ...
    @overload
    def minimize(self, policy: Policy, true_state_pairs: List[Tuple[State, State]], false_state_pairs: List[Tuple[State, State]], policy_factory: PolicyFactory) -> Policy: ...
