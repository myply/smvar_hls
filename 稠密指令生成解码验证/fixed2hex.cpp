#include "fixed2hex.h"
using namespace std;
void infixed2hex(ofstream &fout, in_fixed val)
{
    unsigned char *pr;
    unsigned char alist[2];
    int tmp;

    pr = (unsigned char*) &val;
    alist[0] = *pr;
    alist[1] = *(pr+1);

    tmp = (int) alist[1];
    if(tmp < 16)
        fout << hex << "0" << tmp;
    else
        fout << hex << tmp;
    tmp = (int) alist[0];
    if(tmp < 16)
        fout << hex << "0" << tmp ;
    else
        fout << hex << tmp;

}
//void short2hex(ofstream &fout, short val)
//{
//    unsigned char *pr;
//    unsigned char alist[2];
//    int tmp;
//
//    pr = (unsigned char*) &val;
//    alist[0] = *pr;
//    alist[1] = *(pr+1);
//
//    tmp = (int) alist[1];
//    if(tmp < 16)
//        fout << hex << "0" << tmp;
//    else
//        fout << hex << tmp;
//    tmp = (int) alist[0];
//    if(tmp < 16)
//        fout << hex << "0" << tmp ;
//    else
//        fout << hex << tmp;
//}
void short2hex(ofstream &fout, short val)
{
    unsigned char *pr;
    unsigned char alist[2];
    int tmp;

    pr = (unsigned char*) &val;
    alist[0] = *pr;
    alist[1] = *(pr+1);

    tmp = (int) alist[1];
    if(tmp < 16)
        fout << hex << "0" << tmp;
    else
        fout << hex << tmp;
    tmp = (int) alist[0];
    if(tmp < 16)
        fout << hex << "0" << tmp ;
    else
        fout << hex << tmp;
}
