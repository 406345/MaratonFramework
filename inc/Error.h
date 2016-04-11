/***********************************************************************************
This file is part of Project for MaratonFramework
For the latest info, see  https://github.com/Yhgenomics/MaratonFramework.git

Copyright 2016 Yhgenomics

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************************/

/***********************************************************************************
* Description   :
* Creator       : Shubo Yang
* Date          :
* Modifed       : When      | Who       | What
***********************************************************************************/

#ifndef ERROR_H_
#define ERROR_H_

#include <string>

using std::string;

class Error
{
public :

    // Default
    Error() { };

    // Default
    Error( long long code , string msg ) 
    { 
        this->code_     = code;
        this->message_  = msg;
    };

    // Copy
    Error( Error & error ) 
    {
        this->code_     = error.code_;
        this->message_  = error.message_;
    };

    // Move
    Error( Error && error ) 
    {
        this->code_     = error.code_;
        this->message_  = error.message_;
    };

    Error& operator=( Error & error) 
    { 
        this->code_     = error.code_;
        this->message_  = error.message_;
        return *this;
    };
    
    Error& operator=( Error && error) 
    { 
        this->code_     = error.code_;
        this->message_  = error.message_;
        return *this;
    };
    
    ~Error()
    {

    }

    long long Code    () { return this->code_; }
    void      Code    ( long long  value ) { this->code_ = value; }
    string    Message () { return this->message_; }
    void      Message ( string value ) { this->message_ = value; }
private:

    long long  code_    = 0;
    string     message_ = "";

};

#endif // !ERROR_H_ 