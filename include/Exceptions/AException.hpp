#ifndef NTS_AEXCEPTION_HPP_
#define NTS_AEXCEPTION_HPP_

#include <exception>
#include <iostream>
#include <string>

class AException : public std::exception
{
public:
    inline explicit AException(const std::string& strMsg) noexcept : m_strMsg(strMsg)
    {
    }

    inline ~AException() noexcept override = default;

    inline const char *what() const noexcept override
    {
        return m_strMsg.c_str();
    }

private:
    std::string m_strMsg;
};

#endif