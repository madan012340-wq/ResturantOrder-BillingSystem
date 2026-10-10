#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <limits>
using namespace std;

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseConsole()
{
    cout << "\nPress Enter to continue...";
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
		cout<<"Invalid input! Enter a number: ";
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
			cout<<endl<<"		  FOOD & BEVERAGE MENU"<<endl<<endl;
			for(int i=0; i<itemcount; i++)
				cout<<" "<<itemcode[i]<<".\t"<<itemname[i]<<"\t\tRs. "<<itemprice[i]<<endl;
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
				cout<<"     OPTIONS "<<endl<<endl;
				cout<<" 1. VIEW MENU "<<endl;
				cout<<" 2. TAKE ORDER "<<endl;
				cout<<" 3. CANCEL ORDER "<<endl;
				cout<<" 4. VIEW CURRENT ORDER "<<endl;
				cout<<" 5. PRINT BILL & CHECKOUT "<<endl;
				cout<<" 6. EXIT"<<endl;
				cout<<" ENTER A NUMBER : ";
				choice=getInt();
				if(choice>6||choice<1) cout<<"ERROR: Out of range"<<endl;
			}
			while(choice>6||choice<1);
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
	cout<<"Enter item code: ";
	c=getInt();
	for(i=0; i<itemcount; i++)
		if(itemcode[i]==c)
			break;
	if(i==itemcount)
	{
		cout<<"Invalid code."<<endl;
		return;
	}
	cout<<"Enter quantity: ";
	q=getInt();
	if(q<=0)
	{
		cout<<"Quantity must be atleast 1."<<endl;
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
	cout<<"Order placed: "<<q<<" x "<<itemname[i]<<" = Rs."<<cost<<endl;
}
void Customer::cancel_order()
{
	if(n==0)
	{
		cout<<"No orders to cancel."<<endl;
		return;
	}
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders file."<<endl;
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
		cout<<"No record found."<<endl;
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
	cout<<"Last order cancelled."<<endl;
}
void Customer::show_summary()
{
	cout<<"Customer ID: "<<custid<<endl;
	cout<<"----- Your orders  -----"<<endl;
	if(n==0)
	{
		cout<<"Nothing ordered yet."<<endl;
		return;
	}
	for(int i=0; i<n; i++)
	{
		cout<<qtys[i]<<" x "<<names[i]<<" = Rs."<<prices[i]<<endl;
	}
	cout<<"Total cost of food eaten: Rs."<<total<<endl;
}
void Customer::checkout()
{
	int dd,mm,yy;
	string phone;
	while(true)
	{
		cout<<"Enter your phone number (must start with 98 and be 10 digits): ";
		cin>>phone;
		if(phonecheck(phone)) break;
		cout<<"Invalid phone number!"<<endl;
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
	cout<<"===== BILL ====="<<endl;
	cout<<"Phone: "<<phone<<endl;
	show_summary();
	cout<<"Thank you for visiting!"<<endl;
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
		cout<<"Menu is full!"<<endl;
		return;
	}
	cout<<"Enter new item code: ";
	itemcode[itemcount]=getInt();
	cout<<"Enter item name (one word, eg FriedRice): ";
	cin>>itemname[itemcount];
	cout<<"Enter price: ";
	itemprice[itemcount]=getInt();
	itemcount++;
	savemenu();
	cout<<"Item added."<<endl;
}
void Management::edit_menu()
{
	viewmenu();
	int code;
	cout<<"Enter the item code you want to edit: ";
	code=getInt();
	int i;
	for(i=0; i<itemcount; i++)
		if(itemcode[i]==code) 
		break;
	if(i==itemcount)
	{
		cout<<"Item not found."<<endl;
		return;
	}
	cout<<"Current: "<<itemname[i]<<" Rs."<<itemprice[i]<<endl;
	cout<<" 1. Rename"<<endl;
	cout<<" 2. Change price"<<endl;
	cout<<" 3. Delete item"<<endl;
	cout<<" 4. Cancel"<<endl;
	int ch;
	cout<<"Choice: ";
	ch=getInt();
	if(ch==1)
	{
		cout<<"Enter new name: ";
		cin>>itemname[i];
	}
	else if(ch==2)
	{
		cout<<"Enter new price: ";
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
		cout<<"Item deleted."<<endl;
	}
	else
	{
		cout<<"Edit cancelled."<<endl;
		return;
	}
	savemenu();
	cout<<"Menu file updated."<<endl;
}
void Management::sold_foods()
{
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int d,m,y,cid,code,q,p,cost,grand=0;
	string nm,ph;
	cout<<"----- SOLD FOODS / ALL CUSTOMER RECORDS -----"<<endl;
	while(fin>>d>>m>>y>>cid>>code>>nm>>q>>p>>cost>>ph)
	{
		cout<<d<<"/"<<m<<"/"<<y<<" | "<<ph<<" | "<<nm<<" x"<<q<<" | Rs."<<cost<<endl;
		grand+=cost;
	}
	fin.close();
	cout<<"Grand total income: Rs."<<grand<<endl;
}
void Management::customer_search()
{
	string phone;
	int d,m,y,cid,code,q,p,cost,sum=0,found=0;
	cout<<"Enter customer phone number to search: ";
	cin>>phone;
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	string nm,ph;
	while(fin>>d>>m>>y>>cid>>code>>nm>>q>>p>>cost>>ph)
	{
		if(ph==phone)
		{
			cout<<d<<"/"<<m<<"/"<<y<<" | "<<nm<<" x"<<q<<" | Rs."<<cost<<endl;
			sum+=cost;
			found=1;
		}
	}
	fin.close();
	if(found==1)
		cout<<"Total spent by "<<phone<<": Rs."<<sum<<endl;
	else
		cout<<"No orders found for this number."<<endl;
}
void Management::daily_income()
{
	int dd,mm,yy,phsum[500],i,k,pc=0;
	string phones[500];
	do
	{
		cout<<"Enter day: ";
		dd=getInt();
		if(dd<1||dd>31) cout<<"Day must be between 1 and 31!"<<endl;
	}
	while(dd<1||dd>31);
	do
	{
		cout<<"Enter month: ";
		mm=getInt();
		if(mm<1||mm>12) cout<<"Month must be between 1 and 12!"<<endl;
	}
	while(mm<1||mm>12);
	do
	{
		cout<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) cout<<"Enter a valid year (1900-2100)!"<<endl;
	}
	while(yy<1900||yy>2100);
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
	{
		cnt++;
	}
	fin.close();
	cout<<"----- RECORD FOR "<<dd<<"/"<<mm<<"/"<<yy<<" -----"<<endl;
	for(i=0; i<cnt; i++)
	{
		if(d[i]==dd && m[i]==mm && y[i]==yy)
		{
			cout<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i]<<endl;
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
	cout<<"----- PER CUSTOMER BREAKDOWN -----"<<endl;
	for(k=0; k<pc; k++)
		cout<<phones[k]<<" : Rs."<<phsum[k]<<endl;
	cout<<"Total income for the day: Rs."<<sum<<endl;
}
void Management::monthly_income()
{
	int mm,yy,i,k,pc=0,phsum[500];
	string phones[500];
	do
	{
		cout<<"Enter month (1-12): ";
		mm=getInt();
		if(mm<1||mm>12) cout<<"Month must be between 1 and 12!"<<endl;
	}
	while(mm<1||mm>12);
	do
	{
		cout<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) cout<<"Enter a valid year (1900-2100)!"<<endl;
	}
	while(yy<1900||yy>2100);
	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
	{
		cnt++;
	}
	fin.close();
	cout<<"----- RECORD FOR "<<mm<<"/"<<yy<<" -----"<<endl;
	for(i=0; i<cnt; i++)
	{
		if(m[i]==mm && y[i]==yy)
		{
			cout<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i]<<endl;
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
	cout<<"----- PER CUSTOMER BREAKDOWN -----"<<endl;
	for(int k=0; k<pc; k++)
		cout<<phones[k]<<" : Rs."<<phsum[k]<<endl;
	cout<<"Total income for the month: Rs."<<sum<<endl;
}
void Management::yearly_income()
{
	int i,k,yy,pc=0,phsum[500];
	string phones[500];
	do
	{
		cout<<"Enter year: ";
		yy=getInt();
		if(yy<1900||yy>2100) cout<<"Enter a valid year (1900-2100)!"<<endl;
	}
	while(yy<1900||yy>2100);

	ifstream fin("orders.txt");
	if(!fin.is_open())
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0,sum=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
		cnt++;
	fin.close();
	cout<<"----- RECORD FOR "<<yy<<" -----"<<endl;

	for(i=0; i<cnt; i++)
	{
		if(y[i]==yy)
		{
			cout<<ph[i]<<" | "<<nm[i]<<" x"<<q[i]<<" | Rs."<<cost[i]<<endl;
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
	cout<<"----- PER CUSTOMER BREAKDOWN -----"<<endl;
	for(k=0; k<pc; k++)
		cout<<phones[k]<<" : Rs."<<phsum[k]<<endl;
	cout<<"Total income for the year: Rs."<<sum<<endl;
}
int auth()
{
	int chances=0,userkey,userpass;
	int staffkey[1]= {102};
	int staffpass[1]= {2222};
	while(chances<3)
	{
		cout<<"Enter your id:(if you are a customer type in 101)";
		userkey=getInt();
		if(userkey==101)
		{
			cout<<"welcome"<<endl;
			return 1;
		}
		if(userkey!=staffkey[0])
		{
			chances++;
			cout<<"Wrong id. Attempts left: "<<3-chances<<endl;
			continue;
		}
		while(chances<3)
		{
			cout<<"Enter your password: ";
			userpass=getInt();
			if(staffpass[0]==userpass)
			{
				cout<<"welcome"<<endl;
				return 2;
			}
			else
			{
				chances++;
				cout<<"Wrong password. Attempts left: "<<3-chances<<endl;
			}
		}
		break;
	}
	cout<<"Too many failed attempts. You have been kicked from the program."<<endl;
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
				choice=r.DisplayFirstMenu();
				switch(choice)
				{
					case 1:
						mbj1.DisplaySecondMenu();
						break;
					case 2:
						c.place_order();
						break;
					case 3:
						c.cancel_order();
						break;
					case 4:
						c.show_summary();
						break;
					case 5:
						c.checkout();
						return 0;
					case 6:
						break;
				}
				if(choice != 5 && choice != 6)
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
				cout<<"\n===== MANAGEMENT ====="<<endl;
				cout<<" 1. View menu"<<endl;
				cout<<" 2. Add new menu item"<<endl;
				cout<<" 3. Edit menu (rename / reprice / delete)"<<endl;
				cout<<" 4. View all sales logs"<<endl;
				cout<<" 5. Daily income report"<<endl;
				cout<<" 6. Monthly income report"<<endl;
				cout<<" 7. Yearly income report"<<endl;
				cout<<" 8. Search customer by phone"<<endl;
				cout<<" 9. Exit"<<endl;
				cout<<"Choice: ";
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
					cout<<"Invalid choice."<<endl;
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