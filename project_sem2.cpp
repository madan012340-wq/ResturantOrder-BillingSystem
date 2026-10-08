#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;

int itemcode[50];
string itemname[50];
int itemprice[50];
int itemcount = 0;

const char* MENU_FILE   = "menu.txt";
const char* ORDERS_FILE = "orders.txt";

void create_default_menu()
{
	ofstream fout(MENU_FILE);
	int codes[21]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
	string names[21]= {"VegMomo","ChickenMomo","BuffMomo","JholMomo","VegChowmein",
	                   "ChickenChowmein","BuffChowmein","ChickenChoila","AaluSadeko","PaneerChili",
	                   "BuffSekuwa","VegThakaliSet","ChickenThaliSet","MuttonThakaliSet","MasalaTea",
	                   "BlackCoffee","HoneyTea","ColdDrinks","LimeSoda","SweetLassi","MangoLassi"
	                  };
	int prices[21]= {150,200,190,180,120,200,180,280,120,250,380,320,420,550,
	                 50,80,60,70,110,100,120
	                };
	for(int i=0; i<21; i++)
		fout<<codes[i]<<" "<<names[i]<<" "<<prices[i]<<endl;
	fout.close();
}

void load_menu()//creates initial menu
{
	ifstream fin(MENU_FILE);
	if(!fin.is_open())        
	{
		fin.close();
		create_default_menu();
		fin.open(MENU_FILE);
	}
	itemcount=0;
	while(itemcount<50 && fin>>itemcode[itemcount]>>itemname[itemcount]>>itemprice[itemcount])
		itemcount++;
	fin.close();
}

void save_menu()//new item into menu          
{
	ofstream fout(MENU_FILE);
	for(int i=0; i<itemcount; i++)
		fout<<itemcode[i]<<" "<<itemname[i]<<" "<<itemprice[i]<<endl;
	fout.close();
}

void today(int &d,int &m,int &y)
{
	time_t t=time(0);
	tm *now=localtime(&t);
	d=now->tm_mday;
	m=now->tm_mon+1;
	y=now->tm_year+1900;
}

class Menu
{
	public:
		void DisplaySecondMenu()
		{
			cout<<endl<<"		  FOOD & BEVERAGE MENU"<<endl<<endl;
			for(int i=0; i<itemcount; i++)
				cout<<" "<<itemcode[i]<<".   "<<itemname[i]<<"			Rs. "<<itemprice[i]<<endl;
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
				cin>>choice;
				if(choice>6||choice<1) 
				cout<<"ERROR: Out of range"<<endl;
			}
			while(choice>6||choice<1);
			return choice;
		}
};

int valid_phone(string ph)
{
	if(ph.length()!=10)
		return 0;
	if(ph[0]!='9' || ph[1]!='8')
		return 0;
	for(int i=0; i<10; i++)
	{
		if(ph[i]<'0' || ph[i]>'9')
		{
			return 0;
		}
		else
			return 1;
	}
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
		void attach_phone(string phone);
};

void Customer::place_order()
{
	int c,q,d,m,y,cost;
	cout<<"Enter item code: ";
	cin>>c;
	int i;
	for(i=0; i<itemcount; i++){
		if(itemcode[i]==c) 
		break;
		if(i==itemcount)
		{
			cout<<"Invalid code."<<endl;
			return;
		}
	}
	cout<<"Enter quantity: ";
	cin>>q;
	cost=itemprice[i]*q; 
	today(d,m,y);
	ofstream fout(ORDERS_FILE,ios::app);
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
	int last,i;
	if(n==0)
	{
		cout<<"No orders to cancel."<<endl;
		return;
	}
	ifstream fin(ORDERS_FILE);
	if(!fin.is_open())
	{
		cout<<"No orders file."<<endl;
		return;
	}
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt]){
		cnt++;
	}
	fin.close();
	last=-1;
	for(i=0; i<cnt; i++)
		if(cid[i]==custid && ph[i]=="-") last=i;
	if(last==-1)
	{
		cout<<"No record found."<<endl;
		return;
	}
	ofstream fout(ORDERS_FILE);
	for(i=0; i<cnt; i++)
		if(i!=last)
			fout<<d[i]<<" "<<m[i]<<" "<<y[i]<<" "<<cid[i]<<" "<<code[i]<<" "<<nm[i]<<" "<<q[i]<<" "<<p[i]<<" "<<cost[i]<<" "<<ph[i]<<endl;
	fout.close();
	total-=prices[n-1];
	n--;
	cout<<"Last order cancelled."<<endl;
}

void Customer::show_summary()
{
	cout<<"Customer ID: "<<custid<<endl;
	cout<<"----- Your orders this sitting -----"<<endl;
	if(n==0)
	{
		cout<<"Nothing ordered yet."<<endl;
		return;
	}
	for(int i=0; i<n; i++)
		cout<<qtys[i]<<" x "<<names[i]<<" = Rs."<<prices[i]<<endl;
	cout<<"Total cost of food eaten: Rs."<<total<<endl;
}

