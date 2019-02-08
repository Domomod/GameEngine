#include <utility>

//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_MYEXCEPTIONS_H
#define GAMEENGINE_MYEXCEPTIONS_H

#include <string>
#include <iostream>

class MyException : public std::exception {
public:
    std::string getType();

    std::string getMessage();

    MyException() = default;
    MyException(std::string _exceptionName);
    MyException(const std::string &exceptionName, const std::string &msg);

private:
    std::string exceptionName;
    std::string msg = "No message was set";
};

class ShaderCompilationFailedException : public MyException {
public:
    ShaderCompilationFailedException(const std::string &msg = "No message was set") : MyException("ShaderCompilationFailedException", msg) {}
};

class GluintIdAlreadySetException : public MyException {
public:
    GluintIdAlreadySetException(const std::string &msg = "No message was set") : MyException("GluintIdAlreadySetException", msg) {}
};

class ShadersNotCreatedException : public MyException {
public:
    ShadersNotCreatedException(const std::string &msg = "No message was set") : MyException("ShadersNotCreatedException", msg) {}
};

class ProgramConsolidationFailedException : public MyException {
public:
    ProgramConsolidationFailedException(const std::string &msg = "No message was set") : MyException("ProgramConsolidationFailedException", msg) {}
};

class EmptyFileException : public MyException {
public:
    EmptyFileException(const std::string &msg = "No message was set") : MyException("EmptyFileException", msg) {}
};

class NullProgramException : public MyException {
public:
    NullProgramException(const std::string &msg = "No message was set") : MyException("NullProgramException", msg) {}
};

class NoSuchUniformException : public MyException {
public:
    NoSuchUniformException(const std::string &msg = "No message was set\nTip: check for misspelling errors.") : MyException("NoSuchUniformException", msg) {}
};

class NoSuchAttributeException : public MyException {
public:
    NoSuchAttributeException(const std::string &msg = "No message was set\nTip: check for misspelling errors.") : MyException("NoSuchAttributeException", msg) {}
};
#endif //GAMEENGINE_MYEXCEPTIONS_H
