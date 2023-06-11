/// @brief Provides functionality for width-based planning and learning.

#ifndef DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_
#define DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_

#include "core.h"
#include "state_space.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace dlplan::novelty
{
class TupleNode;

using AtomIndex = int;
using AtomIndices = std::vector<AtomIndex>;

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;

using TupleNodeIndex = int;
using TupleNodeIndices = std::vector<TupleNodeIndex>;
using TupleNodes = std::vector<TupleNode>;

/// @brief Implements a perfect hash function between atom tuples and atom indices.
class NoveltyBase
{
private:
    std::vector<int> m_factors;
    int m_num_atoms;
    int m_arity;

public:
    NoveltyBase(int num_atoms, int arity);
    NoveltyBase(const NoveltyBase &other);
    NoveltyBase &operator=(const NoveltyBase &other);
    NoveltyBase(NoveltyBase &&other);
    NoveltyBase &operator=(NoveltyBase &&other);
    ~NoveltyBase();

    /// @brief Converts a atom tuples of size at most arity to a tuple index.
    ///        The user must sort atoms if atom tuples are viewed as sets.
    ///        This function is a perfect hash function.
    /// @param tuple_atom_indices A vector of atom indices of size at most arity
    /// @return A tuple index that identifies the input atom indices.
    TupleIndex atom_tuple_to_tuple_index(const AtomIndices &tuple_atom_indices) const;

    /// @brief Converts an atom index to a tuple of atoms. The resulting atom indices
    ///        are in the same order as they were used when computing the tuple index.
    /// @param tuple_index
    /// @return
    AtomIndices tuple_index_to_atom_tuple(TupleIndex tuple_index) const;

    int get_num_atoms() const;
    int get_arity() const;
};


/// @brief Implements a novelty table for the manipulation and querying of the
///        novelty status of a tuple with its representing index.
class NoveltyTable
{
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::vector<bool> m_table;

public:
    NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base);
    NoveltyTable(const NoveltyTable &other);
    NoveltyTable &operator=(const NoveltyTable &other);
    NoveltyTable(NoveltyTable &&other);
    NoveltyTable &operator=(NoveltyTable &&other);
    ~NoveltyTable();

    void reset_novelty(const TupleIndices &tuple_indices);

    /// @brief Computes all novel tuple indices of size specified by the arity
    ///        in the novelty base that can be obtained from the given atom indices.
    /// @param atom_indices A vector of atom indices. The user must take care
    ///                     that the atom indices are within correct bound.
    ///                     This is the case when the indices do not exceed the
    ///                     num_atoms parameter provided to the novelty base.
    /// @param add_atom_indices Atoms that were added by operator effects.
    ///                         Must be disjoint with atom_indices.
    /// @return Vector of novel tuple indices obtained from the given atom indices.
    TupleIndices compute_novel_tuple_indices(
        const AtomIndices &atom_indices,
        const AtomIndices &add_atom_indices) const;

    TupleIndices compute_novel_tuple_indices(
        const AtomIndices &atom_indices) const;

    /// @brief Marks all tuples of size specified by the arity in the novelty base
    ///        that can be obtained from the given atom indices as not novel anymore.
    /// @param atom_indices A vector of atom indices. The user must take care
    ///                     that the atom indices are within correct bound.
    ///                     This is the case when the indices do not exceed the
    ///                     num_atoms parameter provided to the novelty base.
    /// @param add_atom_indices Atoms that were added by operator effects.
    ///                         Must be distjoin with atom_indices.
    /// @param stop_if_novel Stop the iteration early if a tuple index was novel.
    /// @return True if at least one given tuple index was novel.
    bool insert_atom_indices(
        const AtomIndices &atom_indices,
        const AtomIndices &add_atom_indices,
        bool stop_if_novel = false);

    bool insert_atom_indices(
        const AtomIndices &atom_indices,
        bool stop_if_novel = false);

    /// @brief Iterates over all given tuple indices and marks them internally
    ///        as being not novel anymore.
    /// @param tuple_indices A vector of tuple indices. The user must take care
    ///                      that the indices are within correct bound. This is
    ///                      the case when the tuples were obtained by calling
    ///                      compute_novel_tuples.
    /// @param stop_if_novel Stop the iteration early if a tuple index was novel.
    /// @return True if at least one given tuple index was novel.
    bool insert_tuple_indices(const TupleIndices &tuple_indices, bool stop_if_novel = false);
};


