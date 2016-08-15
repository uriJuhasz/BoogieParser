#ifndef BOOGIEAST_H_INCLUDED
#define BOOGIEAST_H_INCLUDED

#include <map>
#include <string>
#include <list>
using namespace std;

class BoogieASTNode{};
class BoogieNamedEntity : public BoogieASTNode{
public:
    const string name;
protected:
    BoogieNamedEntity(const string& _name) : name(_name){}
};

class BoogieTypeDeclaration;
class BoogieTypeParameterList;
class BoogieScope : BoogieASTNode {};
class BoogieAttribute : public BoogieASTNode {
public:
    const string name;
protected:
    BoogieAttribute(const string& _name) : name(_name){}
};
class BoogieAttributes : public BoogieASTNode {
    public:
        list<unique_ptr<BoogieAttribute>> attributes;
};
class BoogieIdentifier final : BoogieASTNode{
public:
    BoogieIdentifier(const string& _id) : id(_id){}
    const string id;
};
class BoogieProgram : public BoogieScope{
public:
    void addTypeDeclaration(unique_ptr<BoogieTypeDeclaration> d){typeDeclarations.push_back(move(d));}
private:
    map<string,BoogieTypeDeclaration> types;
    list<unique_ptr<BoogieTypeDeclaration>> typeDeclarations;
};

/////////////////////////////////////////////////////////////////
//Types

class BoogieType;
class BoogieTypeArgumentList;

class BoogieType : public BoogieASTNode {
public:
protected:
    BoogieType(){}
};

class BoogieConstructorInstanceType final : public BoogieType{
public:
    BoogieConstructorInstanceType(
        unique_ptr<BoogieIdentifier>& _identifier,
        unique_ptr<BoogieTypeArgumentList> _typeArguments)
        : identifier(move(_identifier)), typeArguments(move(_typeArguments))
    {}

    const unique_ptr<BoogieIdentifier> identifier;
    const unique_ptr<BoogieTypeArgumentList> typeArguments;
};

class BoogieTypeVariable final : public BoogieNamedEntity{
public:
    BoogieTypeVariable(const string& _name) : BoogieNamedEntity(_name){}
};
class BoogieTypeParameterList : public BoogieASTNode{
public:
    const list<BoogieTypeVariable> typeParameters;
};
class BoogieTypeArgumentList : public BoogieASTNode{
public:
    const list<unique_ptr<BoogieType>> typeArguments;
};

class BoogieTypeDeclaration : public BoogieASTNode{
public:
    const unique_ptr<BoogieIdentifier> identifier;
    const unique_ptr<BoogieTypeParameterList> typeParameters;
    const unique_ptr<BoogieType> synonym;
    const unique_ptr<BoogieAttributes> attributes;
protected:
    BoogieTypeDeclaration(
        unique_ptr<BoogieIdentifier>& _identifier,
        unique_ptr<BoogieTypeParameterList>& _typeParameters,
        unique_ptr<BoogieAttributes>& _attributes
    ) : identifier(move(_identifier)),
        typeParameters(move(_typeParameters)),
        attributes(move(_attributes))
        {}
};

class BoogieTypeConstructorDeclaration : public BoogieTypeDeclaration{
public:
    BoogieTypeConstructorDeclaration(
        unique_ptr<BoogieIdentifier>& _identifier,
        unique_ptr<BoogieAttributes>& _attributes,
        unique_ptr<BoogieTypeParameterList>& _typeParameters
    ) : BoogieTypeDeclaration(_identifier,_typeParameters,_attributes)
        {}
};

class BoogieTypeSynonymDeclaration : public BoogieTypeDeclaration{
public:
    const unique_ptr<BoogieType> synonym;
    BoogieTypeSynonymDeclaration(
        unique_ptr<BoogieIdentifier>& _identifier,
        unique_ptr<BoogieAttributes>& _attributes,
        unique_ptr<BoogieTypeParameterList>& _typeParameters,
        unique_ptr<BoogieType>& _synonym
    ) : BoogieTypeDeclaration(_identifier,_typeParameters,_attributes),
        synonym(move(_synonym))
        {}
};

class BoogieConstDeclaration final : public BoogieNamedEntity{};
class BoogieFunctionDeclaration final : public BoogieNamedEntity{};
class BoogieAxiomDeclaration final : public BoogieNamedEntity{};
class BoogieProcedureDeclaration final : public BoogieNamedEntity{};
class BoogieImplementationDeclaration final : public BoogieNamedEntity{};

class BoogieExpression : public BoogieASTNode {};


///////////////////////////////////////////////////////////
//Attirbutes
class BoogieStringAttribute : public BoogieAttribute{
    public:
        BoogieStringAttribute(const string& _name, const string& _attribute)
            : BoogieAttribute(_name),
              attributeString(_attribute){}
        const string attributeString;
};
class BoogieExpressionAttribute : public BoogieAttribute{
    public:
        BoogieExpressionAttribute(const string& _name, unique_ptr<BoogieExpression> _expression)
            : BoogieAttribute(_name),
              expression(move(_expression)){}
        const unique_ptr<BoogieExpression> expression;
};
///////////////////////////////////////////////////////////

#endif // BOOGIEAST_H_INCLUDED
