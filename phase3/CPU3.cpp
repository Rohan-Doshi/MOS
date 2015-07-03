#include<sstream>
#include<iostream>
#include<string.h>
#include<string>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<queue>

using namespace std;

ifstream fin;
ofstream fout;
int SI,PI,TI;
bool occupied_pages[30];
int IOI,ch[4]={0,5,5,2};
class PCB
{	
private:
	int TLL,TTL,LLC,TLC,page_table_ptr,terminate_code;
	string p_id;
public:
	void initialize_PCB(string limits)
	{
		//initialize the contents of PCB
		p_id=limits.substr(0,4);
		TTL=s_to_i(limits.substr(4,2))*100+s_to_i(limits.substr(6,2));
		TLL=s_to_i(limits.substr(8,2))*100+s_to_i(limits.substr(10,2));
		LLC=0;TLC=0;terminate_code=0;
	}
	int s_to_i(string operand)//ok
	{
		//return the integer no for the given string
		if(operand[0]>='0' && operand[0]<='9' && operand[1]>='0' && operand[1]<='9')
			return ((int)operand[0]-48)*10+((int)operand[1]-48);
		return -1;
	}
	int get_page_table_ptr()
	{
		//reterieve page table pointer
		return page_table_ptr;
	}
	int set_page_table_ptr(int ptr)
	{
		//set the page table pointer
		page_table_ptr=ptr;
	}
	void increment_TLC()
	{
		//increment the TLC
		TLC++;
	}
	void increment_LLC()
	{
		//increment LLC
		LLC++;
	}
	int get_TLC()
	{
		//get the time limit counter
		return TLC;		
	}
	string get_pid()
	{
		//get the process id
		return p_id;	
	}
	int get_terminate_code()
	{
		//get the terminate code
		return terminate_code;
	}
	void set_terminate_code(int t_c)
	{
		//set the terminate code
		terminate_code=t_c;
	}
	int get_LLC()
	{
		//get the LLC
		return LLC;
	}
	int TLC_is_greater_than_TTL()
	{
		//compare TLC anf TTL
		if(TLC>TTL)
			return 1;
		else
			return 0;
	}
	int LLC_is_greater_than_TLL()
	{
		//compare LLC and TLL
		if(LLC>TLL)
			return 1;
		else
			return 0;
	}
};
PCB process;
class memory
{
	private:
		char mem[300][4];
		char ch;
		int page_table_ptr;
		
	public:
		void reset()//ok
		{
			//reset the memory by replacing every symbol in 2D array by $
			memset(mem,'$',sizeof(char)*300*4);
			memset(occupied_pages,false,sizeof(bool)*30);
			page_table_ptr=process.get_page_table_ptr();
			page_table_ptr=rand()%30;
			occupied_pages[page_table_ptr]=true;
			page_table_ptr*=10;
			process.set_page_table_ptr(page_table_ptr);
			SI=PI=TI=0;
		}
		string get_mem(int pos)//get word form memory
		{
			//get the memory contents of given position
			string temp="";
			for(int i=0;i<4;i++)
				temp+=mem[pos][i];
			return temp;
		}
		void set_mem(string s, int pos)//store word in mem
		{
			//set the memory for the recieved value at postion 
			for(int i=0;i<4;i++)
				mem[pos][i]=s[i];
		}
		void set_page_table(int row_num,int page_no)
		{
			//set page table
			page_table_ptr=process.get_page_table_ptr();
			ostringstream temp;
			temp << page_no;
			string table_entry;
			if(page_no<10)
				table_entry="$10"+temp.str();
			else	
				table_entry="$1"+temp.str();
			set_mem(table_entry,page_table_ptr+row_num);
		}
		int allocate_page()
		{
			//allocate page logic goes here
			int page_no=rand()%30;
			while(occupied_pages[page_no]==true)
				page_no=rand()%30;
			occupied_pages[page_no]=true;
			return page_no;
		}

