#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/novelty.h"

namespace py = pybind11;

using namespace dlplan::novelty;
using namespace dlplan::state_space;


void init_novelty(py::module_ &m_novelty) {
    py::class_<NoveltyBase, std::shared_ptr<NoveltyBase>>(m_novelty, "NoveltyBase")
        .def(py::init<int, int>())
        .def("atom_tuple_to_tuple_index", &NoveltyBase::atom_tuple_to_tuple_index)
        .def("tuple_index_to_atom_tuple", &NoveltyBase::tuple_index_to_atom_tuple)
        .def("get_num_atoms", &NoveltyBase::get_num_atoms)
        .def("get_arity", &NoveltyBase::get_arity)
    ;

    py::class_<NoveltyTable>(m_novelty, "NoveltyTable")
        .def(py::init<std::shared_ptr<const NoveltyBase>>())
        .def("reset_novelty", &NoveltyTable::reset_novelty)
        .def("compute_novel_tuple_indices", &NoveltyTable::compute_novel_tuple_indices)
        .def("insert", py::overload_cast<const AtomIndices&, const AtomIndices&, bool>(&NoveltyTable::insert), py::arg("atom_indices"), py::arg("add_atom_indices"), py::arg("stop_if_novel") = true)
        .def("insert", py::overload_cast<const TupleIndices&, bool>(&NoveltyTable::insert), py::arg("tuple_indices"), py::arg("stop_if_novel") = true)
    ;

    py::class_<TupleNode>(m_novelty, "TupleNode")
        .def(py::init<TupleNodeIndex, TupleIndex, const StateIndices&>())
        .def("__repr__", &TupleNode::compute_repr)
        .def("__str__", &TupleNode::str)
        .def("get_index", &TupleNode::get_index)
        .def("get_tuple_index", &TupleNode::get_tuple_index)
        .def("get_state_indices", &TupleNode::get_state_indices, py::return_value_policy::reference)
        .def("get_predecessors", &TupleNode::get_predecessors, py::return_value_policy::reference)
        .def("get_successors", &TupleNode::get_successors, py::return_value_policy::reference)
    ;

    py::class_<TupleGraph>(m_novelty, "TupleGraph")
        .def(py::init<std::shared_ptr<const NoveltyBase>, std::shared_ptr<const StateSpace>, StateIndex>())
        .def("__repr__", &TupleGraph::compute_repr)
        .def("__str__", &TupleGraph::str)
        .def("to_dot", &TupleGraph::to_dot)
        .def("get_novelty_base", &TupleGraph::get_novelty_base)
        .def("get_state_space", &TupleGraph::get_state_space)
        .def("get_root_state_index", &TupleGraph::get_root_state_index)
        .def("get_tuple_nodes", &TupleGraph::get_tuple_nodes, py::return_value_policy::reference)
        .def("get_tuple_node_indices_by_distance", &TupleGraph::get_tuple_node_indices_by_distance, py::return_value_policy::reference)
        .def("get_state_indices_by_distance", &TupleGraph::get_state_indices_by_distance, py::return_value_policy::reference)
    ;
}
