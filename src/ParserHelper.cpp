/*
** EPITECH PROJECT, 2019
** CodeGenerator
** File description:
** Created by besseausamuel
*/

#include "Lexer.h"
#include "Token.h"
#include "ParserHelper.h"
#include <cstring>

bool ParserHelper::Init(const std::string& str)
{
    if (!olex.process(str))
        return false;

    olex.begin();
    next_token();

    return true;
}

void ParserHelper::define(const std::string& strId, const std::string& strValue, bool bAddInTrash)
{
    olex.define(strId, strValue, bAddInTrash);
}

Lexer& ParserHelper::GetLexer()
{
    return olex;
}

const Lexer& ParserHelper::GetLexer() const
{
    return olex;
}

void ParserHelper::next_token()
{
	oPreviousToken = CurrentToken();
    for (;;) {
		oCurrentToken = olex.next_token();
        if (oCurrentToken.m_oType != 84)
            break;
        // ErrorAtCurrent(CurrentToken().GetText().c_str());
    }
}

const Token& ParserHelper::PreviousToken() const
{
    return oPreviousToken;
}

const Token& ParserHelper::CurrentToken() const
{
    return oCurrentToken;
}

void ParserHelper::AdvanceToken(const token_advance_mode mode)
{
	std::string errorType = std::string("ERROR");
    if (CurrentToken().m_oType == errorType)
    {
        ErrorAtCurrent(CurrentToken().GetText().c_str());
        return;
    }

    if (e_advance == mode)
    {
        next_token();
    }
}

bool ParserHelper::IsTokenType(const std::string& strType, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType) {
        if (bAdvance)
        	next_token();
        return (true);
    }
    return (false);
}

bool ParserHelper::IsToken(int iType, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType) {
		if (bAdvance)
        	next_token();
        return (true);
    }
    return (false);
}

bool ParserHelper::IsToken(const std::string& strValue, bool bAdvance)
{
    if (olex.TokenMatch(oCurrentToken, strValue))
    {
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}


bool ParserHelper::IsToken(const std::string& strType, const std::string& strValue, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType && olex.TokenMatch(oCurrentToken, strValue))
    {
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsToken(const std::string& strType, const char* strValue, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType && olex.TokenMatch(oCurrentToken, strValue))
    {
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsToken(int iType, const std::string& strValue, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType && olex.TokenMatch(oCurrentToken, strValue))
    {
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsToken(int iType, const char* strValue, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType && olex.TokenMatch(oCurrentToken, strValue))
    {
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsTokenThenAssign(const std::string& strType, Token& oToken, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType)
    {
        oToken = oCurrentToken;
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsTokenThenAssign(const std::string& strType, const char* strValue, Token& oToken, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType && olex.TokenMatch(oCurrentToken, strValue))
    {
        oToken = oCurrentToken;
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool ParserHelper::IsTokenThenAssign(const std::string& strType, Container<Token&, Allocator>& oTokenList, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType)
    {
        oTokenList.push_back(oCurrentToken.GetText());
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool ParserHelper::IsTokenThenAssign(const std::string& strType, const char* strValue, Container<Token&, Allocator>& oTokenList, bool bAdvance)
{
    if (oCurrentToken.m_oType == strType && olex.TokenMatch(oCurrentToken, strValue))
    {
        oTokenList.push_back(oCurrentToken.GetText());
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}


bool ParserHelper::IsTokenThenAssign(int iType, Token& oToken, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType)
    {
        oToken = oCurrentToken;
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::IsTokenThenAssign(int iType, const char* strValue, Token& oToken, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType && olex.TokenMatch(oCurrentToken, strValue))
    {
        oToken = oCurrentToken;
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool ParserHelper::IsTokenThenAssign(int iType, Container<Token&, Allocator>& oTokenList, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType)
    {
        oTokenList.push_back(oCurrentToken.GetText());
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool ParserHelper::IsTokenThenAssign(int iType, const char* strValue, Container<Token&, Allocator>& oTokenList, bool bAdvance)
{
    if (oCurrentToken.m_oType == iType && olex.TokenMatch(oCurrentToken, strValue))
    {
        oTokenList.push_back(oCurrentToken.GetText());
        if (bAdvance)
        	next_token();
        return true;
    }
    return false;
}

bool ParserHelper::PeekTokenIsType(const std::string& strType)
{
    return (oCurrentToken.m_oType == strType);
}

bool ParserHelper::PeekTokenIsType(int iType)
{
    return (oCurrentToken.m_oType == iType);
}

bool ParserHelper::peek_next_token_is_type(int iType)
{
    return (olex.peek_next_token().m_oType == iType);
}

bool ParserHelper::PeekTokenIs(const std::string& s)
{
    return (olex.TokenMatch(CurrentToken(), s));
}

void ParserHelper::ErrorAtCurrent(const char* message)
{
    ErrorAt(CurrentToken(), message);
}

void ParserHelper::Error(const char* message)
{
    ErrorAt(PreviousToken(), message);
}

void ParserHelper::ErrorAt(const Token& token, const char* message)
{
    if (panicMode)
        return;
    panicMode = true;
    fprintf(stderr, "[line %d] Error", token.m_iLinesTraversed);

	std::string errorType = std::string("ERROR");
    if (token.m_oType != errorType) {
        fprintf(stderr, " at '%s'", token.m_strText.c_str());
    }

    fprintf(stderr, ": %s\n", message);
    hadError = true;
}