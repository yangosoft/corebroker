
#include <algorithm>
#include <iostream>


#include "tfile.h"



using namespace std;

/*
 * 
 */
int main(int /*argc*/, char** /*argv*/)
{
    TFile f("/tmp/file.tmp.1");
    TFile::StringList l;
    l.push_back({"Hello world!"});
    l.push_back({"Goodbye cruel world!"});
    f.writeLines(l);
    
    TFile::StringList lInput;
    f.readByLine(lInput);
    
    
    auto it = l.begin();
    
    
    bool ok = true;
    
    std::any_of(lInput.begin(), lInput.end(), [l,&it, &ok](auto s)
    {
        if(*it != s)
        {
            cout << "Error detected. Readed '" << s << "' expected '" << *it << "'" << endl;
            ok = false;
            return false;
        }
        it++;
        return true;
    });
    
    
    
    cout << "Test OK? " << ok << endl;

    return 0;
}

