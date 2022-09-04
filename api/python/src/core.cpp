#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/core.h"

namespace py = pybind11;

using namespace dlplan;


void init_core(py::module_ &m) {
    py::class_<core::ConceptDenotation>(m, "ConceptDenotation")
        .def("to_sorted_vector", &core::ConceptDenotation::to_sorted_vector)
    ;

    py::class_<core::RoleDenotation>(m, "RoleDenotation")
        .def("to_sorted_vector", &core::RoleDenotation::to_sorted_vector)
    ;

    py::class_<core::Constant>(m, "Constant")
        .def("__eq__", &core::Constant::operator==)
        .def("__neq__", &core::Constant::operator!=)
        .def("get_index", &core::Constant::get_index)
        .def("get_name", &core::Constant::get_name)
        .def("__str__", &core::Constant::get_name)
    ;

    py::class_<core::Object>(m, "Object")
        .def("__eq__", &core::Object::operator==)
        .def("__neq__", &core::Object::operator!=)
        .def("get_index", &core::Object::get_index)
        .def("get_name", &core::Object::get_name)
        .def("__str__", &core::Object::get_name)
    ;

    py::class_<core::Predicate>(m, "Predicate")
        .def("__eq__", &core::Predicate::operator==)
        .def("__neq__", &core::Predicate::operator!=)
        .def("get_name", &core::Predicate::get_name)
        .def("get_arity", &core::Predicate::get_arity)
        .def("get_index", &core::Predicate::get_index)
        .def("__str__", &core::Predicate::get_name)
    ;

    py::class_<core::Atom>(m, "Atom")
        .def("__eq__", &core::Atom::operator==)
        .def("__neq__", &core::Atom::operator!=)
        .def("get_name", &core::Atom::get_name)
        .def("get_index", &core::Atom::get_index)
        .def("get_predicate", &core::Atom::get_predicate)
        .def("get_objects", &core::Atom::get_objects)
        .def("get_object", &core::Atom::get_object)
        .def("get_is_static", &core::Atom::get_is_static)
        .def("__str__", &core::Atom::get_name)
    ;

    py::class_<core::State>(m, "State")
        .def(py::init<std::shared_ptr<const core::InstanceInfo>, const std::vector<core::Atom>&, int>(), py::arg("instance_info"), py::arg("atoms"), py::arg("index") = -1)
        .def("__eq__", &core::State::operator==)
        .def("__neq__", &core::State::operator!=)
        .def("__hash__", &core::State::compute_hash)
        .def("__str__", &core::State::str)
        .def("get_atom_idxs", &core::State::get_atom_idxs)
        .def("get_instance_info", &core::State::get_instance_info)
    ;

    py::class_<core::VocabularyInfo, std::shared_ptr<core::VocabularyInfo>>(m, "VocabularyInfo")
        .def(py::init<>())
        .def("add_predicate", &core::VocabularyInfo::add_predicate)
        .def("add_constant", &core::VocabularyInfo::add_constant)
        .def("exists_predicate", &core::VocabularyInfo::exists_predicate)
        .def("exists_predicate_name", &core::VocabularyInfo::exists_predicate_name)
        .def("get_predicates", &core::VocabularyInfo::get_predicates)
        .def("get_predicate_idx", &core::VocabularyInfo::get_predicate_idx)
        .def("get_predicate", &core::VocabularyInfo::get_predicate)
        .def("exists_constant", &core::VocabularyInfo::exists_constant)
        .def("exists_constant_name", &core::VocabularyInfo::exists_constant_name)
        .def("get_constants", &core::VocabularyInfo::get_constants)
        .def("get_constant_idx", &core::VocabularyInfo::get_constant_idx)
        .def("get_constant", &core::VocabularyInfo::get_constant)
    ;

    py::class_<core::InstanceInfo, std::shared_ptr<core::InstanceInfo>>(m, "InstanceInfo")
        .def(py::init<std::shared_ptr<core::VocabularyInfo>, int>(), py::arg("vocabulary_info"), py::arg("index") = -1)
        .def("add_object", &core::InstanceInfo::add_object)
        .def("add_atom", py::overload_cast<const core::Predicate&, const std::vector<core::Object>&>(&core::InstanceInfo::add_atom))
        .def("add_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&core::InstanceInfo::add_atom))
        .def("add_static_atom", py::overload_cast<const core::Predicate&, const std::vector<core::Object>&>(&core::InstanceInfo::add_static_atom))
        .def("add_static_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&core::InstanceInfo::add_static_atom))
        .def("get_atoms", &core::InstanceInfo::get_atoms)
        .def("get_static_atoms", &core::InstanceInfo::get_static_atoms)
        .def("get_atom", &core::InstanceInfo::get_atom)
        .def("get_atom_idx", &core::InstanceInfo::get_atom_idx)
        .def("get_objects", &core::InstanceInfo::get_objects)
        .def("get_object", &core::InstanceInfo::get_object)
        .def("get_object_idx", &core::InstanceInfo::get_object_idx)
        .def("get_num_objects", &core::InstanceInfo::get_num_objects)
    ;

    py::class_<core::Concept>(m, "Concept")
        .def("__repr__", &core::Concept::compute_repr)
        .def("evaluate", &core::Concept::evaluate)
        .def("compute_complexity", &core::Concept::compute_complexity)
        .def("compute_repr", &core::Concept::compute_repr)
        .def("set_index", &core::Concept::set_index)
    ;

    py::class_<core::Role>(m, "Role")
        .def("__repr__", &core::Role::compute_repr)
        .def("evaluate", &core::Role::evaluate)
        .def("compute_complexity", &core::Role::compute_complexity)
        .def("compute_repr", &core::Role::compute_repr)
        .def("set_index", &core::Role::set_index)
    ;

    py::class_<core::Numerical, std::shared_ptr<core::Numerical>>(m, "Numerical")
        .def("__repr__", &core::Numerical::compute_repr)
        .def("evaluate", &core::Numerical::evaluate)
        .def("compute_complexity", &core::Numerical::compute_complexity)
        .def("compute_repr", &core::Numerical::compute_repr)
        .def("set_index", &core::Numerical::set_index)
    ;

    py::class_<core::Boolean, std::shared_ptr<core::Boolean>>(m, "Boolean")
        .def("__repr__", &core::Boolean::compute_repr)
        .def("evaluate", &core::Boolean::evaluate)
        .def("compute_complexity", &core::Boolean::compute_complexity)
        .def("compute_repr", &core::Boolean::compute_repr)
        .def("set_index", &core::Boolean::set_index)
    ;

    py::class_<core::SyntacticElementFactory, std::shared_ptr<core::SyntacticElementFactory>>(m, "SyntacticElementFactory")
        .def(py::init<std::shared_ptr<const core::VocabularyInfo>>())

        .def("parse_concept", &core::SyntacticElementFactory::parse_concept, py::arg("description"), py::arg("index") = -1)
        .def("parse_role", &core::SyntacticElementFactory::parse_role, py::arg("description"), py::arg("index") = -1)
        .def("parse_numerical", &core::SyntacticElementFactory::parse_numerical, py::arg("description"), py::arg("index") = -1)
        .def("parse_boolean", &core::SyntacticElementFactory::parse_boolean, py::arg("description"), py::arg("index") = -1)

        .def("make_empty_boolean", py::overload_cast<const core::Concept&, int>(&core::SyntacticElementFactory::make_empty_boolean), py::arg("concept"), py::arg("index") = -1)
        .def("make_empty_boolean", py::overload_cast<const core::Role&, int>(&core::SyntacticElementFactory::make_empty_boolean), py::arg("role"), py::arg("index") = -1)
        .def("make_concept_inclusion_boolean", &core::SyntacticElementFactory::make_concept_inclusion_boolean, py::arg("concept_left"), py::arg("concept_right"), py::arg("index") = -1)
        .def("make_role_inclusion_boolean", &core::SyntacticElementFactory::make_role_inclusion_boolean, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_nullary_boolean", &core::SyntacticElementFactory::make_nullary_boolean, py::arg("predicate"), py::arg("index") = -1)

        .def("make_all_concept", &core::SyntacticElementFactory::make_all_concept, py::arg("role"), py::arg("concept"), py::arg("index") = -1)
        .def("make_and_concept", &core::SyntacticElementFactory::make_and_concept, py::arg("concept_left"), py::arg("concept_right"), py::arg("index") = -1)
        .def("make_bot_concept", &core::SyntacticElementFactory::make_bot_concept, py::arg("index") = -1)
        .def("make_diff_concept", &core::SyntacticElementFactory::make_diff_concept, py::arg("concept_left"), py::arg("concept_right"), py::arg("index") = -1)
        .def("make_equal_concept", &core::SyntacticElementFactory::make_equal_concept, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_not_concept", &core::SyntacticElementFactory::make_not_concept, py::arg("concept"), py::arg("index") = -1)
        .def("make_one_of_concept", &core::SyntacticElementFactory::make_one_of_concept, py::arg("constant"), py::arg("index") = -1)
        .def("make_or_concept", &core::SyntacticElementFactory::make_or_concept, py::arg("concept_left"), py::arg("concept_right"), py::arg("index") = -1)
        .def("make_projection_concept", &core::SyntacticElementFactory::make_projection_concept, py::arg("role"), py::arg("pos"), py::arg("index") = -1)
        .def("make_primitive_concept", &core::SyntacticElementFactory::make_primitive_concept, py::arg("predicate"), py::arg("pos"), py::arg("index") = -1)
        .def("make_some_concept", &core::SyntacticElementFactory::make_some_concept, py::arg("role"), py::arg("concept"), py::arg("index") = -1)
        .def("make_subset_concept", &core::SyntacticElementFactory::make_subset_concept, py::arg("concept_left"), py::arg("concept_right"), py::arg("index") = -1)
        .def("make_top_concept", &core::SyntacticElementFactory::make_top_concept, py::arg("index") = -1)

        .def("make_concept_distance", &core::SyntacticElementFactory::make_concept_distance, py::arg("concept_from"), py::arg("role"), py::arg("concept_to"), py::arg("index") = -1)
        .def("make_count", py::overload_cast<const core::Concept&, int>(&core::SyntacticElementFactory::make_count), py::arg("concept"), py::arg("index") = -1)
        .def("make_count", py::overload_cast<const core::Role&, int>(&core::SyntacticElementFactory::make_count), py::arg("role"), py::arg("index") = -1)
        .def("make_role_distance", &core::SyntacticElementFactory::make_role_distance, py::arg("role_from"), py::arg("role"), py::arg("role_to"), py::arg("index") = -1)
        .def("make_sum_concept_distance", &core::SyntacticElementFactory::make_sum_concept_distance, py::arg("concept_from"), py::arg("role"), py::arg("concept_to"), py::arg("index") = -1)
        .def("make_sum_role_distance", &core::SyntacticElementFactory::make_sum_role_distance, py::arg("role_from"), py::arg("role"), py::arg("role_to"), py::arg("index") = -1)

        .def("make_and_role", &core::SyntacticElementFactory::make_and_role, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_compose_role", &core::SyntacticElementFactory::make_compose_role, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_diff_role", &core::SyntacticElementFactory::make_diff_role, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_identity_role", &core::SyntacticElementFactory::make_identity_role, py::arg("concept"), py::arg("index") = -1)
        .def("make_inverse_role", &core::SyntacticElementFactory::make_inverse_role, py::arg("role"), py::arg("index") = -1)
        .def("make_not_role", &core::SyntacticElementFactory::make_not_role, py::arg("role"), py::arg("index") = -1)
        .def("make_or_role", &core::SyntacticElementFactory::make_or_role, py::arg("role_left"), py::arg("role_right"), py::arg("index") = -1)
        .def("make_primitive_role", &core::SyntacticElementFactory::make_primitive_role, py::arg("role"), py::arg("pos_1"), py::arg("pos_2"), py::arg("index") = -1)
        .def("make_restrict_role", &core::SyntacticElementFactory::make_restrict_role, py::arg("role"), py::arg("concept"), py::arg("index") = -1)
        .def("make_top_role", &core::SyntacticElementFactory::make_top_role, py::arg("index") = -1)
        .def("make_transitive_closure", &core::SyntacticElementFactory::make_transitive_closure, py::arg("role"), py::arg("index") = -1)
        .def("make_transitive_reflexive_closure", &core::SyntacticElementFactory::make_transitive_reflexive_closure, py::arg("role"), py::arg("index") = -1)
    ;
}