void Customer::attach_phone(string phone)
{
	ifstream fin(ORDERS_FILE);
	if(!fin.is_open()) return;
	int d[500],m[500],y[500],cid[500],code[500],q[500],p[500],cost[500],cnt=0;
	string nm[500],ph[500];
	while(cnt<500 && fin>>d[cnt]>>m[cnt]>>y[cnt]>>cid[cnt]>>code[cnt]>>nm[cnt]>>q[cnt]>>p[cnt]>>cost[cnt]>>ph[cnt])
		cnt++;
	fin.close();

	int dd,mm,yy;
	today(dd,mm,yy);
	int remaining=n;  
	for(int i=cnt-1; i>=0 && remaining>0; i--)
	{
		if(cid[i]==custid && ph[i]=="-")
		{
			ph[i]=phone;
			remaining--;
		}
	}
	ofstream fout(ORDERS_FILE);
	for(int i=0; i<cnt; i++)
		fout<<d[i]<<" "<<m[i]<<" "<<y[i]<<" "<<cid[i]<<" "<<code[i]<<" "<<nm[i]<<" "<<q[i]<<" "<<p[i]<<" "<<cost[i]<<" "<<ph[i]<<endl;
	fout.close();
}

void Customer::checkout()
{
	string phone;
	while(1) 
	{
		cout<<"Enter your 10-digit phone number (must start with 98): ";
		cin>>phone;
		if(valid_phone(phone)) 
		break;
		cout<<"Invalid number! It must start with 98 and be exactly 10 digits."<<endl;
	}
	attach_phone(phone);
	cout<<"===== BILL ====="<<endl;
	cout<<"Phone: "<<phone<<endl;
	show_summary();
	cout<<"Thank you for visiting!"<<endl;
}

struct OrderRec   //management side              
{
	int d,m,y,cid,code,q,p,cost;
	string nm, phone;
};

int load_orders(OrderRec r[], int max)
{
	ifstream fin(ORDERS_FILE);
	if(!fin.is_open()) return -1;
	int cnt=0;
	while(cnt<max && fin>>r[cnt].d>>r[cnt].m>>r[cnt].y>>r[cnt].cid>>r[cnt].code
	        >>r[cnt].nm>>r[cnt].q>>r[cnt].p>>r[cnt].cost>>r[cnt].phone)
		cnt++;
	fin.close();
	return cnt;
}

void print_report(OrderRec r[], int cnt, string title)
{
	if(cnt==0)
	{
		cout<<"No records for "<<title<<"."<<endl;
		return;
	}
	int sum=0;
	cout<<"----- RECORD FOR "<<title<<" -----"<<endl;
	for(int i=0; i<cnt; i++)
	{
		cout<<r[i].phone<<" | "<<r[i].nm<<" x"<<r[i].q<<" | Rs."<<r[i].cost<<endl;
		sum+=r[i].cost;
	}
	string phones[500];
	int psum[500];
	int pcnt=0;
	for(int i=0; i<cnt; i++)
	{
		int k;
		for(k=0; k<pcnt; k++) if(phones[k]==r[i].phone) break;
		if(k==pcnt)
		{
			phones[pcnt]=r[i].phone;
			psum[pcnt]=0;
			pcnt++;
		}
		psum[k]+=r[i].cost;
	}
	cout<<"----- PER-CUSTOMER -----"<<endl;
	for(int k=0; k<pcnt; k++)
		cout<<phones[k]<<" : Rs."<<psum[k]<<endl;
	cout<<"TOTAL INCOME: Rs."<<sum<<endl;
}

class Management
{
	public:
		void view_menu();
		void add_item();
		void edit_menu();
		void sold_foods();
		void customer_search();
		void daily_income();
		void monthly_income();
		void yearly_income();
};

void Management::view_menu()
{
	Menu m;
	m.DisplaySecondMenu();
}

void Management::add_item()
{
	if(itemcount>=50)
	{
		cout<<"Menu is full (50 items max)."<<endl;
		return;
	}
	cout<<"Enter new item code: ";
	cin>>itemcode[itemcount];
	cout<<"Enter item name (one word, e.g. FriedRice): ";
	cin>>itemname[itemcount];
	cout<<"Enter price: ";
	cin>>itemprice[itemcount];
	itemcount++;
	save_menu();     
	cout<<"Item added and saved to menu.txt."<<endl;
}