		void store_card(string s,int mem_cnt)
		{
			//extract the words and call the setmem function
			string word="";
			int page_no=allocate_page();
			set_page_table(mem_cnt, page_no);
			page_no*=10;
			for(int i=0;i<s.length();i+=4)
			{
				for(int j=0;j<4;j++)
				{
					word+=s[i+j];
				}	
				set_mem(word,page_no);
				page_no++;
				word="";
			}
		}
		void print_mem()
		{
			int flag=0;
			for(int i=0;i<300;i++)
			{
				if(i%10==0)
				cout<<"----------Block "<<i/10<<"----------\n";
				for(int j=0;j<4;j++)
				{
					cout<<mem[i][j];
				}
				cout<<endl;
			}
		}
}m_obj;

class supervisory_storage
{
	private:
		queue < string > ibq,obq;
		int ebc,ibc,obc;
	public:
		supervisory_storage()
		{
			ebc=10;
		}
		void inc_ebc()
		{
			ebc++;
		}
		void dec_ebc()
		{
			ebc--;
		}
		int get_ebc()
		{
			return ebc;
		}
		void fill_ibq(string temp)
		{
			ibq.push(temp);
		}
		string get_cards_from_ib()
		{
			string s=ibq.front();
			ibq.pop();
			return s;
		}
}super;

class aux_drum
{
	private:
		string drum[100];
		int curr;
	public:
		aux_drum()
		{
			curr=0;
		}
		void set_drum(string s)
		{
			drum[curr]=s;
			++curr;
		}
		string get_drum_entry(int i)
		{
			return drum[i];
		}
		int get_curr()
		{
			return curr;
		}
}drum_obj;

class cpu
{
	private:
		int fetched_IC,pos,flag,program_card_cnt;
		bool terminate,fetched_C;
		string fetched_IR,operand,opreator,fetched_R,compare_string;
		char IR[4],R[4],IC[2];
		bool C,run_mos;
		string s;

	public:
	//set and reset function of all the register
		int s_to_i(string operand)//ok
		{
			//return the integer no for the given string
			if(operand[0]>='0' && operand[0]<='9' && operand[1]>='0' && operand[1]<='9')
				return ((int)operand[0]-48)*10+((int)operand[1]-48);
			return -1;
		}
		void set_IC()
		{
			IC[0]='0';
			IC[1]='0';
		}
		void set_IC(int pos)//ok
		{
			IC[1]=((char)pos%10)+48;
			pos=pos/10;
			IC[0]=((char)pos%10)+48;
		}
		int get_IC()
		{
			int val;
			val=((int)IC[0]-48)*10+((int)IC[1]-48);
			return val;	
		}	
		void inc_IC()//ok
		{
			int val;
			val=get_IC();
			val++;
			set_IC(val);
		}

