#ifndef BOOGIEAST_H_INCLUDED
#define BOOGIEAST_H_INCLUDED

#include <map>
#include <string>
using namespace std;

class BoogieASTNode{};
class BoogieTypeDeclaration;
class BoogieAttributes;
class BoogieTypeParameters;
class BoogieProgram : public BoogieASTNode{
    map<string,BoogieTypeDeclaration> types;
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
class BoogieNamedEntity;
final class BoogieConstDeclaration : public BoogieNamedEntity{};
final class BoogieFunctionDeclaration : public BoogieNamedEntity{};
final class BoogieAxiomDeclaration : public BoogieNamedEntity{};
final class BoogieProcedureDeclaration : public BoogieNamedEntity{};
final class BoogieImplementationDeclaration : public BoogieNamedEntity{};

class BoogieType : public BoogieASTNode {};
final class BoogieNoType : public BoogieType{};
class BoogieExpression : public BoogieASTNode {};

class BoogieNamedEntity : public BoogieASTNode{
public:
    const string name;
protected:
    BoogieNamedEntity(const string& _name) : name(_name){}
}
class BoogieTypeVariable : public BoogieNamedEntity{
public:
    BoogieTypeVariable(const string& _name) : BoogieNamedEntity(_name){}
};
class BoogieTypeParameters : public BoogieASTNode{
    const list BoogieTypeVariable typeParameters;
}

///////////////////////////////////////////////////////////
//Attirbutes
class BoogieAttributes : public BoogieASTNode {
    public:
        list BoogieAttribute attributes;
};
class BoogieAttribute : public BoogieASTNode {
public:
    const string name;
protected:
    BoogieAttribute(const string& _name) : name(_name){}
};
class BoogieStringAttribute : public BoogieAttribute{
    public:
        BoogieStringAttribute(const string& _name, const string& _attribute) : BoogieAttribute(name), attributeString(_attribute){}
        const string attributeString;
}
class BoogieExpressionAttribute : public BoogieAttribute{
    public:
        BoogieStringAttribute(const string& _name, const BoogieExpression& _expression) : BoogieAttribute(name), expression(_expression){}
        const BoogieExpression& expression;
}
///////////////////////////////////////////////////////////

#endif // BOOGIEAST_H_INCLUDED
