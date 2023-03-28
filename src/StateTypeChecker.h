#ifndef STELLA_STATETYPECHECKER_H
#define STELLA_STATETYPECHECKER_H

#include <map>
#include "Stella/Absyn.H"

namespace Stella {
    class StateTypeChecker {
    private:
        std::vector<std::pair<StellaIdent, Type *>> stackDecl = {};
        std::vector<size_t> stackDeclScopes = {};

        std::vector<std::pair<StellaIdent, Type *>> stackType = {};
        std::vector<size_t> stackTypeScopes = {};
    public:
        Type *findIdentDecl(const StellaIdent &ident) {
            for (size_t i = stackType.size(); i > 0; --i) {
                if (stackType[i - 1].first == ident) {
                    return stackType[i - 1].second;
                }
            }
            return nullptr;
        }

        void insertIdentDecl(const StellaIdent &ident, Type *type) {
            stackType.emplace_back(ident, type);
        }

        Type *findIdentType(const StellaIdent &ident) {
            for (size_t i = stackType.size(); i > 0; --i) {
                if (stackType[i - 1].first == ident) {
                    return stackType[i - 1].second;
                }
            }
            return nullptr;
        }

        void insertIdentType(const StellaIdent &ident, Type *type) {
            stackType.emplace_back(ident, type);
        }

        void appendScope() {
            stackDeclScopes.push_back(stackDecl.size());
            stackTypeScopes.push_back(stackType.size());
        }

        void removeScope() {
            size_t size_decl = stackDeclScopes.back();
            stackDeclScopes.pop_back();
            stackDecl.resize((int) size_decl);

            size_t size_type = stackTypeScopes.back();
            stackTypeScopes.pop_back();
            stackType.resize((int) size_type);
        }
    };
} // namespace Stella

#endif //STELLA_STATETYPECHECKER_H