		void set_IR(int IC)//ok
		{
			string returned_value="";
			returned_value=m_obj.get_mem(IC);
			for(int i=0;i<4;i++)
			IR[i]=returned_value[i];
		}
		string get_IR()//ok
		{
			string ret_IR="";
			for(int i=0;i<4;i++)
			ret_IR+=IR[i];
			return ret_IR;
		}
		void set_R(int pos)//ok
		{
			string returned_value="";
			returned_value=m_obj.get_mem(pos);
			for(int i=0;i<4;i++)
				R[i]=returned_value[i];
		}
		string get_R()//ok
		{
			string ret_R="";
			for(int i=0;i<4;i++)
			{
				ret_R+=R[i];
			}
			return ret_R;
		}
		void set_C(bool value)//ok
		{
			C=value;
		}
		bool get_C()//ok
		{
			return C;
		}
		int address_tranlation(int virtual_add)
		{
			//address translation logic
			int page=process.get_page_table_ptr()+(virtual_add/10);//get the page table pointer and the virtual address tens place
			string value_page=m_obj.get_mem(page);//get the required page from the memory
			if(value_page[1]=='$')//check for the page fault
			{
				PI=3;
				return -1;
			}
			value_page=value_page.substr(2,2);//reterieve the page value
			return (s_to_i(value_page)*10+(virtual_add%10));//return the real by adding the unit place to retrieve the specific instruction in the that page
		}
		int address_tranlation(string op)
		{
			//check for operan error
			if(s_to_i(op)==-1)
			{
				PI=2;
				return -2;
			}
			else
				address_tranlation(s_to_i(op));//no operand error go for tranlation logic
		}
		void IR1()
		{
			if(fin.eof())
				flag=4;
			else
			{
				getline(fin,s);
				cout<<"IN CH1....reading from file\nRead value : "<<s<<endl;
				if(s.find("$AMJ")!=-1)
				{
					//reset the memory
					process.initialize_PCB(s.substr(4,12));
					flag=1;
					program_card_cnt=0;
				}
				else if(s.find("$DTA")!=-1)
				{	
					flag=2;
				}
				else if(s.find("$END")!=-1)
				{
					//proceed to the next job
					flag=3;
				}
				else if(flag==1 || flag==2)
				{
					cout<<"Pushing data in IFQ"<<endl;
					//load the program card as soon as it comes
					super.fill_ibq(s);
					super.dec_ebc();
				}
				ch[1]=5;
			}
		}
		void IR3()
		{
			cout<<"IN CH3\n";
			string temp;
			if(flag==4)
			{
				cout<<"EOF found...Memory printing\n";
				m_obj.print_mem();
				exit(0);
			}
			if(super.get_ebc()!=10)
			{
				temp=super.get_cards_from_ib();
				cout<<"Found data in IFQ....Data is "<<temp<<endl;
				super.inc_ebc();
				drum_obj.set_drum(temp);
				if(flag==1)
					program_card_cnt++;
			}
			else if(flag==3)
			{
				cout<<"Moving from drum to memory\n";
				//int lim_cnt=drum_obj.get_curr();
				for(int i=0;i<program_card_cnt;i++)
				{
					s=drum_obj.get_drum_entry(i);
					m_obj.store_card(s,i);
				}
				program_card_cnt=0;
			}
			ch[3]=2;
		}
		void startexe()
		{
			//fetch decode execute cycle given below
			set_IC(0);
			terminate=false;
			while(!terminate)
			{
				//fetch ic
				//fetch ir form the location specified by ic
				//get the value of operator and operand
				run_mos=false;
				fetched_IC=address_tranlation(get_IC()); 	
				inc_IC();
				set_IR(fetched_IC);
				fetched_IR=get_IR();
				if((fetched_IR.compare("H"))==3)
				//handle the error of instruction single H , suppose if the user writes in the program H instead of H123
					fetched_IR="Hrrr";
				opreator=fetched_IR.substr(0,2);
				operand=fetched_IR.substr(2,2);
				pos=address_tranlation(s_to_i(operand));
				if(address_tranlation(operand)!=-2 || !(operand.compare("rr")))
				{
					if(!(opreator.compare("LR")))//ok
					{
						//set the contents of the register R from the given location of memory specified in the operand
						//cout<<"LR";
						if(pos==-1)
						{
							run_mos=true;//if operand error runmos = true
						}
						else
						{
							set_R(pos);
							process.increment_TLC();

						}
					}
					else if (!(opreator.compare("SR")))//ok
					{
						//get the contents of register R
						//store them at the location of memory specified in the operand
						//cout<<"SR";
						//process.increment_TLC();
						fetched_R=get_R();
						if(pos==-1)
						{
							run_mos=true;
						}
						else
						{
							m_obj.set_mem(fetched_R, pos);
							process.increment_TLC();
						}
					}
					else if (!(opreator.compare("CR")))//ok
					{
						//get the contents of register r
						//compare with given memory location
						//if the values of above two matches then set toggle register to true
						//else set the toggle register to false
						//cout<<"CR";
						fetched_R=get_R();
						if(pos==-1)
						{
							run_mos=true;
						}
						else
						{
							process.increment_TLC();
							compare_string=m_obj.get_mem(pos);
							if(fetched_R.compare(compare_string)==0)
								set_C(true);
							else
								set_C(false);
						}
					}
					else if (!(opreator.compare("BT")))//ok
					{

						//if the value of toggle register is true 
						//we change the value of ic
						//cout<<"BT";
						//fetched_C=get_C();
						if(fetched_C)
						{	
							fetched_C=get_C();
							//int get_physical_add=address_tranlation();
							set_IC(s_to_i(operand));
						}
						process.increment_TLC();
					}
					else if (!(opreator.compare("GD")))//ok
					{
						if(pos!=-1)
						{
							process.increment_TLC();
							SI=1;
						}
						run_mos=true;
					}
					else if (!(opreator.compare("PD")))//ok
					{
						if(pos!=-1)
						{
							process.increment_TLC();
							SI=2;
						}
						run_mos=true;
					}
					else if (!(opreator.compare("Hr")))//ok
					{
						process.increment_TLC();
						fetched_IR="H";
						SI=3;
						run_mos=true;
					}
					else
					{
						PI=1;
						run_mos=true;
					}
				}
				else
				{
					run_mos=true;
				}
				if(process.TLC_is_greater_than_TTL())
				{
					TI=2;
					run_mos=true;
					terminate=true;
					process.set_terminate_code(3);
				}
				if(run_mos)
					MOS();
			}
		}
		void MOS()
		{
			//acording to ti si and pi set here we will handle the interrupt
			if(TI==0 && SI==1)
			{
				//get the instrution of the file in terms of 4 words 
				//store it one by one into memory
				//at start store the program card from memory location with unit place 0

				string s;
				pos=(pos/10)*10;
				getline(fin,s);
				if(s.find("$END")!=-1)// if the $END encountered before hand then out of data
				{
					terminate=true;
					process.set_terminate_code(1);
				}
				else
				{
					if(!s.empty() && s[s.size()-1]=='\r')
						s.erase(s.size()-1);
					int len=s.length(),start=0,i;
					string s1;
					for(i=pos;start<len;i++)
					{
						if((len-start)<4)
							s1=s.substr(start,(len-start));
						else	
							s1=s.substr(start,4);
						start+=4;
						m_obj.set_mem(s1,i);
					}
				}
				SI=0;
			}
			else if(TI==2 && SI==1)
			{
				terminate=true;
				process.set_terminate_code(3);
			}
			else if((TI==0 || TI==2) && SI==2)
			{
				process.increment_LLC();
				if(process.LLC_is_greater_than_TLL())//lime limit exceeded
				{
					terminate=true;
					process.set_terminate_code(2);
				}
				//put the data from memory into the file specified
				else
				{
					int pos=address_tranlation(s_to_i(operand)),flag=0;
					pos=(pos/10)*10;
					string ans="",temp="";
					for(int i=pos;i<pos+10;i++)
					{
						temp=m_obj.get_mem(i);
						for(int j=0;j<4;j++)
						{
							if(temp[j]=='\0' || temp[j]=='$')
							{
								break;
								flag=1;
							}
							ans+=temp[j];
						}
						if(flag)
							break;
					}
					fout<<ans<<endl;
					if(TI==2)//time limit exceeded
					{
						terminate=true;
						process.set_terminate_code(3);
					}
				}
				SI=0;
			}
			else if((TI==0 || TI==2) && SI==3)//no error has ocurred terminated the program normally
			{
				//make terminate true to end the program
				//as we encountered the halt instruction 

				terminate=true;
				process.set_terminate_code(0);
			}
			else if(TI==0 && PI==1)//operation code error has occured
			{
				process.set_terminate_code(4);
				terminate=true;
			}
			else if(TI==0 && PI==2)//operand code error has ocurred
			{
				process.set_terminate_code(5);
				terminate=true;
			}

			else if(TI==0 && PI==3)//handle the valid page fault or terminate with invaild page faults
			{
				if(!(opreator.compare("GD")) || !(opreator.compare("SR")))
				{
					process.increment_TLC();
					int page_no=m_obj.allocate_page();
					m_obj.set_page_table((s_to_i(operand))/10,page_no);
					set_IC(get_IC()-1);
				}
				else
				{
					process.set_terminate_code(6);
					terminate=true;
				}
			}

			else if(TI==2 && PI==1)//time limit exceeded with operation code error
			{
				process.set_terminate_code(7);
				terminate=true;
			}

			else if(TI==2 && PI==2)//time limit exceeded with operand error
			{
				process.set_terminate_code(8);
				terminate=true;
			}
			
			else if(TI==2 && PI==3)//time limit exceeded
			{
				process.set_terminate_code(3);
				terminate=true;
			}
			switch(IOI)
			{
				case 0:
				break;
				case 1:
					IR1();
				break;
				case 2:
				//	IR2();
				break;
				case 3:
				//	IR2();
					IR1();
				break;
				case 4:
					IR3();
				break;
				case 5:
					IR3();
					IR1();
				break;
				case 6:
					IR3();
					//IR2();
				break;
				case 7:
					IR3();
					//IR2();
					IR1();

				break;
			}
			IOI=0;
			if(terminate)
			{
				fout<<process.get_pid()<<" ";
				switch(process.get_terminate_code())
				{
					case 0:fout<<"NO ERROR\n";
					       break;
					case 1:fout<<"OUT OF DATA\n";
					       break;
					case 2:fout<<"LINE LIMIT EXCEEDED\n";
					       break;
					case 3:fout<<"TIME LIMIT EXCEEDED\n";
					       break;
					case 4:fout<<"OPERATION CODE ERROR\n";
					       break;
					case 5:fout<<"OPERAND ERROR\n";
					       break;
					case 6:fout<<"INVALID PAGE FAULT\n";
					       break;
					case 7:fout<<"TIME LIMIT EXCEEDED with OPERATION CODE ERROR\n";
					       break;
					case 8:fout<<"TIME LIMIT EXCEEDED with OPERAND ERROR\n";
					       break;
				}
				fout<<get_IC()<<" "<<fetched_IR<<" "<<process.get_TLC()<<" "<<process.get_LLC()<<endl; 
				fout<<endl<<endl;
			}
		}
}exe;

