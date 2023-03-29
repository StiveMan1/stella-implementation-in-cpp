#include "VisitTypeCheck.h"
#include <iostream>

namespace Stella {
    void VisitTypeCheck::visitProgram(Program *t) {}                   // abstract class
    void VisitTypeCheck::visitLanguageDecl(LanguageDecl *t) {}         // abstract class
    void VisitTypeCheck::visitDecl(Decl *t) {}                         // abstract class
    void VisitTypeCheck::visitLocalDecl(LocalDecl *t) {}               // abstract class
    void VisitTypeCheck::visitAnnotation(Annotation *t) {}             // abstract class
    void VisitTypeCheck::visitParamDecl(ParamDecl *t) {}               // abstract class
    void VisitTypeCheck::visitReturnType(ReturnType *t) {}             // abstract class
    void VisitTypeCheck::visitThrowType(ThrowType *t) {}               // abstract class
    void VisitTypeCheck::visitExpr(Expr *t) {}                         // abstract class
    void VisitTypeCheck::visitExprData(ExprData *t) {}                 // abstract class
    void VisitTypeCheck::visitType(Type *t) {}                         // abstract class
    void VisitTypeCheck::visitTyping(Typing *t) {}                     // abstract class

    void VisitTypeCheck::visitAProgram(AProgram *a_program) {
        /* Code For AProgram Goes Here */

        if (a_program->languagedecl_)
            a_program->languagedecl_->accept(this);
        if (a_program->listextension_)
            a_program->listextension_->accept(this);
        if (a_program->listdecl_)
            a_program->listdecl_->accept(this);
    }

    void VisitTypeCheck::visitLanguageCore(LanguageCore *language_core) {
        /* Code For LanguageCore Goes Here */
    }

    void VisitTypeCheck::visitDeclFun(DeclFun *decl_fun) {
        /* Code For DeclFun Goes Here */

        if (decl_fun->listannotation_)
            decl_fun->listannotation_->accept(this);
        visitStellaIdent(decl_fun->stellaident_);
        if (decl_fun->listparamdecl_)
            decl_fun->listparamdecl_->accept(this);
        ListType *type_list = _type_list;
        if (decl_fun->returntype_)
            decl_fun->returntype_->accept(this);
        Type *retType = _type;
        if (decl_fun->throwtype_)
            decl_fun->throwtype_->accept(this);

        _state->insertIdentDecl(decl_fun->stellaident_, new TypeFun(type_list, retType));

        _state->appendScope();
        if (decl_fun->listdecl_)
            decl_fun->listdecl_->accept(this);
        if (decl_fun->expr_)
            decl_fun->expr_->accept(this);
        if(!_type->cmp(retType)){
            fprintf(stderr, "error: %d,%d: semantic error\n", decl_fun->line_number, decl_fun->char_number);
            exit(1);
        }
        _state->removeScope();

        _type = new TypeFun(type_list, _type);
        _state->insertIdentDecl(decl_fun->stellaident_, _type);
    }

    void VisitTypeCheck::visitDeclTypeAlias(DeclTypeAlias *decl_type_alias) {
        /* Code For DeclTypeAlias Goes Here */

        visitStellaIdent(decl_type_alias->stellaident_);
        if (decl_type_alias->type_)
            decl_type_alias->type_->accept(this);

        _state->insertIdentType(decl_type_alias->stellaident_, _type);
    }

    void VisitTypeCheck::visitALocalDecl(ALocalDecl *a_local_decl) {
        /* Code For ALocalDecl Goes Here */

        if (a_local_decl->decl_)
            a_local_decl->decl_->accept(this);
    }

    void VisitTypeCheck::visitInlineAnnotation(InlineAnnotation *inline_annotation) {
        /* Code For InlineAnnotation Goes Here */
    }

    void VisitTypeCheck::visitAParamDecl(AParamDecl *a_param_decl) {
        /* Code For AParamDecl Goes Here */

        visitStellaIdent(a_param_decl->stellaident_);
        if (a_param_decl->type_)
            a_param_decl->type_->accept(this);
        _state->insertIdentDecl(a_param_decl->stellaident_, _type);
    }

    void VisitTypeCheck::visitNoReturnType(NoReturnType *no_return_type) {
        /* Code For NoReturnType Goes Here */
        _type = nullptr;
    }

