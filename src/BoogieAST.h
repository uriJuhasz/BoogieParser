#ifndef BOOGIEAST_H_INCLUDED
#define BOOGIEAST_H_INCLUDED

#include <map>
#include <string>
#include <list>
using namespace std;

class BoogieASTNode{};
class BoogieTypeDeclaration;
class BoogieAttributes;
class BoogieTypeParameters;
class BoogieScope : BoogieASTNode {};
class BoogieIdentifier final : BoogieASTNode{
public:
    BoogieIdentifier(const string& _id) : id(_id){}
    const string id;
};
class BoogieProgram : public BoogieScope{
    map<string,BoogieTypeDeclaration> types;
    list<unique_ptr<BoogieTypeDeclaration>> typeDeclarations;
};
class BoogieType : public BoogieASTNode {
protected:
    BoogieType(){}
};
class BoogieTypeDeclaration : public BoogieASTNode{
public:
    BoogieTypeDeclaration(
        const string& _name,
        const BoogieAttributes& _attributes,
        const BoogieTypeParameters& _typeParameters,
        const BoogieType& _synonym
    ) : name(_name),
        typeParameters(_typeParameters),
        synonym(_synonym),
        attributes(_attributes)
        {}
    const string name;
    const BoogieTypeParameters& typeParameters;
    const BoogieType& synonym;
    const BoogieAttributes& attributes;
};
class BoogieNamedEntity : public BoogieASTNode{
public:
    const string name;
protected:
    BoogieNamedEntity(const string& _name) : name(_name){}
};
class BoogieConstDeclaration final : public BoogieNamedEntity{};
class BoogieFunctionDeclaration final : public BoogieNamedEntity{};
class BoogieAxiomDeclaration final : public BoogieNamedEntity{};
class BoogieProcedureDeclaration final : public BoogieNamedEntity{};
class BoogieImplementationDeclaration final : public BoogieNamedEntity{};

class BoogieNoType final: public BoogieType{
public:
    static BoogieNoType* get(){
        if (singleton==nullptr)
            singleton = new BoogieNoType();
        return singleton;
    }
private:
    BoogieNoType(){};
    static BoogieNoType* singleton;
};
class BoogieExpression : public BoogieASTNode {};

class BoogieTypeVariable : public BoogieNamedEntity{
public:
    BoogieTypeVariable(const string& _name) : BoogieNamedEntity(_name){}
};
class BoogieTypeParameters : public BoogieASTNode{
    const list<BoogieTypeVariable> typeParameters;
};

///////////////////////////////////////////////////////////
//Attirbutes
class BoogieAttribute;
class BoogieAttributes : public BoogieASTNode {
    public:
        list<BoogieAttribute> attributes;
};
class BoogieAttribute : public BoogieASTNode {
public:
    const string name;
protected:
    BoogieAttribute(const string& _name) : name(_name){}
};
class BoogieStringAttribute : public BoogieAttribute{
    public:
        BoogieStringAttribute(const string& _name, const string& _attribute)
            : BoogieAttribute(_name),
              attributeString(_attribute){}
        const string attributeString;
};
class BoogieExpressionAttribute : public BoogieAttribute{
    public:
        BoogieExpressionAttribute(const string& _name, const BoogieExpression& _expression)
            : BoogieAttribute(_name),
              expression(_expression){}
        const BoogieExpression& expression;
};
///////////////////////////////////////////////////////////

#endif // BOOGIEAST_H_INCLUDED