/// @brief Encapsulates data related to a node in a tuple graph and provides
///        functionality to access it.
class TupleNode
{
private:
    TupleNodeIndex m_index;
    TupleIndex m_tuple_index;
    state_space::StateIndices m_state_indices;
    TupleIndices m_predecessors;
    TupleIndices m_successors;

    TupleNode(TupleNodeIndex index, TupleIndex tuple_index, const state_space::StateIndices &state_indices);
    TupleNode(TupleNodeIndex index, TupleIndex tuple_index, state_space::StateIndices &&state_indices);

    void add_predecessor(TupleIndex tuple_index);
    void add_successor(TupleIndex tuple_index);

    friend class TupleGraphBuilder;
    friend class TupleGraph;

public:
    TupleNode(const TupleNode &other);
    TupleNode &operator=(const TupleNode &other);
    TupleNode(TupleNode &&other);
    TupleNode &operator=(TupleNode &&other);
    ~TupleNode();

    /// @brief Compute the canonical string representation of this tuple node.
    /// @return The canonical string representation of this tuple node.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the TupleNode class.
    ///        Outputs a string representation of a TupleNode object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The TupleNode to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream &operator<<(std::ostream &os, const TupleNode &tuple_node);

    /// @brief Compute a string representation of this tuple node.
    /// @return A string representation of this tuple node.
    std::string str() const;

    TupleNodeIndex get_index() const;
    TupleIndex get_tuple_index() const;
    const state_space::StateIndices &get_state_indices() const;
    const TupleIndices &get_predecessors() const;
    const TupleIndices &get_successors() const;
};


/// @brief Implements a tuple graph and provides functionality for the
///        construction and for accessing the data.
class TupleGraph
{
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::shared_ptr<const state_space::StateSpace> m_state_space;
    state_space::StateIndex m_root_state_index;

    TupleNodes m_nodes;
    std::vector<TupleNodeIndices> m_node_indices_by_distance;
    std::vector<state_space::StateIndices> m_state_indices_by_distance;

public:
    TupleGraph(
        std::shared_ptr<const NoveltyBase> novelty_base,
        std::shared_ptr<const state_space::StateSpace> state_space,
        state_space::StateIndex root_state_index);
    TupleGraph(const TupleGraph &other);
    TupleGraph &operator=(const TupleGraph &other);
    TupleGraph(TupleGraph &&other);
    TupleGraph &operator=(TupleGraph &&other);
    ~TupleGraph();

    /// @brief Compute the canonical string representation of this tuple graph.
    /// @return The canonical string representation of this tuple graph.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the TupleGraph class.
    ///        Outputs a string representation of a TupleGraph object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The TupleGraph to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    friend std::ostream &operator<<(std::ostream &os, const TupleGraph &tuple_graph);

    /// @brief Compute a string representation of this tuple graph.
    /// @return A string representation of this tuple graph.
    std::string str() const;

    /// @brief Compute a string-dot representation of this tuple graph.
    /// @param verbosity_level
    /// @return A string-dot representation of this tuple graph.
    std::string to_dot(int verbosity_level) const;

    std::shared_ptr<const NoveltyBase> get_novelty_base() const;
    std::shared_ptr<const state_space::StateSpace> get_state_space() const;
    state_space::StateIndex get_root_state_index() const;
    const TupleNodes& get_tuple_nodes() const;
    const std::vector<TupleNodeIndices>& get_tuple_node_indices_by_distance() const;
    const std::vector<state_space::StateIndices>& get_state_indices_by_distance() const;
};

}

#endif
