#ifndef STELLA_STATETYPECHECKER_H
#define STELLA_STATETYPECHECKER_H

#include <map>
#include "Stella/Absyn.H"

namespace Stella {
    class StateTypeChecker {
    private:
        // Two structs for saving decls and their names
        std::vector<std::pair<StellaIdent, Type *>> stackDecl = {};
        std::vector<size_t> stackDeclScopes = {}; // Stack of sizes for entering and exiting from scopes

        // Two structs for saving types and their names
        std::vector<std::pair<StellaIdent, Type *>> stackType = {};
        std::vector<size_t> stackTypeScopes = {}; // Stack of sizes for entering and exiting from scopes

        // Two structs for saving exception types and their names
        std::vector<std::pair<StellaIdent, Type *>> stackExceptionIdent = {};
        std::vector<Type *> stackException = {};
        std::vector<std::pair<size_t, size_t>> stackExceptionScopes = {}; // Stack of sizes for entering and exiting from scopes
    public:
        // Function to find Declared expr type by identifier
        Type *findIdentDecl(const StellaIdent &ident) {
            for (size_t i = stackType.size(); i > 0; --i) {
                if (stackType[i - 1].first == ident) {
                    return stackType[i - 1].second;
                }
            }
            fprintf(stderr, "error: undefined variable\n");
            exit(1);
        }

        // Function to insert Declared expr type by identifier
        void insertIdentDecl(const StellaIdent &ident, Type *type) {
            stackType.emplace_back(ident, type);
        }

        // Function to find type by identifier
        Type *findIdentType(const StellaIdent &ident) {
            for (size_t i = stackType.size(); i > 0; --i) {
                if (stackType[i - 1].first == ident) {
                    return stackType[i - 1].second;
                }
            }
            fprintf(stderr, "error: undefined variable\n");
            exit(1);
        }

        // Function to insert type by identifier
        void insertIdentType(const StellaIdent &ident, Type *type) {
            stackType.emplace_back(ident, type);
        }

        // Function to find Exception type by identifier
        Type *findIdentException(const StellaIdent &ident) {
            for (size_t i = stackExceptionIdent.size(); i > 0; --i) {
                if (stackExceptionIdent[i - 1].first == ident) {
                    return stackExceptionIdent[i - 1].second;
                }
            }
            fprintf(stderr, "error: undefined variable\n");
            exit(1);
        }
        void findException(Type *type) {
            for (size_t i = stackException.size(); i > 0; --i) {
                if (type->cmp(stackException[i - 1])) {
                    return;
                }
            }
            fprintf(stderr, "error: undefined variable\n");
            exit(1);
        }

        // Function to insert Exception type by identifier
        void insertIdentException(const StellaIdent &ident, Type *type) {
            stackExceptionIdent.emplace_back(ident, type);
        }

        // Function to insert Exception type
        void insertException(Type *type) {
            stackException.push_back(type);
        }

        // Function to save in stacks size of two vectors
        void appendScope() {
            stackDeclScopes.push_back(stackDecl.size());
            stackTypeScopes.push_back(stackType.size());
            stackExceptionScopes.emplace_back(stackException.size(), stackExceptionIdent.size());
        }

        // Function to exit from scope and remove all declarations inside scopes
        void removeScope() {
            size_t size_decl = stackDeclScopes.back();
            stackDeclScopes.pop_back();
            stackDecl.resize((int) size_decl);

            size_t size_type = stackTypeScopes.back();
            stackTypeScopes.pop_back();
            stackType.resize((int) size_type);

            size_t size_exception = stackExceptionScopes.back().first;
            stackException.resize((int) size_exception);
            size_exception = stackExceptionScopes.back().second;
            stackExceptionIdent.resize((int) size_exception);
            stackExceptionScopes.pop_back();
        }
    };
} // namespace Stella

#endif //STELLA_STATETYPECHECKER_H
