#pragma once

#include "Utils.h"
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace shroud {

// An opaque predicate evaluator: takes input(s) and returns bool.
// The result is known at obfuscation time but hard to determine statically.
enum class PredicateKind {
    AlwaysTrue,
    AlwaysFalse,
    TwoWay // P^? - either branch is valid
};

enum class PredicateCategory {
    NumberTheoretic,
    ModularArithmetic,
    Bitwise,
    FloatingPoint,
    Pointer,
    Dynamic,
    GraphTheoretic,
    LatticeBased,
    MultivariateQuadratic,
    ExpanderGraph,
    CellularAutomaton,
    CryptographicHash,
    ErrorCorrectingCode,
    Undecidable,
    AlgebraicGeometry,
    RepresentationTheory,
    Combinatorial,
    PAdicArithmetic,
    InformationTheoretic,
    TopologyInspired,
    ContinuedFraction,
    AutomataTheory,
    HashingTheory,
    SignalProcessing,
    GameTheory,
    Thermodynamic,
    PolynomialRing,
    LFSRSequence
};

// Represents a single opaque predicate with its evaluation function
struct OpaquePredicate {
    std::string name;
    PredicateKind kind;
    PredicateCategory category;
    int numInputs; // 1 or 2

    // Evaluate the predicate for given inputs (for testing)
    std::function<bool(uint32_t x, uint32_t y)> evaluate;

    // Human-readable description
    std::string description;
};

// ============================================================================
// Predicate generators - each returns a collection of predicates
// ============================================================================

// 3a: Number-theoretic invariant predicates
std::vector<OpaquePredicate> createNumberTheoreticPredicates();

// 3b: Modular arithmetic predicates
std::vector<OpaquePredicate> createModularArithmeticPredicates(ObfRNG& rng);

// 3c: Bitwise & machine-level predicates
std::vector<OpaquePredicate> createBitwisePredicates();

// 3d: Floating-point predicates
std::vector<OpaquePredicate> createFloatingPointPredicates();

// 3e: Pointer/memory-based predicates (Collberg-style)
std::vector<OpaquePredicate> createPointerPredicates();

// 3f: Dynamic opaque predicates (unique per compilation)
std::vector<OpaquePredicate> createDynamicPredicates(ObfRNG& rng, int count = 10);

// 3g: Graph-theoretic predicates
std::vector<OpaquePredicate> createGraphTheoreticPredicates(ObfRNG& rng);

// 3h: Lattice-based predicates (post-quantum)
std::vector<OpaquePredicate> createLatticeBasedPredicates(ObfRNG& rng);

// 3i: Multivariate quadratic predicates (post-quantum)
std::vector<OpaquePredicate> createMQPredicates(ObfRNG& rng);

// 3j: Expander graph predicates
std::vector<OpaquePredicate> createExpanderGraphPredicates(ObfRNG& rng);

// 3k: Cellular automaton / chaotic map predicates
std::vector<OpaquePredicate> createCellularAutomatonPredicates(ObfRNG& rng);

// 3l: Cryptographic hash predicates
std::vector<OpaquePredicate> createCryptoHashPredicates(ObfRNG& rng);

// 3m: Error-correcting code predicates (post-quantum)
std::vector<OpaquePredicate> createECCPredicates(ObfRNG& rng);

// 3n: Undecidable/intractable predicates
std::vector<OpaquePredicate> createUndecidablePredicates(ObfRNG& rng);

// 3p: Novel predicates from diverse mathematical/CS domains
std::vector<OpaquePredicate> createNovelPredicates(ObfRNG& rng);

// 3o: Composite predicates (AND/OR/XOR of simpler ones)
OpaquePredicate composePredicates(const OpaquePredicate& a, const OpaquePredicate& b,
                                   ObfRNG& rng);

// Master function: get all available predicates
std::vector<OpaquePredicate> getAllPredicates(ObfRNG& rng);

// Pick a random predicate of a given kind
const OpaquePredicate& pickPredicate(const std::vector<OpaquePredicate>& predicates,
                                      PredicateKind kind, ObfRNG& rng);

} // namespace shroud
