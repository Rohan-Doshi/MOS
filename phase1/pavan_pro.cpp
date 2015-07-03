#include<iostream>
#include<stdio.h>
//#include<conio.h>
#include<string>
#include<string.h>
#include<stdlib.h>

using namespace std;

//char line[100][41];
string line[100];
int line_no,out_no=0;
int loc=0;
int job=1;

//FILE *fo = fopen("/home/pavan/inout/out.txt","w");
FILE *fi = fopen("job1.txt","r");
FILE *fo = fopen("out.txt","w");
///NEVER USE 2-d char array FOR PROCESSING STRINGS.

///IMPROVEMENT NEEDED:
//REMOVE SOME OF THESE GLOBAL VARIABLES AND REDUNDENT CODE.
//NEED TO BE MORE GENERIC

class CPU
{
public:

    string IR;
    short IC;
    //char R[4]={'\0'};
    string R;
    bool C;
    //char M[100][5];
    string M[100];
    CPU()
    {
//       for(int k=0;k<100;k++)
//        for(int l=0;l<4;l++)
//            M[k][l]='\0';
//
//       for(int k=0;k<100;k++)
//        for(int l=0;l<41;l++)
//            line[k][l]='\0';
    }

    void LOAD(int n)
    {
       string s;
       //int line_no=0;
       //int loc=0;

       cout<<"-----------------\n"<<n<<"\n";

       for(line_no=0; line_no<n; line_no++)
        cout<<"line"<<line_no<<":"<<line[line_no]<<"\n";

        for(line_no=0; line_no<n; line_no++)
        {
            if(strstr(line[line_no].c_str(),"$AMJ")!=NULL)
            {

                cout<<"\n------Loading the program into memory...\n";
                cout<<line[line_no]<<"\n";
                //continue;
            }

            else
            {
                if(strstr(line[line_no].c_str(),"$DTA")!=NULL)
                {

                    for(int k=0; k<loc && k<100; k++)
                        cout<<"M"<<k<<":"<<M[k]<<"\n";

                    cout<<"\nExecuting the program...\n";
                    cout<<line[line_no]<<"\n";
                    START_EXECUTION();

                    while(strstr(line[line_no].c_str(),"$END")!=NULL)
                    {
                        line_no++;

                        if(line_no==n)
                            break;
                    }

                    cout<<"\n************Job"<<job++<<" Done!***********\n\n";

                    loc=0;
                   // line_no++;
//                    for(int k=0;k<100;k++)
//                       for(int l=0;l<4;l++)
//                          M[k][l]='\0';

                      for(int k=0;k<100;k++)
                        M[k].clear();
                }

                else
                {
                    //strcpy(s.c_str(),line[line_no]);
                    if(strstr(line[line_no].c_str(),"$END")==NULL)
                    {
                        s.assign(line[line_no],0,40);
                    //don know why the fuck i need to give this range whn line[] has 40 chars anyways.
                    //see inside main whn line[] was being read

                        cout<<"==s: "<<s<<"\n";

                        int len = s.length();
                        int m = len/4;
                        int r = len%4;

                        if(len <= 40)
                        {
                            for(int j=0 ; j< m ;j++)
                            {
                            string ss=s.substr(4*j,4);
                            M[loc].assign(ss);
                            loc++;
                            }
                        }

                         else
                        {
                            for(int j=0 ; j< m ;j++)
                            {
                            //cout<<s.substr(4*j,(4*j)+4)<<"\n";
                            cout<<s.substr(4*j,4).c_str()<<"\n\n";
                            string ss=s.substr(4*j,4);
                            M[loc].assign(ss);
                            loc++;
                            }
                         }
                    }

                        else
                        cout<<"line has end!\n";
                }
            }
        }
        fclose(fo);
    }

    void START_EXECUTION()
    {
        IC=0;
        PROGRAM();
    }

