#pragma once

// Used as a "tag" to show that the type derived from this is a system.
// The ctor is protected in order to prevent instaniation.
struct ISystem {
protected:
	ISystem() {}
};
