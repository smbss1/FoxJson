#include "Exceptions/ParserError.hpp"

ParserError::ParserError(const Token& oToken, const std::string& strMsg) throw()
	: AException("[line " + std::to_string(oToken.m_iLinesTraversed) + "] " + strMsg) { }
ParserError::~ParserError() throw() { }