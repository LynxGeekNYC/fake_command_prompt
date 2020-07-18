/******************************************************************************

                             MS Command Prompt Emulator
                                        By
                                  Alexander Mirvis

*******************************************************************************/
#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <string>
#include <fstream>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Ws2_32.lib")
using namespace std;


void displayProgressBar(int barW, int sleepTime)
{
    float progress = 0.0;
    while (progress <= 1.0) {
        int barWidth = barW;

        cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << int(progress * 100.0) << " %\r";
        cout.flush();

        progress += 0.01;
        if (progress >= 0.0 && progress < 0.25)
        {
            Sleep(sleepTime);
        }
        if (progress >= 0.25 && progress < 0.50)
        {
            Sleep(sleepTime * 2);
        }
        if (progress >= 0.50 && progress < 0.75)
        {
            Sleep(sleepTime * 3);
        }
        if (progress >= 0.75 && progress < 1.0)
        {
            Sleep(sleepTime * 4);
        }
    }
    cout << endl << endl;
}

struct IPv4
{
    unsigned char b1, b2, b3, b4;
    void printIPAddress()
    {
        cout << "IP: " << (int)b1 << "." << (int)b2 << "." << (int)b3 << "." << (int)b4 << endl;
    }
    string getlocalIP()
    {
        return to_string((int)b1) + "." + to_string((int)b2) + "." + to_string((int)b3) + "." + to_string((int)b4);
    }
};

bool localIP(IPv4& myIP)
{
    char szBuffer[1024];

#ifdef WIN32
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0);
    if (::WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
#endif


    if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
    {
#ifdef WIN32
        WSACleanup();
#endif
        return false;
    }

    struct hostent* host = gethostbyname(szBuffer);
    if (host == NULL)
    {
#ifdef WIN32
        WSACleanup();
#endif
        return false;
    }

    //Obtain the computer's IP
    myIP.b1 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b1;
    myIP.b2 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b2;
    myIP.b3 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b3;
    myIP.b4 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b4;
#ifdef WIN32
    WSACleanup();
#endif
    return true;
}

string externalIP() {

    HINTERNET net = InternetOpenA("IP retriever",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0);

    HINTERNET conn = InternetOpenUrlA(net, "http://myexternalip.com/raw",
        NULL,
        0,
        INTERNET_FLAG_RELOAD,
        0);

    char buffer[4096];
    DWORD read;

    InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read);
    InternetCloseHandle(net);

    return string(buffer, read);
}

