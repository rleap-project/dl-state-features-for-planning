#include "../../include/dlplan/core.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


namespace dlplan::core {
Predicate::Predicate(const std::string& name, PredicateIndex index, int arity, bool is_static)
    : m_name(name), m_index(index), m_arity(arity), m_is_static(is_static) { }

Predicate::Predicate(const Predicate& other) = default;

Predicate& Predicate::operator=(const Predicate& other) = default;

Predicate::Predicate(Predicate&& other) = default;

Predicate& Predicate::operator=(Predicate&& other) = default;

Predicate::~Predicate() = default;

bool Predicate::operator==(const Predicate& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name()) && (get_arity() == other.get_arity()) && (is_static() == other.is_static());
}

bool Predicate::operator!=(const Predicate& other) const {
    return !(*this == other);
}

std::string Predicate::compute_repr() const {
    std::stringstream ss;
    ss << "Predicate("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "arity=" << m_arity << ", "
       << "is_static=" << m_is_static
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Predicate& predicate) {
    os << predicate.compute_repr();
    return os;
}

std::string Predicate::str() const {
    return compute_repr();
}

const std::string& Predicate::get_name() const {
    return m_name;
}

PredicateIndex Predicate::get_index() const {
    return m_index;
}

int Predicate::get_arity() const {
    return m_arity;
}

bool Predicate::is_static() const {
    return m_is_static;
}

}


namespace boost::serialization {

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::Predicate& /* t */, const unsigned int /* version */) {
}

template<class Archive>
void save_construct_data(
    Archive & ar, const dlplan::core::Predicate* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
    ar << t->m_arity;
    ar << t->m_is_static;
}

template<class Archive>
void load_construct_data(
    Archive & ar, dlplan::core::Predicate* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::PredicateIndex index;
    int arity;
    bool is_static;
    ar >> name;
    ar >> index;
    ar >> arity;
    ar >> is_static;
    ::new(t)dlplan::core::Predicate(name, index, arity, is_static);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::Predicate& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::Predicate& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::Predicate* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::Predicate* t, const unsigned int version);
}