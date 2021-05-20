#ifndef NTS_AEXCEPTION_HPP_
#define NTS_AEXCEPTION_HPP_

#include <exception>
#include <iostream>
#include <string>

class AException : public std::exception
{
public:
    AException(const std::string &) throw();
    virtual ~AException() throw();

    const char *what() const throw();

private:
    std::string m_strMsg;
};

#endif