void Management::edit_menu()
{
	if(itemcount==0)
	{
		cout<<"Menu is empty."<<endl;
		return;
	}
	view_menu();
	int code;
	cout<<"Enter the item code to edit: ";
	cin>>code;
	int i;
	for(i=0; i<itemcount; i++) if(itemcode[i]==code) break;
	if(i==itemcount)
	{
		cout<<"Item not found."<<endl;
		return;
	}

	cout<<"Current -> "<<itemname[i]<<" : Rs."<<itemprice[i]<<endl;
	cout<<" 1. Rename item"<<endl;
	cout<<" 2. Change price"<<endl;
	cout<<" 3. Delete item"<<endl;
	cout<<" 4. Cancel"<<endl;
	cout<<"Choice: ";
	int ch;
	cin>>ch;
	if(ch==1)
	{
		cout<<"Enter new name (one word): ";
		cin>>itemname[i];
	}
	else if(ch==2)
	{
		cout<<"Enter new price: ";
		cin>>itemprice[i];
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
	save_menu();      // amend file
	cout<<"menu.txt updated."<<endl;
}

void Management::sold_foods()
{
	OrderRec r[500];
	int cnt=load_orders(r,500);
	if(cnt==-1)
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int grand=0;
	cout<<"----- SOLD FOODS / ALL CUSTOMER RECORDS -----"<<endl;
	for(int i=0; i<cnt; i++)
	{
		cout<<r[i].d<<"/"<<r[i].m<<"/"<<r[i].y<<" | "<<r[i].phone
		    <<" | "<<r[i].nm<<" x"<<r[i].q<<" | Rs."<<r[i].cost<<endl;
		grand+=r[i].cost;
	}
	cout<<"Grand total income: Rs."<<grand<<endl;
}

void Management::customer_search()
{
	string phone;
	cout<<"Enter customer phone number to search: ";
	cin>>phone;
	OrderRec r[500];
	int cnt=load_orders(r,500);
	if(cnt==-1)
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int sum=0, found=0;
	cout<<"----- ORDERS BY "<<phone<<" -----"<<endl;
	for(int i=0; i<cnt; i++)
	{
		if(r[i].phone==phone)
		{
			cout<<r[i].d<<"/"<<r[i].m<<"/"<<r[i].y<<" | "<<r[i].nm
			    <<" x"<<r[i].q<<" | Rs."<<r[i].cost<<endl;
			sum+=r[i].cost;
			found=1;
		}
	}
	if(found) cout<<"Total spent by "<<phone<<": Rs."<<sum<<endl;
	else cout<<"No orders found for "<<phone<<"."<<endl;
}

void Management::daily_income()
{
	int dd,mm,yy;
	cout<<"Enter day: ";
	cin>>dd;
	cout<<"Enter month: ";
	cin>>mm;
	cout<<"Enter year: ";
	cin>>yy;
	OrderRec r[500], f[500];
	int cnt=load_orders(r,500);
	if(cnt==-1)
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int fc=0;
	for(int i=0; i<cnt; i++)
		if(r[i].d==dd && r[i].m==mm && r[i].y==yy) f[fc++]=r[i];
	char title[20];
	snprintf(title,20,"%d/%d/%d",dd,mm,yy);
	print_report(f,fc,title);
}

void Management::monthly_income()
{
	int mm,yy;
	cout<<"Enter month (1-12): ";
	cin>>mm;
	cout<<"Enter year: ";
	cin>>yy;
	OrderRec r[500], f[500];
	int cnt=load_orders(r,500);
	if(cnt==-1)
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int fc=0;
	for(int i=0; i<cnt; i++)
		if(r[i].m==mm && r[i].y==yy) f[fc++]=r[i];
	char title[20];
	snprintf(title,20,"%d/%d",mm,yy);
	print_report(f,fc,title);
}

void Management::yearly_income()
{
	int yy;
	cout<<"Enter year: ";
	cin>>yy;
	OrderRec r[500], f[500];
	int cnt=load_orders(r,500);
	if(cnt==-1)
	{
		cout<<"No orders yet."<<endl;
		return;
	}
	int fc=0;
	for(int i=0; i<cnt; i++)
		if(r[i].y==yy) f[fc++]=r[i];
	char title[20];
	snprintf(title,20,"%d",yy);
	print_report(f,fc,title);
}

int auth()
{
	int chances=0,userkey,userpass;
	int staffkey[1]= {102};
	int staffpass[1]= {2222};

	while(chances<3)
	{
		cout<<"Enter your id:(if you are a customer type in 101)";
		cin>>userkey;
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
			cin>>userpass;
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
	load_menu();     // fetch the menu

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
						c.checkout();   // asks & validates phone, prints bill
						return 0;
					case 6:
						break;
				}
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
				cin>>ch;
				switch(ch)
				{
					case 1:
						mg.view_menu();
						break;
					case 2:
						mg.add_item();
						break;
					case 3:
						mg.edit_menu();
						break;
					case 4:
						mg.sold_foods();
						break;
					case 5:
						mg.daily_income();
						break;
					case 6:
						mg.monthly_income();
						break;
					case 7:
						mg.yearly_income();
						break;
					case 8:
						mg.customer_search();
						break;
					case 9:
						break;
					default:
						cout<<"Invalid choice."<<endl;
				}
			}
			while(ch!=9);
			break;
		}
		default:
			return 0;
	}
	return 0;
}