    void VisitTypeCheck::visitSomeReturnType(SomeReturnType *some_return_type) {
        /* Code For SomeReturnType Goes Here */

        if (some_return_type->type_)
            some_return_type->type_->accept(this);
    }

    void VisitTypeCheck::visitNoThrowType(NoThrowType *no_throw_type) {
        /* Code For NoThrowType Goes Here */
    }

    void VisitTypeCheck::visitSomeThrowType(SomeThrowType *some_throw_type) {
        /* Code For SomeThrowType Goes Here */

        if (some_throw_type->listtype_)
            some_throw_type->listtype_->accept(this);
    }

    void VisitTypeCheck::visitIf(If *if_) {
        /* Code For If Goes Here */

        if (if_->expr_1)
            if_->expr_1->accept(this);
        if(!_type->cmp(new TypeBool())){
            fprintf(stderr, "error: %d,%d: semantic error\n", if_->line_number, if_->char_number);
            exit(1);
        }
        if (if_->expr_2)
            if_->expr_2->accept(this);
        Type *type1 = _type;
        if (if_->expr_3)
            if_->expr_3->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(type2)){
            fprintf(stderr, "error: %d,%d: semantic error\n", if_->line_number, if_->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitLet(Let *let) {
        /* Code For Let Goes Here */
        _state->appendScope();
        visitStellaIdent(let->stellaident_);
        if (let->expr_1)
            let->expr_1->accept(this);
        _state->insertIdentDecl(let->stellaident_, _type);
        if (let->expr_2)
            let->expr_2->accept(this);
        _state->removeScope();
    }

    void VisitTypeCheck::visitLessThan(LessThan *less_than) {
        /* Code For LessThan Goes Here */

        if (less_than->expr_1)
            less_than->expr_1->accept(this);
        Type *type1 = _type;
        if (less_than->expr_2)
            less_than->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", less_than->line_number, less_than->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitLessThanOrEqual(LessThanOrEqual *less_than_or_equal) {
        /* Code For LessThanOrEqual Goes Here */

        if (less_than_or_equal->expr_1)
            less_than_or_equal->expr_1->accept(this);
        Type *type1 = _type;
        if (less_than_or_equal->expr_2)
            less_than_or_equal->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", less_than_or_equal->line_number, less_than_or_equal->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitGreaterThan(GreaterThan *greater_than) {
        /* Code For GreaterThan Goes Here */

        if (greater_than->expr_1)
            greater_than->expr_1->accept(this);
        Type *type1 = _type;
        if (greater_than->expr_2)
            greater_than->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", greater_than->line_number, greater_than->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitGreaterThanOrEqual(GreaterThanOrEqual *greater_than_or_equal) {
        /* Code For GreaterThanOrEqual Goes Here */

        if (greater_than_or_equal->expr_1)
            greater_than_or_equal->expr_1->accept(this);
        Type *type1 = _type;
        if (greater_than_or_equal->expr_2)
            greater_than_or_equal->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", greater_than_or_equal->line_number, greater_than_or_equal->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitEqual(Equal *equal) {
        /* Code For Equal Goes Here */

        if (equal->expr_1)
            equal->expr_1->accept(this);
        Type *type1 = _type;
        if (equal->expr_2)
            equal->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", equal->line_number, equal->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitNotEqual(NotEqual *not_equal) {
        /* Code For NotEqual Goes Here */

        if (not_equal->expr_1)
            not_equal->expr_1->accept(this);
        Type *type1 = _type;
        if (not_equal->expr_2)
            not_equal->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", not_equal->line_number, not_equal->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitTypeAsc(TypeAsc *type_asc) {
        /* Code For TypeAsc Goes Here */

        if (type_asc->expr_)
            type_asc->expr_->accept(this);
        Type *type1 = _type;
        if (type_asc->type_)
            type_asc->type_->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(type2)){
            fprintf(stderr, "error: %d,%d: semantic error\n", type_asc->line_number, type_asc->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitAbstraction(Abstraction *abstraction) {
        /* Code For Abstraction Goes Here */
        _state->appendScope();
        if (abstraction->listparamdecl_)
            abstraction->listparamdecl_->accept(this);
        ListType *type_list = _type_list;
        if (abstraction->expr_)
            abstraction->expr_->accept(this);
        _state->removeScope();
        _type = new TypeFun(type_list, _type);
    }

    void VisitTypeCheck::visitTuple(Tuple *tuple) {
        /* Code For Tuple Goes Here */

        if (tuple->listexpr_)
            tuple->listexpr_->accept(this);
        _type = new TypeTuple(_type_list);
    }

    void VisitTypeCheck::visitList(List *list) {
        /* Code For List Goes Here */

        if (list->listexpr_)
            list->listexpr_->accept(this);
        _type = nullptr;
        for (auto & i : *_type_list) {
            if(!i->cmp(_type)){
                fprintf(stderr, "error: %d,%d: semantic error\n", list->line_number, list->char_number);
                exit(1);
            }
            _type = i;
        }
        _type = new TypeList(_type);
    }

    void VisitTypeCheck::visitAdd(Add *add) {
        /* Code For Add Goes Here */

        if (add->expr_1)
            add->expr_1->accept(this);
        Type *type1 = _type;
        if (add->expr_2)
            add->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", add->line_number, add->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitLogicOr(LogicOr *logic_or) {
        /* Code For LogicOr Goes Here */

        if (logic_or->expr_1)
            logic_or->expr_1->accept(this);
        Type *type1 = _type;
        if (logic_or->expr_2)
            logic_or->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeBool()) || !type2->cmp(new TypeBool())){
            fprintf(stderr, "error: %d,%d: semantic error\n", logic_or->line_number, logic_or->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitMultiply(Multiply *multiply) {
        /* Code For Multiply Goes Here */

        if (multiply->expr_1)
            multiply->expr_1->accept(this);
        Type *type1 = _type;
        if (multiply->expr_2)
            multiply->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeNat()) || !type2->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", multiply->line_number, multiply->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitLogicAnd(LogicAnd *logic_and) {
        /* Code For LogicAnd Goes Here */

        if (logic_and->expr_1)
            logic_and->expr_1->accept(this);
        Type *type1 = _type;
        if (logic_and->expr_2)
            logic_and->expr_2->accept(this);
        Type *type2 = _type;
        if(!type1->cmp(new TypeBool()) || !type2->cmp(new TypeBool())){
            fprintf(stderr, "error: %d,%d: semantic error\n", logic_and->line_number, logic_and->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitApplication(Application *application) {
        /* Code For Application Goes Here */

        if (application->expr_)
            application->expr_->accept(this);
        if(!_type->cmp(new TypeFun(nullptr, nullptr))){
            fprintf(stderr, "error: %d,%d: semantic error\n", application->line_number, application->char_number);
            exit(1);
        }
        auto *func = dynamic_cast<TypeFun*>(_type);
        if (application->listexpr_)
            application->listexpr_->accept(this);

        if(!func->listtype_->cmp(_type_list)){
            fprintf(stderr, "error: %d,%d: semantic error\n", application->line_number, application->char_number);
            exit(1);
        }
        _type = func->type_;
    }

    void VisitTypeCheck::visitConsList(ConsList *cons_list) {
        /* Code For ConsList Goes Here */

        if (cons_list->expr_1)
            cons_list->expr_1->accept(this);
        if (cons_list->expr_2)
            cons_list->expr_2->accept(this);
    }

    void VisitTypeCheck::visitHead(Head *head) {
        /* Code For Head Goes Here */

        if (head->expr_)
            head->expr_->accept(this);

        if(!_type->cmp(new TypeList(nullptr))){
            fprintf(stderr, "error: %d,%d: semantic error\n", head->line_number, head->char_number);
            exit(1);
        }
        _type = dynamic_cast<TypeList*>(_type)->type_;
    }

    void VisitTypeCheck::visitIsEmpty(IsEmpty *is_empty) {
        /* Code For IsEmpty Goes Here */

        if (is_empty->expr_)
            is_empty->expr_->accept(this);

        if(!_type->cmp(new TypeList(nullptr))){
            fprintf(stderr, "error: %d,%d: semantic error\n", is_empty->line_number, is_empty->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitTail(Tail *tail) {
        /* Code For Tail Goes Here */

        if (tail->expr_)
            tail->expr_->accept(this);

        if(!_type->cmp(new TypeList(nullptr))){
            fprintf(stderr, "error: %d,%d: semantic error\n", tail->line_number, tail->char_number);
            exit(1);
        }
        _type = dynamic_cast<TypeList*>(_type)->type_;
    }

    void VisitTypeCheck::visitSucc(Succ *succ) {
        /* Code For Succ Goes Here */

        if (succ->expr_)
            succ->expr_->accept(this);

        if(!_type->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", succ->line_number, succ->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitLogicNot(LogicNot *logic_not) {
        /* Code For LogicNot Goes Here */

        if (logic_not->expr_)
            logic_not->expr_->accept(this);

        if(!_type->cmp(new TypeBool())){
            fprintf(stderr, "error: %d,%d: semantic error\n", logic_not->line_number, logic_not->char_number);
            exit(1);
        }
    }

    void VisitTypeCheck::visitPred(Pred *pred) {
        /* Code For Pred Goes Here */

        if (pred->expr_)
            pred->expr_->accept(this);
    }

    void VisitTypeCheck::visitIsZero(IsZero *is_zero) {
        /* Code For IsZero Goes Here */

        if (is_zero->expr_)
            is_zero->expr_->accept(this);
        if(!_type->cmp(new TypeNat())){
            fprintf(stderr, "error: %d,%d: semantic error\n", is_zero->line_number, is_zero->char_number);
            exit(1);
        }
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitFix(Fix *fix) {
        /* Code For Fix Goes Here */

        if (fix->expr_)
            fix->expr_->accept(this);
    }

    void VisitTypeCheck::visitNatRec(NatRec *nat_rec) {
        /* Code For NatRec Goes Here */

        if (nat_rec->expr_1)
            nat_rec->expr_1->accept(this);
        auto *type_list1 = new ListType();
        type_list1->push_back(_type);
        if (nat_rec->expr_2)
            nat_rec->expr_2->accept(this);
        auto *type_list2 = new ListType();
        type_list2->push_back(_type);
        if (nat_rec->expr_3)
            nat_rec->expr_3->accept(this);
        Type *type3 = _type;
        if(!type3->cmp(new TypeFun(type_list1, new TypeFun(type_list2, nullptr)))){
            fprintf(stderr, "error: %d,%d: semantic error\n", nat_rec->line_number, nat_rec->char_number);
            exit(1);
        }
        _type = dynamic_cast<TypeFun *>(dynamic_cast<TypeFun *>(type3)->type_)->type_;
    }

    void VisitTypeCheck::visitFold(Fold *fold) {
        /* Code For Fold Goes Here */

        if (fold->type_)
            fold->type_->accept(this);
        if (fold->expr_)
            fold->expr_->accept(this);
    }

    void VisitTypeCheck::visitUnfold(Unfold *unfold) {
        /* Code For Unfold Goes Here */

        if (unfold->type_)
            unfold->type_->accept(this);
        if (unfold->expr_)
            unfold->expr_->accept(this);
    }

    void VisitTypeCheck::visitDotTuple(DotTuple *dot_tuple) {
        /* Code For DotTuple Goes Here */

        if (dot_tuple->expr_)
            dot_tuple->expr_->accept(this);
        if(!_type->cmp(new TypeTuple(nullptr))){
            fprintf(stderr, "error: %d,%d: semantic error\n", dot_tuple->line_number, dot_tuple->char_number);
            exit(1);
        }
        auto *tuple = dynamic_cast<TypeTuple*>(_type);
        if(tuple->listtype_->size() < dot_tuple->integer_){
            fprintf(stderr, "error: %d,%d: semantic error\n", dot_tuple->line_number, dot_tuple->char_number);
            exit(1);
        }
        _type = tuple->listtype_->at(dot_tuple->integer_);
    }

    void VisitTypeCheck::visitConstTrue(ConstTrue *const_true) {
        /* Code For ConstTrue Goes Here */
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitConstFalse(ConstFalse *const_false) {
        /* Code For ConstFalse Goes Here */
        _type = new TypeBool();
    }

    void VisitTypeCheck::visitConstInt(ConstInt *const_int) {
        /* Code For ConstInt Goes Here */
        _type = new TypeNat();
    }

    void VisitTypeCheck::visitVar(Var *var) {
        /* Code For Var Goes Here */

        visitStellaIdent(var->stellaident_);
        _type = _state->findIdentDecl(var->stellaident_);
    }

    void VisitTypeCheck::visitTypeFun(TypeFun *type_fun) {
        /* Code For TypeFun Goes Here */

        if (type_fun->listtype_)
            type_fun->listtype_->accept(this);
        ListType *type_list = _type_list;
        if (type_fun->type_)
            type_fun->type_->accept(this);
        _type = new TypeFun(type_list, _type);
    }

    void VisitTypeCheck::visitTypeSum(TypeSum *type_sum) {
        /* Code For TypeSum Goes Here */

        if (type_sum->type_1)
            type_sum->type_1->accept(this);
        Type *type1 = _type;
        if (type_sum->type_2)
            type_sum->type_2->accept(this);
        Type *type2 = _type;
        _type = new TypeSum(type1, type2);
    }

    void VisitTypeCheck::visitTypeTuple(TypeTuple *type_tuple) {
        /* Code For TypeTuple Goes Here */

        if (type_tuple->listtype_)
            type_tuple->listtype_->accept(this);
        _type = new TypeTuple(_type_list);
    }

    void VisitTypeCheck::visitTypeList(TypeList *type_list) {
        /* Code For TypeList Goes Here */

        if (type_list->type_)
            type_list->type_->accept(this);
        _type = new TypeList(_type);
    }

    void VisitTypeCheck::visitTypeBool(TypeBool *type_bool) {
        /* Code For TypeBool Goes Here */
        _type = type_bool;
    }

    void VisitTypeCheck::visitTypeNat(TypeNat *type_nat) {
        /* Code For TypeNat Goes Here */
        _type = type_nat;
    }

    void VisitTypeCheck::visitTypeUnit(TypeUnit *type_unit) {
        /* Code For TypeUnit Goes Here */
        _type = type_unit;
    }

    void VisitTypeCheck::visitTypeVar(TypeVar *type_var) {
        /* Code For TypeVar Goes Here */

        visitStellaIdent(type_var->stellaident_);
        _type = _state->findIdentType(type_var->stellaident_);
    }

    void VisitTypeCheck::visitATyping(ATyping *a_typing) {
        /* Code For ATyping Goes Here */

        if (a_typing->expr_)
            a_typing->expr_->accept(this);
        if (a_typing->type_)
            a_typing->type_->accept(this);
    }

    void VisitTypeCheck::visitListStellaIdent(ListStellaIdent *list_stella_ident) {
        for (auto & i : *list_stella_ident) {
            visitStellaIdent(i);
        }
    }

    void VisitTypeCheck::visitListDecl(ListDecl *list_decl) {
        for (auto & i : *list_decl) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListLocalDecl(ListLocalDecl *list_local_decl) {
        for (auto & i : *list_local_decl) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListAnnotation(ListAnnotation *list_annotation) {
        for (auto & i : *list_annotation) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListParamDecl(ListParamDecl *list_param_decl) {
        auto *type_list = new ListType();
        for (auto & i : *list_param_decl) {
            i->accept(this);
            type_list->push_back(_type);
        }
        _type_list = type_list;
    }

    void VisitTypeCheck::visitListExpr(ListExpr *list_expr) {
        auto *type_list = new ListType();
        for (auto & i : *list_expr) {
            i->accept(this);
            type_list->push_back(_type);
        }
        _type_list = type_list;
    }

    void VisitTypeCheck::visitListType(ListType *list_type) {
        auto *type_list = new ListType();
        for (auto & i : *list_type) {
            i->accept(this);
            type_list->push_back(_type);
        }
        _type_list = type_list;
    }

    void VisitTypeCheck::visitListRecordFieldType(ListRecordFieldType *list_record_field_type) {
        for (auto & i : *list_record_field_type) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitInteger(Integer x) {
        /* Code for Integer Goes Here */
    }

    void VisitTypeCheck::visitChar(Char x) {
        /* Code for Char Goes Here */
    }

    void VisitTypeCheck::visitDouble(Double x) {
        /* Code for Double Goes Here */
    }

    void VisitTypeCheck::visitString(String x) {
        /* Code for String Goes Here */
    }

    void VisitTypeCheck::visitIdent(Ident x) {
        /* Code for Ident Goes Here */
    }

    void VisitTypeCheck::visitStellaIdent(StellaIdent x) {
        /* Code for StellaIdent Goes Here */
    }

}
namespace Stella {
    void VisitTypeCheck::visitExtension(Extension *t) {}               // abstract class
    void VisitTypeCheck::visitMatchCase(MatchCase *t) {}               // abstract class
    void VisitTypeCheck::visitOptionalTyping(OptionalTyping *t) {}     // abstract class
    void VisitTypeCheck::visitPatternData(PatternData *t) {}           // abstract class
    void VisitTypeCheck::visitPattern(Pattern *t) {}                   // abstract class
    void VisitTypeCheck::visitLabelledPattern(LabelledPattern *t) {}   // abstract class
    void VisitTypeCheck::visitBinding(Binding *t) {}                   // abstract class
    void VisitTypeCheck::visitVariantFieldType(VariantFieldType *t) {} // abstract class
    void VisitTypeCheck::visitRecordFieldType(RecordFieldType *t) {}   // abstract class

    void VisitTypeCheck::visitAnExtension(AnExtension *an_extension) {
        /* Code For AnExtension Goes Here */

        if (an_extension->listextensionname_)
            an_extension->listextensionname_->accept(this);
    }

    void VisitTypeCheck::visitRecord(Record *record) {
        /* Code For Record Goes Here */

        if (record->listbinding_)
            record->listbinding_->accept(this);
    }

    void VisitTypeCheck::visitVariant(Variant *variant) {
        /* Code For Variant Goes Here */

        visitStellaIdent(variant->stellaident_);
        if (variant->exprdata_)
            variant->exprdata_->accept(this);
    }

    void VisitTypeCheck::visitMatch(Match *match) {
        /* Code For Match Goes Here */

        if (match->expr_)
            match->expr_->accept(this);
        if (match->listmatchcase_)
            match->listmatchcase_->accept(this);
    }

    void VisitTypeCheck::visitDotRecord(DotRecord *dot_record) {
        /* Code For DotRecord Goes Here */

        if (dot_record->expr_)
            dot_record->expr_->accept(this);
        visitStellaIdent(dot_record->stellaident_);
    }

    void VisitTypeCheck::visitAMatchCase(AMatchCase *a_match_case) {
        /* Code For AMatchCase Goes Here */

        if (a_match_case->pattern_)
            a_match_case->pattern_->accept(this);
        if (a_match_case->expr_)
            a_match_case->expr_->accept(this);
    }

    void VisitTypeCheck::visitNoTyping(NoTyping *no_typing) {
        /* Code For NoTyping Goes Here */
    }

    void VisitTypeCheck::visitSomeTyping(SomeTyping *some_typing) {
        /* Code For SomeTyping Goes Here */

        if (some_typing->type_)
            some_typing->type_->accept(this);
    }

    void VisitTypeCheck::visitNoPatternData(NoPatternData *no_pattern_data) {
        /* Code For NoPatternData Goes Here */
    }

    void VisitTypeCheck::visitSomePatternData(SomePatternData *some_pattern_data) {
        /* Code For SomePatternData Goes Here */

        if (some_pattern_data->pattern_)
            some_pattern_data->pattern_->accept(this);
    }

    void VisitTypeCheck::visitNoExprData(NoExprData *no_expr_data) {
        /* Code For NoExprData Goes Here */
    }

    void VisitTypeCheck::visitSomeExprData(SomeExprData *some_expr_data) {
        /* Code For SomeExprData Goes Here */

        if (some_expr_data->expr_)
            some_expr_data->expr_->accept(this);
    }

    void VisitTypeCheck::visitPatternVariant(PatternVariant *pattern_variant) {
        /* Code For PatternVariant Goes Here */

        visitStellaIdent(pattern_variant->stellaident_);
        if (pattern_variant->patterndata_)
            pattern_variant->patterndata_->accept(this);
    }

    void VisitTypeCheck::visitPatternTuple(PatternTuple *pattern_tuple) {
        /* Code For PatternTuple Goes Here */

        if (pattern_tuple->listpattern_)
            pattern_tuple->listpattern_->accept(this);
    }

    void VisitTypeCheck::visitPatternRecord(PatternRecord *pattern_record) {
        /* Code For PatternRecord Goes Here */

        if (pattern_record->listlabelledpattern_)
            pattern_record->listlabelledpattern_->accept(this);
    }

    void VisitTypeCheck::visitPatternList(PatternList *pattern_list) {
        /* Code For PatternList Goes Here */

        if (pattern_list->listpattern_)
            pattern_list->listpattern_->accept(this);
    }

    void VisitTypeCheck::visitPatternCons(PatternCons *pattern_cons) {
        /* Code For PatternCons Goes Here */

        if (pattern_cons->pattern_1)
            pattern_cons->pattern_1->accept(this);
        if (pattern_cons->pattern_2)
            pattern_cons->pattern_2->accept(this);
    }

    void VisitTypeCheck::visitPatternFalse(PatternFalse *pattern_false) {
        /* Code For PatternFalse Goes Here */
    }

    void VisitTypeCheck::visitPatternTrue(PatternTrue *pattern_true) {
        /* Code For PatternTrue Goes Here */
    }

    void VisitTypeCheck::visitPatternInt(PatternInt *pattern_int) {
        /* Code For PatternInt Goes Here */

        visitInteger(pattern_int->integer_);
    }

    void VisitTypeCheck::visitPatternSucc(PatternSucc *pattern_succ) {
        /* Code For PatternSucc Goes Here */

        if (pattern_succ->pattern_)
            pattern_succ->pattern_->accept(this);
    }

    void VisitTypeCheck::visitPatternVar(PatternVar *pattern_var) {
        /* Code For PatternVar Goes Here */

        visitStellaIdent(pattern_var->stellaident_);
    }

    void VisitTypeCheck::visitALabelledPattern(ALabelledPattern *a_labelled_pattern) {
        /* Code For ALabelledPattern Goes Here */

        visitStellaIdent(a_labelled_pattern->stellaident_);
        if (a_labelled_pattern->pattern_)
            a_labelled_pattern->pattern_->accept(this);
    }

    void VisitTypeCheck::visitABinding(ABinding *a_binding) {
        /* Code For ABinding Goes Here */

        visitStellaIdent(a_binding->stellaident_);
        if (a_binding->expr_)
            a_binding->expr_->accept(this);
    }

    void VisitTypeCheck::visitTypeRecord(TypeRecord *type_record) {
        /* Code For TypeRecord Goes Here */

        if (type_record->listrecordfieldtype_)
            type_record->listrecordfieldtype_->accept(this);
    }

    void VisitTypeCheck::visitTypeVariant(TypeVariant *type_variant) {
        /* Code For TypeVariant Goes Here */

        if (type_variant->listvariantfieldtype_)
            type_variant->listvariantfieldtype_->accept(this);
    }

    void VisitTypeCheck::visitTypeRec(TypeRec *type_rec) {
        /* Code For TypeRec Goes Here */

        visitStellaIdent(type_rec->stellaident_);
        if (type_rec->type_)
            type_rec->type_->accept(this);
    }

    void VisitTypeCheck::visitAVariantFieldType(AVariantFieldType *a_variant_field_type) {
        /* Code For AVariantFieldType Goes Here */

        visitStellaIdent(a_variant_field_type->stellaident_);
        if (a_variant_field_type->optionaltyping_)
            a_variant_field_type->optionaltyping_->accept(this);
    }

    void VisitTypeCheck::visitARecordFieldType(ARecordFieldType *a_record_field_type) {
        /* Code For ARecordFieldType Goes Here */

        visitStellaIdent(a_record_field_type->stellaident_);
        if (a_record_field_type->type_)
            a_record_field_type->type_->accept(this);
    }

    void VisitTypeCheck::visitListExtensionName(ListExtensionName *list_extension_name) {
        for (auto & i : *list_extension_name) {
            visitExtensionName(i);
        }
    }

    void VisitTypeCheck::visitListExtension(ListExtension *list_extension) {
        for (auto & i : *list_extension) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListMatchCase(ListMatchCase *list_match_case) {
        for (auto & i : *list_match_case) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListPattern(ListPattern *list_pattern) {
        for (auto & i : *list_pattern) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListLabelledPattern(ListLabelledPattern *list_labelled_pattern) {
        for (auto & i : *list_labelled_pattern) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListBinding(ListBinding *list_binding) {
        for (auto & i : *list_binding) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitListVariantFieldType(ListVariantFieldType *list_variant_field_type) {
        for (auto & i : *list_variant_field_type) {
            i->accept(this);
        }
    }

    void VisitTypeCheck::visitExtensionName(ExtensionName x) {
        /* Code for ExtensionName Goes Here */
    }

}