    void PROGRAM()
    {
        string s;
        int SI;
        int pos;

        int flag;

       while(1)
       {
           flag=0;
           IR.assign(M[IC]);
           cout<<"\n--->M"<<IC<<":"<<M[IC]<<"\n";

           if(IR.find("GD")!=string::npos)
            {
                flag=1;
                cout<<"\nGD is called...\n";
                SI=1;
                MOS(SI);
                IC++;
            }

            if(IR.find("PD")!=string::npos)
            {
                flag=1;
                cout<<"\nPD is called...\n";
                SI=2;
                MOS(SI);
                IC++;
            }

            if(IR.find("H")!=string::npos)
            {
                flag=1;
                cout<<"\nHalt is called...\n";
                SI=3;
                MOS(SI);
                break;
            }

            if(IR.find("LR")!=string::npos)
            {
                flag=1;
                cout<<"\nExecuting LR...\n";
                pos = atoi(IR.substr(2,2).c_str());

                //strcpy(R,M[pos]);
                R.assign(M[pos]);

//                if(M[pos][0]!='\0')
//                    strcpy(R,M[pos]);
//
//                else
//                    strcpy(R,"----");

                cout<<"\nM"<<pos<<":"<<M[pos]<<"\n";
                cout<<"Register:"<<R<<"\n";

                IC++;
            }

            if(IR.find("SR")!=string::npos)
            {
                flag=1;
                cout<<"\nExecuting SR...\n";
                pos = atoi(IR.substr(2,2).c_str());

               // strcpy(M[pos],R);
               M[pos].assign(R);

                cout<<"R:"<<R<<"\n";
               // cout<<"Value at "<<pos<<" in memory:"<<M[pos]<<"\n";
                cout<<"M"<<pos<<":"<<M[pos]<<"\n";
                IC++;
            }

            if(IR.find("CR")!=string::npos)
            {
                flag=1;
                cout<<"\nExecuting CR...\n";
                pos = atoi(IR.substr(2,2).c_str());

                if(R.compare(M[pos])==0)
                    C= true;
                else
                    C=false;

                cout<<"\nM"<<pos<<": "<<M[pos]<<"\n";
                cout<<"Register: "<<R<<"\n";
                cout<<"\nValue of compare Register: "<<C<<"\n";
                IC++;
            }

            if(IR.find("BT")!=string::npos)
            {
                flag=1;
                cout<<"\nExecuting BT...\n";

                if(C)
                {
                   cout<<"C is true\n";
                   pos= atoi(IR.substr(2,2).c_str());
                   IC=pos;
                   cout<<"IC changed to:"<<IC<<"\n";
                }

                else
                {
                   cout<<"C is false\n";
                   cout<<"IC remains:"<<IC<<"\n";
                   IC++;
                }
            }

            if(flag==0)
            {
                cout<<"\nHey, Mr. X!, u havn't defined this instruction yet!\n";
                IC++;
            }
//            else
//                exit(0);
        }
    }

    void MOS(int SI)
    {
        string s,ss;
        int pos;

        if(SI==1)
        {
            cout<<"\nExecuting GD...\n";

            pos = atoi(IR.substr(2,2).c_str());
            pos = pos-(pos%10);

            //strcpy(s,line[line_no+1]);
            if(strstr(line[line_no].c_str(),"$DTA")!=NULL)
                line_no++;

            s.assign(line[line_no]);
            int len = s.length();

            cout<<"\nGetting line:"<<s<<"\nLength: "<<len<<"\n";

            if(len <= 40)
            {
                int m = len/4;
                int r = len%4;

                for(int j=0 ; j< m ;j++)
                {
                    //strcpy(M[pos],s.substr(4*j,4).c_str());
                    ss=s.substr(4*j,4).c_str();
                    M[pos].assign(ss);
                    //M[pos][4]='\0';
                    cout<<"M"<<pos<<": "<<M[pos]<<"\n";
                    pos++;
                }

            if(r!=0)
            {
                    ss=s.substr(4*m,r);
                    M[pos].assign(ss);

                    cout<<"M"<<pos<<": "<<M[pos]<<"\n";
                    pos++;
            }

//                while(pos%10!=0)
//                   {
//                        M[loc][4]={'-'};
//                        M[loc][5]='-';
//                        loc++;
//                   }

//                for(int j=0 ; j< m ;j++)
//                    cout<<"---\n"<<M[pos]<<"\n";
            }

            line_no++;
        }

        if(SI==2)
        {
            cout<<"\nExecuting PD...\n";

            pos = atoi(IR.substr(2,2).c_str());
            pos = pos-(pos%10);

            for(int j=pos;j<pos+10;j++)
            {
                 //fprintf(fo,"%s",M[j]);
                 fputs(M[j].c_str(),fo);
                 cout<<"M"<<j<<":"<<M[j]<<"\n";

            }
            fputs("\n",fo);
        }

        if(SI==3)
        {
            cout<<"\nHalting...\n";

            fputs("\n",fo);
            fputs("\n",fo);
            fputs("*****************************************\n",fo);
        }
    }
};

int main()
{
    printf("****\n");
    char str[100];
    int i=0;
    string s;

    CPU c;

    if(fi==NULL)
    {
        printf("Can't open the job file");
        exit(0);
    }

    if(fo==NULL)
    {
        printf("Can't open the output file");
        exit(0);
    }

    while(fgets(str,100,fi) != NULL)
    {
      //  cout<<"str:"<<str<<"\n";
       // strcpy(line[i],str);
        char *p = strtok(str,"\n");  ///remove the newline char...take care of this while calling PD
        line[i].assign(p);
        i++;
    }

    //cout<<"---Inside CPU!"<<"\n";

    fputs("Input:-\n",fo);

    for(int j=0;j<i; j++)
    {
       fputs(line[j].c_str(),fo);
       fputs("\n",fo);
       //fputs(s.c_str(),fo);
       //fprintf(fo,"%s",s.c_str());
    }

    fputs("\n\nOutput:\n",fo);

    c.LOAD(i);

    fclose(fi);
    fclose(fo);

   // getch();
    return 1;
}