void simulation()
{
	// process.increment_TLC();
	// if(process.TLC_is_greater_than_TTL())
	// 	TI=2;
	/////////////////////////////
	for(int i=1;i<4;i++)
	{
		if(ch[i]!=0)
			ch[i]--;
		else
		{
			IOI+=i;
			if(i==3)
				IOI+=1;
		}
	}

}
int main()
{

	//open two files one input and one output
	//create memory and cpu object

	fin.open("job1.txt");
	fout.open("job2_Output2.txt");
	string s,s1;
	int mem_cnt=0;
	m_obj.reset();
	while(true)
	{
		simulation();
		exe.MOS();
		//s_m();
	}
	/*
	while(!(fin.eof()))//ok
	{
		//get the line one byb one and check wether it contains as follows

		getline(fin,s);
		if(s.empty())
		break;
		if(s.find("$AMJ")!=-1)
		{
			//reset the memory
			process.initialize_PCB(s.substr(4,12));
			m_obj.reset();
			mem_cnt=0;
			continue;
		}
		else if(s.find("$DTA")!=-1)
		{	
			exe.startexe();
		}
		else if(s.find("$END")!=-1)
		{
			//proceed to the next job
			mem_cnt=0;	
			continue;
		}
		else 
		{
			//load the program card as soon as it comes
			m_obj.store_card(s,mem_cnt++);
		}
	}*/
	fin.close();
	fout.close();
	return 0;
}
