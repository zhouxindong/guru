#pragma once

// what you need is the Type Erasure exactly!!
// dyno demo!!

// FunctionPtr 
// just like std::function<>

// primary template:
template <typename Signature>
class FunctionPtr;

// partial specialization