void findAndReplaceAll(string& data, string toSearch, string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while (pos != string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

void logIP(string localIPAddress, string externalIPAddress)
{
    ofstream outfile;
    outfile.open("lol.txt");

    // write inputted data into the file.
    outfile << "Local IP: " << localIPAddress << endl;
    outfile << "Public IP: " << externalIPAddress << endl;

    // close the opened file.
    outfile.close();
}

void printNetStats()
{
    IPv4 localIPAddressX;
    localIP(localIPAddressX);
    string localIPAddress = localIPAddressX.getlocalIP();
    string externalIPAddress = externalIP();

    cout << "\nActive Connections\n\n";
    string info("    Proto  Local Address\t\t\b\b\b\bForeign Address\t\t\b\b\b\b\bState\n\
    TCP    192.168.0.103:2869     192.168.0.1:50498      TIME_WAIT\n\
    TCP    192.168.0.103:2869     unknown                SCAMMER DETECTED\n\
    TCP    192.168.0.103:49909    52.139.250.253:https   ESTABLISHED\n\
    TCP    192.168.0.103:50135    a23 - 215 - 204 - 10:https   CLOSE_WAIT\n\
    TCP    192.168.0.103:50342    40.84.185.67:9354      ESTABLISHED\n\
    TCP    192.168.0.103:50442    185.63.144.1:https     CLOSE_WAIT\n\
    TCP    192.168.0.103:50443    93.184.220.29:http     CLOSE_WAIT\n\
    TCP    192.168.0.103:50451    152.199.19.161:https   CLOSE_WAIT\n\
    TCP    192.168.0.103:50489    111.221.29.254:https   ESTABLISHED\n\
    TCP    192.168.0.103:50569    40.114.241.141:https   ESTABLISHED\n\
    TCP    192.168.0.103:50570    fjr02s08 - in - f4:https   ESTABLISHED\n\
    TCP    192.168.0.103:50573    fjr02s03 - in - f13:https  ESTABLISHED\n\
    TCP    192.168.0.103:50574    fjr02s08 - in - f3:http    ESTABLISHED\n\
    TCP    192.168.0.103:50575    fjr02s04 - in - f3:http    ESTABLISHED\n\
    TCP    192.168.0.103:50576    fjr02s04 - in - f3:http    ESTABLISHED\n\
    TCP    192.168.0.103:50578    par10s22 - in - f234:https  ESTABLISHED\n\
    TCP    192.168.0.103:50579    fjr01s01 - in - f14:https  ESTABLISHED\n\
    TCP    192.168.0.103:50580    fjr02s03 - in - f10:https  ESTABLISHED\n\
    TCP    192.168.0.103:50581    fra07s30 - in - f46:https  ESTABLISHED\n\
    TCP    192.168.0.103:50582    fjr01s01 - in - f14:https  ESTABLISHED\n\
    TCP    192.168.0.103:50583    fjr02s08 - in - f3:https   ESTABLISHED\n\
    TCP    192.168.0.103:50584    wo - in - f188:5228        ESTABLISHED\n\
    TCP    192.168.0.103:50585    zrh04s07 - in - f3:https   BENCHOD CONNECTED\n\
    TCP    192.168.0.103:50586    zrh04s08 - in - f14:https  ESTABLISHED\n\
    TCP    192.168.0.103:50587    fjr02s08 - in - f3:https   ESTABLISHED\n\
    TCP    192.168.0.103:50589    fra07s30 - in - f46:https  ESTABLISHED\n\
    TCP    192.168.0.103:50590    zrh04s07 - in - f174:https  ESTABLISHED\n\
    TCP    192.168.0.103:50592    zrh04s07 - in - f3:https   ESTABLISHED\n\
    TCP    192.168.0.103:50597    zrh04s07 - in - f174:http  ESTABLISHED\n\
    TCP    192.168.0.103:50598    205:http               ESTABLISHED\n\
    TCP    192.168.0.103:50600    192.168.0.1:ssdp       TIME_WAIT\n\
    TCP    192.168.0.103:50604    192.168.0.1:ssdp       TIME_WAIT\n\
    TCP    192.168.0.103:50636    fjr02s04 - in - f10:https  ESTABLISHED\n");
    findAndReplaceAll(info, localIPAddress, externalIPAddress);
    logIP(localIPAddress, externalIPAddress);
    for (int i = 0; i < info.length(); i++)
    {
        cout << info[i];
        if (info[i] == '\n')
        {
            int random = 1 + rand() % 3;
            if (random == 1)
            {
                Sleep(1000);
            }
            else if (random == 2)
            {
                Sleep(1300);
            }
            else if (random == 3)
            {
                Sleep(1750);
            }
            //cout << random << endl;
        }
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    string param;

    cout << "Microsoft Windows [Version 10.0.18363.959]" << endl;
    cout << "(c) 2019 Microsoft Corporation. All rights reserved." << endl << endl;



    while (true)
    {
        cout << "C:\\Windows\\System32> ";
        cin >> param;
        if (param != "eventvwr" && param != "tree" && param != "netstat")
        {
            cout << "This command is no longer supported by your version of Windows\n\n";
            //exit(1);
        }
        if (param == "eventvwr")
        {
            displayProgressBar(100, 250);
        }
        else if (param == "tree")
        {
            cout << "Windows no longer supports the tree command.\n\n";
        }
        else if (param == "netstat")
        {
            printNetStats();
        }
    }

    return 0;
}
