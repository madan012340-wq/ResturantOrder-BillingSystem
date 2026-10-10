#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <sstream>
using namespace std;

const int CONSOLE_WIDTH = 120;

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string pad(const string &s)
{
	int p = (CONSOLE_WIDTH - (int)s.length()) / 2;
	if(p < 0) p = 0;
	return string(p, ' ');
}

void printCentered(const string &s)
{
	cout << pad(s) << s << endl;
}

void pauseConsole()
{
	cout << pad("Press Enter to continue...") << "Press Enter to continue...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}
int itemcode[50]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
string itemname[50]= {"VegMomo","ChickenMomo","BuffMomo","JholMomo","VegChowmein","ChickenChowmein","BuffChowmein","ChickenChoila","AaluSadeko","PaneerChili","BuffSekuwa","VegThakaliSet","ChickenThaliSet","MuttonThakaliSet","MasalaTea","BlackCoffee","HoneyTea","ColdDrinks","LimeSoda","SweetLassi","MangoLassi"};
int itemprice[50]= {150,200,190,180,120,200,180,280,120,250,380,320,420,550,50,80,60,70,110,100,120};
int itemcount=21;
int getInt()//int,char,float validation
{
	int num;
	cin>>num;
	while(cin.fail())
	{
		cin.clear();
		cin.ignore(1000,'\n');
		cout<<pad("Invalid input! Enter a number: ")<<"Invalid input! Enter a number: ";
		cin>>num;
	}
	return num;
}
void today(int &d,int &m,int &y)
{
	time_t t=time(0);
	tm *now=localtime(&t);
	d=now->tm_mday;
	m=now->tm_mon+1;
	y=now->tm_year+1900;
}
void savemenu()//writes the whole menu back to the file after editing menu
{
	ofstream fout("menu.txt");
	for(int i=0; i<itemcount; i++)
	{
		fout<<itemcode[i]<<" "<<itemname[i]<<" "<<itemprice[i]<<endl;
	}
	fout.close();
}
void loadmenu()
{
	ifstream fin("menu.txt");
	cout<<endl<<endl;
	printCentered("Resturant Order and Billing System");
	cout<<endl;
	if(!fin.is_open())//makes menu.txt from the menu above
	{
		fin.close();
		savemenu();
		return;
	}
	itemcount=0;
	while(itemcount<50 && fin>>itemcode[itemcount]>>itemname[itemcount]>>itemprice[itemcount])
	{
		itemcount++;
	}
	fin.close();
}
class Menu
{
	public:
		void DisplaySecondMenu()
		{
			cout<<"\n\n";
			printCentered("FOOD & BEVERAGE MENU");
			cout<<"\n";
			for(int i=0; i<itemcount; i++)
			{
				string num = to_string(itemcode[i]);
				string line = num + ".";
				line += (num.length() == 1) ? "  " : " ";
				int dots = 60 - (int)itemname[i].length();
				if(dots < 1) dots = 1;
				string pr = to_string(itemprice[i]);
				if(pr.length() < 3) pr = string(3 - pr.length(), ' ') + pr;
				line += itemname[i] + string(dots, '.') + "Rs. " + pr;
				printCentered(line);
			}
			cout<<"\n\n";
		}
};
class Resturant
{
	public:
		int DisplayFirstMenu()
		{
			int choice;
			do
			{
				printCentered("OPTIONS");
				cout<<endl;
				printCentered("1. TAKE ORDER");
				printCentered("2. CANCEL ORDER");
				printCentered("3. VIEW CURRENT ORDER");
				printCentered("4. PRINT BILL & CHECKOUT");
				printCentered("5. EXIT");
				cout<<endl;
				cout<<pad("ENTER A NUMBER : ")<<"ENTER A NUMBER : ";
				choice=getInt();
				if(choice>5||choice<1) printCentered("ERROR: Out of range");
			}
			while(choice>5||choice<1);
			return choice;
		}
};
bool phonecheck(string ph)//phone must start with 98 and be 10 digits
{
	if(ph.length()!=10) return false;
	if(ph[0]!='9'||ph[1]!='8') return false;
	for(int i=0; i<10; i++)
	{
		if(ph[i]<'0'||ph[i]>'9') return false;
	}
	return true;
}
class Customer
{
		int custid;
		string names[20];
		int qtys[20];
		int prices[20];
		int n;
		int total;
	public:
		void setid(int id)
		{
			custid=id;
			n=0;
			total=0;
		}
		int getid()
		{
			return custid;
		}
		void place_order();
		void cancel_order();
		void show_summary();
		void checkout();
};
void Customer::place_order()
{
	int c,q,i,cost,d,m,y;
	Menu ordermenu;
	ordermenu.DisplaySecondMenu();
	cout<<endl;
	cout<<pad("Enter item code: ")<<"Enter item code: ";
	c=getInt();
	for(i=0; i<itemcount; i++)
		if(itemcode[i]==c)
			break;
	if(i==itemcount)
	{
		printCentered("Invalid code.");
		return;
	}
	cout<<pad("Enter quantity: ")<<"Enter quantity: ";
	q=getInt();
	if(q<=0)
	{
		printCentered("Quantity must be atleast 1.");
		return;
	}
	cost=itemprice[i]*q;
	today(d,m,y);
	ofstream fout("orders.txt",ios::app);
	fout<<d<<" "<<m<<" "<<y<<" "<<custid<<" "<<itemcode[i]<<" "<<itemname[i]<<" "<<q<<" "<<itemprice[i]<<" "<<cost<<" -"<<endl;
	fout.close();
	names[n]=itemname[i];
	qtys[n]=q;
	prices[n]=cost;
	n++;
	total+=cost;
	ostringstream oss;
	oss<<"Order placed: "<<q<<" x "<<itemname[i]<<" = Rs."<<cost;
	printCentered(oss.str());
}
void Customer::cancel_order()
{
	if(n==0)
	{
		printCentered("No orders to cancel.");
		return;
	}
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders file.");
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
		cnt++;
	fin.close();
	int last=-1;
	for(int i=0; i<cnt; i++)
	{
		if(cid[i]==custid && ph[i]=="-")
			last=i;	//only this sitting's orders (phone not added yet)
	}
	if(last==-1)
	{
		printCentered("No record found.");
		return;
	}
	ofstream fout("orders.txt");
	for(int i=0; i<cnt; i++)
	{
		if(i!=last)
			fout<<d[i]<<" "<<m[i]<<" "<<y[i]<<" "<<cid[i]<<" "<<code[i]<<" "<<nm[i]<<" "<<q[i]<<" "<<p[i]<<" "<<cost[i]<<" "<<ph[i]<<endl;
	}
	fout.close();
	total-=prices[n-1];
	n--;
	printCentered("Last order cancelled.");
}
void Customer::show_summary()
{
	ostringstream oss;
	oss<<"Customer ID: "<<custid;
	printCentered(oss.str());
	printCentered("----- Your orders  -----");
	if(n==0)
	{
		printCentered("Nothing ordered yet.");
		return;
	}
	for(int i=0; i<n; i++)
	{
		ostringstream oss2;
		oss2<<qtys[i]<<" x "<<names[i]<<" = Rs."<<prices[i];
		printCentered(oss2.str());
	}
	ostringstream oss3;
	oss3<<"Total cost of food eaten: Rs."<<total;
	printCentered(oss3.str());
}
void Customer::checkout()
{
	int dd,mm,yy;
	string phone;
	while(true)
	{
		cout<<pad("Enter your phone number (must start with 98 and be 10 digits): ")<<"Enter your phone number (must start with 98 and be 10 digits): ";
		cin>>phone;
		if(phonecheck(phone)) break;
		printCentered("Invalid phone number!");
	}
	today(dd,mm,yy);
	ifstream fin("orders.txt");
	if(fin.is_open())
	{
		int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0;
		string nm[500],ph[500];
		while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
			cnt++;
		fin.close();
		ofstream fout("orders.txt");
		for(int i=0; i<cnt; i++)
		{
			if(cid[i]==custid && ph[i]=="-" && d[i]==dd && m[i]==mm && y[i]==yy)
				ph[i]=phone;
			fout<<d[i]<<" "<<m[i]<<" "<<y[i]<<" "<<cid[i]<<" "<<code[i]<<" "<<nm[i]<<" "<<q[i]<<" "<<p[i]<<" "<<cost[i]<<" "<<ph[i]<<endl;
		}
		fout.close();
	}
	cout<<endl;
	printCentered("===== BILL =====");
	ostringstream oss;
	oss<<"Phone: "<<phone;
	printCentered(oss.str());
	show_summary();
	printCentered("Thank you for visiting!");
}
class Management
{
	public:
		void viewmenu();
		void add_item();
		void edit_menu();
		void sold_foods();
		void customer_search();
		void daily_income();
		void monthly_income();
		void yearly_income();
};
void Management::viewmenu()
{
	Menu m;
	m.DisplaySecondMenu();
}
void Management::add_item()
{
	if(itemcount>=50)
	{
		printCentered("Menu is full!");
		return;
	}
	cout<<pad("Enter new item code: ")<<"Enter new item code: ";
	itemcode[itemcount]=getInt();
	cout<<pad("Enter item name (one word, eg FriedRice): ")<<"Enter item name (one word, eg FriedRice): ";
	cin>>itemname[itemcount];
	while(itemname[itemcount].length()>50)
	{
		cout<<pad("Name too long (max 50 characters). Enter again: ")<<"Name too long (max 50 characters). Enter again: ";
		cin>>itemname[itemcount];
	}
	cout<<pad("Enter price: ")<<"Enter price: ";
	itemprice[itemcount]=getInt();
	itemcount++;
	savemenu();
	printCentered("Item added.");
}
void Management::edit_menu()
{
	viewmenu();
	int code;
	cout<<pad("Enter the item code you want to edit: ")<<"Enter the item code you want to edit: ";
	code=getInt();
	int i;
	for(i=0; i<itemcount; i++)
		if(itemcode[i]==code) 
		break;
	if(i==itemcount)
	{
		printCentered("Item not found.");
		return;
	}
	ostringstream oss;
	oss<<"Current: "<<itemname[i]<<" Rs."<<itemprice[i];
	printCentered(oss.str());
	printCentered("1. Rename");
	printCentered("2. Change price");
	printCentered("3. Delete item");
	printCentered("4. Cancel");
	int ch;
	cout<<pad("Choice: ")<<"Choice: ";
	ch=getInt();
	if(ch==1)
	{
		cout<<pad("Enter new name: ")<<"Enter new name: ";
		cin>>itemname[i];
	}
	else if(ch==2)
	{
		cout<<pad("Enter new price: ")<<"Enter new price: ";
		itemprice[i]=getInt();
	}
	else if(ch==3)
	{
		for(int j=i; j<itemcount-1; j++)
		{
			itemcode[j]=itemcode[j+1];
			itemname[j]=itemname[j+1];
			itemprice[j]=itemprice[j+1];
		}
		itemcount--;
		printCentered("Item deleted.");
	}
	else
	{
		printCentered("Edit cancelled.");
		return;
	}
	savemenu();
	printCentered("Menu file updated.");
}
void Management::sold_foods()
{
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders yet.");
		return;
	}
	int d,m,y,cid,code,q,p,cost,grand=0;
	string nm,ph;
	cout<<endl;
	printCentered("----- SOLD FOODS / ALL CUSTOMER RECORDS -----");
	while(fin>>d>>m>>y>>cid>>code>>nm>>q>>p>>cost>>ph)
	{
		ostringstream oss;
		oss<<d<<"/"<<m<<"/"<<y<<" | "<<ph<<" | "<<nm<<" x"<<q<<" | Rs."<<cost;
		printCentered(oss.str());
		grand+=cost;
	}
	fin.close();
	ostringstream oss;
	oss<<"Grand total income: Rs."<<grand;
	printCentered(oss.str());
}
void Management::customer_search()
{
	string phone;
	int d,m,y,cid,code,q,p,cost,sum=0,found=0;
	cout<<pad("Enter customer phone number to search: ")<<"Enter customer phone number to search: ";
	cin>>phone;
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders yet.");
		return;
	}
	string nm,ph;
	while(fin>>d>>m>>y>>cid>>code>>nm>>q>>p>>cost>>ph)
	{
		if(ph==phone)
		{
			ostringstream oss;
			oss<<d<<"/"<<m<<"/"<<y<<" | "<<nm<<" x"<<q<<" | Rs."<<cost;
			printCentered(oss.str());
			sum+=cost;
			found=1;
		}
	}
	fin.close();
	if(found==1)
	{
		ostringstream oss;
		oss<<"Total spent by "<<phone<<": Rs."<<sum;
		printCentered(oss.str());
	}
	else
		printCentered("No orders found for this number.");
}
void Management::daily_income()
{
	int dd,mm,yy,phsum[500],i,k,pc=0;
	string phones[500];
	do
	{
		cout<<pad("Enter day: ")<<"Enter day: ";
		dd=getInt();
		if(dd<1||dd>31) printCentered("Day must be between 1 and 31!");
	}
	while(dd<1||dd>31);
	do
	{
		cout<<pad("Enter month: ")<<"Enter month: ";
		mm=getInt();
		if(mm<1||mm>12) printCentered("Month must be between 1 and 12!");
	}
	while(mm<1||mm>12);
	do
	{
		cout<<pad("Enter year: ")<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) printCentered("Enter a valid year (1900-2100)!");
	}
	while(yy<1900||yy>2100);
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders yet.");
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
	{
		cnt++;
	}
	fin.close();
	cout<<endl;
	ostringstream hdr;
	hdr<<"----- RECORD FOR "<<dd<<"/"<<mm<<"/"<<yy<<" -----";
	printCentered(hdr.str());
	for(i=0; i<cnt; i++)
	{
		if(d[i]==dd && m[i]==mm && y[i]==yy)
		{
			ostringstream oss;
			oss<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i];
			printCentered(oss.str());
			sum+=cost[i];
			for(k=0; k<pc; k++)
				if(phones[k]==ph[i]) break;
			if(k==pc)
			{
				phones[pc]=ph[i];
				phsum[pc]=0;
				pc++;
			}
			phsum[k]+=cost[i];
		}
	}
	printCentered("----- PER CUSTOMER BREAKDOWN -----");
	for(k=0; k<pc; k++)
	{
		ostringstream oss;
		oss<<phones[k]<<" : Rs."<<phsum[k];
		printCentered(oss.str());
	}
	ostringstream tot;
	tot<<"Total income for the day: Rs."<<sum;
	printCentered(tot.str());
}
void Management::monthly_income()
{
	int mm,yy,i,k,pc=0,phsum[500];
	string phones[500];
	do
	{
		cout<<pad("Enter month (1-12): ")<<"Enter month (1-12): ";
		mm=getInt();
		if(mm<1||mm>12) printCentered("Month must be between 1 and 12!");
	}
	while(mm<1||mm>12);
	do
	{
		cout<<pad("Enter year: ")<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) printCentered("Enter a valid year (1900-2100)!");
	}
	while(yy<1900||yy>2100);
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders yet.");
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
	{
		cnt++;
	}
	fin.close();
	cout<<endl;
	ostringstream hdr;
	hdr<<"----- RECORD FOR "<<mm<<"/"<<yy<<" -----";
	printCentered(hdr.str());
	for(i=0; i<cnt; i++)
	{
		if(m[i]==mm && y[i]==yy)
		{
			ostringstream oss;
			oss<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i];
			printCentered(oss.str());
			sum+=cost[i];
			for(k=0; k<pc; k++)
				if(phones[k]==ph[i]) break;
			if(k==pc)
			{
				phones[pc]=ph[i];
				phsum[pc]=0;
				pc++;
			}
			phsum[k]+=cost[i];
		}
	}
	printCentered("----- PER CUSTOMER BREAKDOWN -----");
	for(int k=0; k<pc; k++)
	{
		ostringstream oss;
		oss<<phones[k]<<" : Rs."<<phsum[k];
		printCentered(oss.str());
	}
	ostringstream tot;
	tot<<"Total income for the month: Rs."<<sum;
	printCentered(tot.str());
}
void Management::yearly_income()
{
	int i,k,yy,pc=0,phsum[500];
	string phones[500];
	do
	{
		cout<<pad("Enter year: ")<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) printCentered("Enter a valid year (1900-2100)!");
	}
	while(yy<1900||yy>2100);

	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		printCentered("No orders yet.");
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
		cnt++;
	fin.close();
	cout<<endl;
	ostringstream hdr;
	hdr<<"----- RECORD FOR "<<yy<<" -----";
	printCentered(hdr.str());

	for(i=0; i<cnt; i++)
	{
		if(y[i]==yy)
		{
			ostringstream oss;
			oss<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i];
			printCentered(oss.str());
			sum+=cost[i];
			for(k=0; k<pc; k++)
				if(phones[k]==ph[i]) break;
			if(k==pc)
			{
				phones[pc]=ph[i];
				phsum[pc]=0;
				pc++;
			}
			phsum[k]+=cost[i];
		}
	}
	printCentered("----- PER CUSTOMER BREAKDOWN -----");
	for(k=0; k<pc; k++)
	{
		ostringstream oss;
		oss<<phones[k]<<" : Rs."<<phsum[k];
		printCentered(oss.str());
	}
	ostringstream tot;
	tot<<"Total income for the year: Rs."<<sum;
	printCentered(tot.str());
}
int auth()
{
	int chances=0,userkey,userpass;
	int staffkey[1]= {102};
	int staffpass[1]= {2222};
	while(chances<3)
	{
		cout<<pad("Enter your id:")<<"Enter your id:"<<endl;
		cout<<pad("(if you are a customer type in 101) ")<<"(if you are a customer type in 101) ";
		userkey=getInt();
		if(userkey==101)
		{
			printCentered("welcome");
			return 1;
		}
		if(userkey!=staffkey[0])
		{
			chances++;
			ostringstream oss;
			oss<<"Wrong id. Attempts left: "<<3-chances;
			printCentered(oss.str());
			continue;
		}
		while(chances<3)
		{
			cout<<pad("Enter your password: ")<<"Enter your password: ";
			userpass=getInt();
			if(staffpass[0]==userpass)
			{
				printCentered("welcome");
				return 2;
			}
			else
			{
				chances++;
				ostringstream oss;
				oss<<"Wrong password. Attempts left: "<<3-chances;
				printCentered(oss.str());
			}
		}
		break;
	}
	printCentered("Too many failed attempts. You have been kicked from the program.");
	return 0;
}
int main()
{
	clearConsole();
	loadmenu();
	int valid=auth();
	switch(valid)
	{
		case 1:
		{
			Customer c;
			c.setid(101);
			Resturant r;
			Menu mbj1;
			int choice;
			do
			{
				clearConsole();
				cout<<endl<<endl;
				choice=r.DisplayFirstMenu();
				switch(choice)
				{
					case 1:
						c.place_order();
						break;
					case 2:
						c.cancel_order();
						break;
					case 3:
						cout<<endl;
						c.show_summary();
						break;
					case 4:
						c.checkout();
						return 0;
					case 5:
						break;
				}
				if(choice != 4 && choice != 5)
					pauseConsole();
			}
			while(choice!=6);
			break;
		}
		case 2:
		{
			Management mg;
			int ch;
			do
			{
				clearConsole();
				cout<<"\n\n";
				printCentered("===== MANAGEMENT =====");
				cout<<endl;
				printCentered("1. View menu");
				printCentered("2. Add new menu item");
				printCentered("3. Edit menu (rename / reprice / delete)");
				printCentered("4. View all sales logs");
				printCentered("5. Daily income report");
				printCentered("6. Monthly income report");
				printCentered("7. Yearly income report");
				printCentered("8. Search customer by phone");
				printCentered("9. Exit");
				cout<<endl;
				cout<<pad("Choice: ")<<"Choice: ";
				ch=getInt();
				if(ch==1)
					mg.viewmenu();
				else if(ch==2)
					mg.add_item();
				else if(ch==3)
					mg.edit_menu();
				else if(ch==4)
					mg.sold_foods();
				else if(ch==5)
					mg.daily_income();
				else if(ch==6)
					mg.monthly_income();
				else if(ch==7)
					mg.yearly_income();
				else if(ch==8)
					mg.customer_search();
				else if(ch==9)
					break;
				else
					printCentered("Invalid choice.");
				if(ch != 9)
					pauseConsole();
			}
			while(ch!=9);
			break;
		}
		default:
			return 0;
	}
	return 0;
}