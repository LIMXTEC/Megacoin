
#ifndef SENDNEWS_H
#define SENDNEWS_H

using namespace std;

class CSendNewsMessage
{
public:
    CSendNewsMessage();
    ~CSendNewsMessage();

    void Test(string header, string text, string trayNotify, string pKey);
};

#endif // SENDNEWS_H
