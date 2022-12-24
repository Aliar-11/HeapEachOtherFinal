#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<graphics.h>
#include<easyx.h>

//////////////////////////文件说明//////////////////////////
/*
AdminSignIn.txt：管理员账号、密码
CommonUserList.txt：普通用户账号（手机号）、密码、住址、邮箱
ApplicationList.txt：申请注册的新用户信息
ItemList.txt：物品列表
TypeList.txt：物品类型列表
*/

#define Num 30    //最多可储存的记录数

int ItemNum = 0;    //当前物品数
int TypeNum = 0;    //当前类型数
int CommonUserNum = 0;    //当前普通用户数
int RegisNum = 0;    //待批准的注册申请数

int BackAdmin = -1;    //为-1，跳转回AdminPage；为-1，跳转回CommonUserPage

//物品结构体
typedef struct Item
{
	char i_Name[64];    //物品名称
	char i_Type[64];    //物品类型
	char i_Note[64];    //物品说明：类型的属性
	char i_Tel[64];    //联系人电话
	char i_Addr[64];    //联系人地址
	char i_Email[64];    //联系人邮箱
}Item;

Item ItemArry[Num],ItemFound[Num];

//物品类型结构体
typedef struct Type
{
	char t_Name[64];
	char t_Attrib[64];
}Type;

Type TypeArry[Num];

//管理员结构体
typedef struct Admin
{
	char UserName[64];
	char PassWord[64];
}Admin;

Admin AdminUser;

//普通用户结构体
typedef struct CommonUser
{
	char UserName[64];    //手机号作为用户名
	char PassWord[64];
	char Addr[64];
	char Email[64];
}CommonUser;

CommonUser User[Num],CurrentUser,Regis[Num];

//显示全部数据的页面跳转标识
int FlagAI = -1;
//查询找到页面跳转标识
int FlagFPT = -1;

//前置声明
void AdminPage();    //管理员界面
void AdminSignIn();    //管理员登陆界面
void CommonUserSignIn();    //普通用户登录界面
void CommonUserPage();    //普通用户界面
void Registration();    //新用户注册申请页面
void SaveItem();    //更新ItemList.txt文件
void SaveType();    //更新Type.txt文件
void ReadInfor();    //读取ItemList.txt、TypeList.txt
void SaveLastItem();    //保存最新添加的一样物品到Item.txt
void SaveLastType();    //保存最新添加的一种类型到Type.txt
void SaveApplication();    //保存最新注册的待审核用户
void SaveNewCommonUser(int x);    //保存通过审核的新用户
void ShowAllType();    //显示全部类型
int ShowUserItem();    //显示当前用户添加的物品
void ManageItem();    //增删查改物品
void ManageType();    //增删查改类型（管理员功能）
void AddType();    //添加类型
void DeleteType();    //删除类型
void ShowApplication();    //显示新用户申请信息
void ItemPrint(int tot);    //打印物品信息
void AllItemPrint();    //打印所有物品信息
void ShowAllItem();    //打印所有物品
void TypePrint();    //打印类型信息
int DeletePosition();    //返回要删除物品的下标
void DeleteItem();    //删除物品
void SearchItem();    //查找物品
void AddItem();    //添加物品



/*
	//////////////////////实现图形界面//////////////////////
*/


//首页
void FirstPage()
{
	//读取数据
	ReadInfor();

	//窗口大小
	initgraph(760, 760);
	//鼠标操作1
	MOUSEMSG m1;
	//设置背景图片
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193, 193, 193));
	settextcolor(BLACK);

	//大标题
	char Title[20] = { "你帮我助管理系统" };
	settextstyle(60, 0, "黑体");
	outtextxy(150, 100, Title);

	//4个按钮的背景
	fillrectangle(230, 385, 560, 430);
	fillrectangle(230, 445, 560, 490);
	fillrectangle(230, 505, 560, 550);
	fillrectangle(230, 565, 560, 610);
	setbkmode(TRANSPARENT);
	//4个按钮的文字
	settextstyle(25, 0, "黑体");
	char FirstButton0[20] = { "新用户注册申请" };
	char FirstButton1[20] = { "管理员操作界面" };
	char FirstButton2[20] = { "普通用户操作界面" };
	char FirstButton3[20] = { "退出程序" };
	outtextxy(305, 390, FirstButton0);
	outtextxy(305, 450, FirstButton1);
	outtextxy(290, 510, FirstButton2);
	outtextxy(340, 570, FirstButton3);

	
	//设置按钮操作
	while (true)
	{
		m1 = GetMouseMsg();
		if (m1.x >= 230 && m1.x <= 560 && m1.y >= 385 && m1.y <= 430)
		{
			setlinecolor(RED);
			rectangle(230, 385, 560, 430);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				//申请注册
				Registration();
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 445 && m1.y <= 490)    //管理员界面
		{
			setlinecolor(RED);
			rectangle(230, 445, 560, 490);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{			
				AdminSignIn();    //管理员登录
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 505 && m1.y <= 550)    //普通用户界面
		{
			setlinecolor(RED);
			rectangle(230, 505, 560, 550);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				CommonUserSignIn();    //普通用户登录
			}
		}
		else if (m1.x >= 230 && m1.x <= 560 && m1.y >= 565 && m1.y <= 610)    //退出
		{
			setlinecolor(RED);
			rectangle(230, 565, 560, 610);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				exit(0);
				strcpy(CurrentUser.UserName,"");    //重置当前用户名为空
			}
		}
		else    //鼠标不在任何按钮上悬停
		{
			setlinecolor(WHITE);
			rectangle(230, 385, 560, 430);
			rectangle(230, 445, 560, 490);
			rectangle(230, 505, 560, 550);
			rectangle(230, 565, 560, 610);
		}
	}
}

//管理员界面
void AdminPage()
{
	initgraph(760, 760);   	
	cleardevice();
	//第二个鼠标操作
	MOUSEMSG m2;
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	char SecondTitle1[5] = { "管" };
	char SecondTitle2[5] = { "理" };
	char SecondTitle3[5] = { "员" };
	char SecondTitle4[5] = { "操" };
	char SecondTitle5[5] = { "作" };
	char SecondTitle6[5] = { "界" };
	char SecondTitle7[5] = { "面" };


	settextstyle(30, 0, "黑体");
	outtextxy(50, 150, SecondTitle1);
	outtextxy(50, 210, SecondTitle2);
	outtextxy(50, 270, SecondTitle3);
	outtextxy(50, 330, SecondTitle4);
	outtextxy(50, 390, SecondTitle5);
	outtextxy(50, 450, SecondTitle6);
	outtextxy(50, 510, SecondTitle7);
	setbkmode(TRANSPARENT);

	//4个按钮的背景
	fillrectangle(230, 110, 560, 155);
	fillrectangle(230, 240, 560, 285);
	fillrectangle(230, 370, 560, 415);
	fillrectangle(230, 500, 560, 545);
	setbkmode(TRANSPARENT);

	//4个按钮的文字
	settextstyle(30, 0, "黑体");
	char SecondSelect0[20] = { "用户注册申请" };
	char SecondSelect1[20] = { "显示所有类型" };
	char SecondSelect2[20] = { "添加/修改类型" };
	char SecondSelect3[20] = { "返回" };
	outtextxy(300, 115, SecondSelect0);
	outtextxy(300, 245, SecondSelect1);
	outtextxy(300, 375, SecondSelect2);
	outtextxy(360, 505, SecondSelect3);

	//进入主界面选项操作界面
	while (true)
	{
		m2 = GetMouseMsg();//获取鼠标操作
		if (m2.x >= 230 && m2.x <= 560 && m2.y >= 110 && m2.y <= 155)
		{
			setlinecolor(RED);
			rectangle(230, 110, 560, 155);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//用户注册申请
				cleardevice();
				BackAdmin = 1;
				ShowApplication();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 240 && m2.y <= 285)
		{
			setlinecolor(RED);
			rectangle(230, 240, 560, 285);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//显示所有类型
				cleardevice();
				ShowAllType();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 370 && m2.y <= 415)
		{
			setlinecolor(RED);
			rectangle(230, 370, 560, 415);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				ManageType();
			}
		}
		else if (m2.x >= 230 && m2.x <= 560 && m2.y >= 500 && m2.y <= 545)
		{
			setlinecolor(RED);
			rectangle(230, 500, 560, 545);
			if (m2.uMsg == WM_LBUTTONDOWN)
			{
				//返回
				cleardevice();
				FirstPage();
			}
		}
		//鼠标不在任何按钮上悬停
		else
		{
			setlinecolor(WHITE);
			rectangle(230, 240, 560, 285);
			rectangle(230, 370, 560, 415);
			rectangle(230, 500, 560, 545);
		}
	}

}

//普通用户界面
void CommonUserPage()
{
	initgraph(760, 760);
	cleardevice();
	//定义第3个鼠标操作
	MOUSEMSG m3;
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 760, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	char SecondTitle1[5] = { "普" };
	char SecondTitle2[5] = { "通" };
	char SecondTitle3[5] = { "用" };
	char SecondTitle4[5] = { "户" };
	char SecondTitle5[5] = { "操" };
	char SecondTitle6[5] = { "作" };
	char SecondTitle7[5] = { "界" };
	char SecondTitle8[5] = { "面" };

	settextstyle(30, 0, "黑体");
	outtextxy(50, 150, SecondTitle1);
	outtextxy(50, 210, SecondTitle2);
	outtextxy(50, 270, SecondTitle3);
	outtextxy(50, 330, SecondTitle4);
	outtextxy(50, 390, SecondTitle5);
	outtextxy(50, 450, SecondTitle6);
	outtextxy(50, 510, SecondTitle7);
	outtextxy(50, 570, SecondTitle8);
	setbkmode(TRANSPARENT);

	//三个按钮的背景
	fillrectangle(230, 240, 560, 285);
	fillrectangle(230, 370, 560, 415);
	fillrectangle(230, 500, 560, 545);
	setbkmode(TRANSPARENT);
	//三个按钮的文字
	settextstyle(30, 0, "黑体");
	char ThirdSelect1[20] = { "显示所有物品" };
	char ThirdSelect2[20] = { "增删查找物品" };
	char ThirdSelect3[20] = { "返回" };
	outtextxy(300, 245, ThirdSelect1);
	outtextxy(300, 375, ThirdSelect2);
	outtextxy(370, 505, ThirdSelect3);

	//进入主界面选项操作界面
	while (1)
	{
		m3 = GetMouseMsg();//获取鼠标操作
		if (m3.x >= 230 && m3.x <= 560 && m3.y >= 240 && m3.y <= 285)
		{
			setlinecolor(RED);
			rectangle(230, 240, 560, 285);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				//显示所有物品
				cleardevice();
				ShowAllItem();
			}
		}
		else if (m3.x >= 230 && m3.x <= 560 && m3.y >= 370 && m3.y <= 415)
		{
			setlinecolor(RED);
			rectangle(230, 370, 560, 415);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				//增删查数据
				cleardevice();
				ManageItem();
			}
		}
		else if (m3.x >= 230 && m3.x <= 560 && m3.y >= 500 && m3.y <= 545)//选项3——返回
		{
			setlinecolor(RED);
			rectangle(230, 500, 560, 545);
			if (m3.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				FirstPage();
			}
		}
		//鼠标不在任何按钮上悬停
		else
		{
			setlinecolor(WHITE);
			rectangle(230, 240, 560, 285);
			rectangle(230, 370, 560, 415);
			rectangle(230, 500, 560, 545);
		}
	}
}

/*
	//////////////////////用户登录、注册//////////////////////
*/

//管理员登录
void AdminSignIn()
{
	//读取管理员用户名、密码
	FILE* fp = fopen("AdminSignIn.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	fscanf(fp, "%s\t%s\t\n", AdminUser.UserName, AdminUser.PassWord);
	fclose(fp);

	char ReceAcctNumber[64];
	TCHAR InputAcct[] = _T("请输入用户名");
	InputBox(ReceAcctNumber, 12, InputAcct);

	char ReceAcctPassWord[64];
	TCHAR InputPass[] = _T("请输入密码");
	InputBox(ReceAcctPassWord, 7, InputPass);

	//如果用户名和密码都正确才进入管理员界面，否则弹窗提示错误
	if (strcmp(AdminUser.UserName, ReceAcctNumber) == 0 && strcmp(AdminUser.PassWord, ReceAcctPassWord) == 0)
	{
		cleardevice();
		AdminPage();    //管理员界面
	}
	else
	{
		HWND SignError = GetHWnd();
		int isok = MessageBox(SignError, "用户名或密码错误!", "提示", MB_OK);
	}
}

//普通用户登录
void CommonUserSignIn()
{
	/*
	//读取用户名、密码
	FILE* fp = fopen("CommonUserSignIn.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t\n",
			User[i].UserName,
			User[i].PassWord);
	}
	int UserNum = 0;
	for (int k = 0; k < Num; k++)
	{
		if (strlen(User[k].UserName) != 0)
		{
			UserNum++;
		}
	}
	fclose(fp);
	*/
	char ReceAcctNumber[64];
	TCHAR InputAcct[] = _T("请输入用户名");
	InputBox(ReceAcctNumber, 12, InputAcct);

	char ReceAcctPassWord[64];
	TCHAR InputPass[] = _T("请输入密码");
	InputBox(ReceAcctPassWord, 7, InputPass);

	bool flag = false;    //记录是否找到一条正确的用户名、密码
	for (int i = 0; i < CommonUserNum; i++)
	{
		if (strcmp(User[i].UserName, ReceAcctNumber) == 0 && strcmp(User[i].PassWord, ReceAcctPassWord) == 0)
		{
			flag = true;
			CurrentUser = User[i];    //记录当前用户手机号
			cleardevice();
			CommonUserPage();    //普通用户界面
		}
	}
	if (!flag)
	{
		HWND SignError = GetHWnd();
		int isok = MessageBox(SignError, "用户名或密码错误!", "提示", MB_OK);
	}
}

//新用户注册
void Registration() {
	while (true)
	{
		TCHAR InputWindow0[] = _T("请输入您的手机号作为用户名");
		InputBox(Regis[RegisNum].UserName, 12, InputWindow0);
		//判断是否输入重复
		bool Flag = false;
		for (int i = 0; i < CommonUserNum; i++)
		{
			if (strcmp(Regis[RegisNum].UserName, User[i].UserName) == 0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "该手机号已注册，请重新输入", "提示", MB_OK);
		}
		else
		{
			break;
		}
	}

	//输入密码
	TCHAR InputWindow2[] = _T("请输入六位密码");
	InputBox(Regis[RegisNum].PassWord, 7, InputWindow2);
	//输入地址
	TCHAR InputWindow3[] = _T("请输入您的地址");
	InputBox(Regis[RegisNum].Addr, 16, InputWindow3);
	//输入邮箱
	TCHAR InputWindow4[] = _T("请输入您的邮箱");
	InputBox(Regis[RegisNum].Email, 21, InputWindow4);
	//保存至管理员未读信息ApplicationList.txt
	SaveApplication();

	HWND hwndInput2 = GetHWnd();
	int isok = MessageBox(hwndInput2, "注册申请已发送，请耐心等待管理员审核", "提示", MB_OK);
}


/*
	//////////////////////管理数据//////////////////////
*/

//读取文件
void ReadInfor()
{
	//读取物品列表
	FILE* fp = fopen("ItemList.txt", "r");
	if (fp == NULL)
	{
		return;
	}

	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t\n",
			ItemArry[i].i_Name,
			ItemArry[i].i_Type,
			ItemArry[i].i_Note,
			ItemArry[i].i_Tel,
			ItemArry[i].i_Addr,
			ItemArry[i].i_Email);
	}
	//记录物品总数
	for (int k = 0; k < Num; k++)
	{
		if (strlen(ItemArry[k].i_Name) != 0)
		{
			ItemNum++;
		}
	}
	fclose(fp);

	//读取物品类型列表
	fp = fopen("TypeList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\n",
			TypeArry[i].t_Name,
			TypeArry[i].t_Attrib);
	}
	//记录类型总数
	for (int k = 0; k < Num; k++)
	{
		if (strlen(TypeArry[k].t_Name) != 0)
		{
			TypeNum++;
		}
	}
	fclose(fp);

	//读取用户列表
	fp = fopen("CommonUserList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\n",
			User[i].UserName,
			User[i].PassWord,
			User[i].Addr,
			User[i].Email);
	}
	//记录普通用户总数
	for (int k = 0; k < Num; k++)
	{
		if (strlen(User[k].UserName) != 0)
		{
			CommonUserNum++;
		}
	}
	fclose(fp);

	//读取注册申请列表
	fp = fopen("ApplicationList.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < Num; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\n",
			Regis[i].UserName,
			Regis[i].PassWord,
			Regis[i].Addr,
			Regis[i].Email);
	}
	//记录普通用户总数
	for (int k = 0; k < Num; k++)
	{
		if (strlen(Regis[k].UserName) != 0)
		{
			RegisNum++;
		}
	}
	fclose(fp);
}

//保存新的一条注册申请至ApplicationList.txt
void SaveApplication() 
{
	if (strlen(Regis[RegisNum].UserName) != 0)
	{
		FILE* fp = fopen("ApplicationList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\n",
			Regis[RegisNum].UserName,
			Regis[RegisNum].PassWord,
			Regis[RegisNum].Addr,
			Regis[RegisNum].Email);
		fclose(fp);
		//保存完毕物品总数++
		RegisNum++;
	}
}

//更新Item.txt文件
void SaveItem()
{
	remove("ItemList.txt");
	//如果不为空才写入文件
	FILE* fp = fopen("ItemList.txt", "a");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < ItemNum; i++)
	{
		if (strlen(ItemArry[i].i_Name) != 0)
		{
			fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",
				ItemArry[i].i_Name,
				ItemArry[i].i_Type,
				ItemArry[i].i_Note,
				ItemArry[i].i_Tel,
				ItemArry[i].i_Addr,
				ItemArry[i].i_Email);
		}
	}
	fclose(fp);
}

//保存最新添加的一样物品到文件
void SaveLastItem()
{
	//不为空才写入文件
	if (strlen(ItemArry[ItemNum].i_Name) != 0)
	{
		FILE* fp = fopen("ItemList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",
			ItemArry[ItemNum].i_Name,
			ItemArry[ItemNum].i_Type,
			ItemArry[ItemNum].i_Note,
			ItemArry[ItemNum].i_Tel,
			ItemArry[ItemNum].i_Addr,
			ItemArry[ItemNum].i_Email);
		fclose(fp);
		//保存完毕物品总数++
		ItemNum++;
	}
}

//返回要删除物品的下标
int DeletePosition()
{
	//接收用户输入
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("请输入要删除的物品名称\n注意：您只能删除自己添加的物品");
	InputBox(ReceInput, 20, InputWindowFI);

	//进行循环查找，一旦相同就跳出循环
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Name, ReceInput) == 0 
			&& strcmp(ItemArry[w].i_Tel, CurrentUser.UserName) == 0)  
		{
			return w;
		}
	}
	//没找到就返回-1
	return -1;
}

//添加物品
void AddItem()
{
	//不能存在物品名和用户都相同的记录
	//通过循环来检查是否有重复的
	char Attrib[64];

	//输入物品名称
	while (true)
	{
		TCHAR InputWindow0[] = _T("请输入物品名称");
		InputBox(ItemArry[ItemNum].i_Name, 16, InputWindow0,0,0,0,0,false);
		//判断是否输入重复
		bool Flag = false;
		for (int i = 0; i < ItemNum; i++)
		{
			if (strcmp(ItemArry[ItemNum].i_Name, ItemArry[i].i_Name) == 0
				&& strcmp(ItemArry[ItemNum].i_Tel, ItemArry[i].i_Tel) == 0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "该物品已存在，请重新输入", "提示", MB_OK);
		}
		else
		{
			break;
		}
	}

	//输入类型
	TypePrint();
	while (true)
	{
		TCHAR InputWindow1[] = _T("请输入物品类型");
		InputBox(ItemArry[ItemNum].i_Type, 16, InputWindow1,NULL,NULL,0,0,false);

		//判断输入的类型是否存在
		bool Flag = false;
		for (int i = 0; i < TypeNum; i++)
		{
			if (strcmp(ItemArry[ItemNum].i_Type, TypeArry[i].t_Name) == 0)
			{
				Flag = true;
				strcpy(Attrib, TypeArry[i].t_Attrib);
				break;
			}
		}
		if (!Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "该类型不存在，请重新输入", "提示", MB_OK);
			ManageItem();
		}
		else
		{
			break;
		}
	}

	//输入类型说明
	TCHAR InputWindow2[] = _T("请输入物品说明，格式样例：“保质期：XXX”");
	InputBox(ItemArry[ItemNum].i_Note, 21, InputWindow2, NULL, NULL, 0, 0, false);

	strcpy(ItemArry[ItemNum].i_Tel, CurrentUser.UserName);
	strcpy(ItemArry[ItemNum].i_Addr, CurrentUser.Addr);
	strcpy(ItemArry[ItemNum].i_Email, CurrentUser.Email);

	//保存数据
	SaveLastItem();
}

//删除某个物品的信息
void DeleteItem()
{
	if (ShowUserItem() == -1)
	{
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "您尚未添加物品，没有可以删除的物品!", "提示", MB_OK);
		return;
	}
	int TempDelt = DeletePosition();
	if (TempDelt == -1)    //没找到
	{
		//获取窗口句柄
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "不存在该物品!", "提示", MB_OK);
	}
	else    //找到了，确认是否删除
	{
		HWND hndtipsDS = GetHWnd();
		int isok = MessageBox(hndtipsDS, "确定删除？", "提示", MB_OKCANCEL);
		if (isok == IDOK)
		{
			for (int d = TempDelt; d < ItemNum - 1; d++)
			{
				ItemArry[d] = ItemArry[d + 1];
			}
			ItemNum--;
			//更新到文件中
			SaveItem();
		}
		else if (isok == IDCANCEL)
		{
			//取消删除不执行任何操作
		}
	}
}

//查找物品
void SearchItem()
{
	//接收用户输入
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("请输入要查找的物品名称或类型");
	InputBox(ReceInput, 20, InputWindowFI, NULL, NULL, 0, 0, false);

	int tot=0;    //标记找到的符合条件的物品数
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Name, ReceInput) == 0
			|| strcmp(ItemArry[w].i_Type, ReceInput) == 0)
		{
			ItemFound[tot] = ItemArry[w];
			tot++;
		}
	}
	
	if (tot==0)    //没找到
	{
		HWND hndtipsF = GetHWnd();
		int isok = MessageBox(hndtipsF, "不存在这样的物品!", "提示", MB_OK);
	}
	else
	{
		cleardevice();
		ItemPrint(tot);
	}
}

//显示当前用户添加的物品
int ShowUserItem()
{
	int tot = 0;    
	for (int w = 0; w < ItemNum; w++)
	{
		if (strcmp(ItemArry[w].i_Tel, CurrentUser.UserName) == 0)
		{
			ItemFound[tot] = ItemArry[w];
			tot++;
		}
	}

	if (tot == 0)    //当前用户没有添加过物品
	{
		return -1;
	}
	else
	{
		for (int q = 1; q <= tot; q++)
		{
			//顺序
			settextstyle(30, 0, "黑体");
			char Nums[5];
			sprintf(Nums, "%d", q);

			//缩放字体
			settextstyle(15, 0, "黑体");
			//名称
			outtextxy(80, 40 + 40 * q, ItemFound[q - 1].i_Name);
			//类型
			outtextxy(200, 40 + 40 * q, ItemFound[q - 1].i_Type);
			//说明
			outtextxy(300, 40 + 40 * q, ItemFound[q - 1].i_Note);
			//联系人手机
			outtextxy(400, 40 + 40 * q, ItemFound[q - 1].i_Tel);
			//联系人地址
			outtextxy(500, 40 + 40 * q, ItemFound[q - 1].i_Addr);
			//联系人邮箱
			outtextxy(600, 40 + 40 * q, ItemFound[q - 1].i_Email);
		}
	}
}

//更新Type.txt文件
void SaveType()
{
	remove("TypeList.txt");
	//如果不为空才写入文件
	FILE* fp = fopen("TypeList.txt", "a");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < TypeNum; i++)
	{
		if (strlen(TypeArry[i].t_Name) != 0)
		{
			fprintf(fp, "%s\t%s\n",
				TypeArry[i].t_Name,
				TypeArry[i].t_Attrib);
		}
	}
	fclose(fp);
}

//保存最新添加的一种类型到Type.txt
void SaveLastType()
{
	//不为空才写入文件
	if (strlen(TypeArry[TypeNum].t_Name) != 0)
	{
		FILE* fp = fopen("TypeList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\n",
			TypeArry[TypeNum].t_Name,
			TypeArry[TypeNum].t_Attrib);
		fclose(fp);
		//保存完毕类型总数++
		TypeNum++;
	}
}

//保存通过审核的新用户
void SaveNewCommonUser(int x)
{
	if (strlen(Regis[x].UserName) != 0)
	{
		FILE* fp = fopen("CommonUserList.txt", "a");
		if (fp == NULL)
		{
			return;
		}
		fprintf(fp, "%s\t%s\t%s\t%s\n",
			Regis[x].UserName,
			Regis[x].PassWord,
			Regis[x].Addr,
			Regis[x].Email);
		fclose(fp);
		//保存完毕类型总数++
		CommonUserNum++;
	}
	
}

//返回要删除类型的下标
int DeleteTypePosition()
{
	//接收用户输入
	char ReceInput[64];
	TCHAR InputWindowFI[] = _T("请输入要删除的类型名称");
	InputBox(ReceInput, 20, InputWindowFI);

	//进行循环查找，一旦相同就跳出循环
	for (int w = 0; w < TypeNum; w++)
	{
		if (strcmp(TypeArry[w].t_Name, ReceInput) == 0)
		{
			return w;
		}
	}
	//没找到就返回-1
	return -1;
}

//添加类型
void AddType()
{	
	while (true)
	{
		TCHAR InputWindow0[] = _T("请输入类型名称");
		InputBox(TypeArry[TypeNum].t_Name, 16, InputWindow0);
		//判断是否输入重复
		bool Flag = false;
		for (int i = 0; i < TypeNum; i++)
		{
			if (strcmp(TypeArry[TypeNum].t_Name, TypeArry[i].t_Name)==0)
			{
				Flag = true;
				break;
			}
		}
		if (Flag)
		{
			HWND hwndInput1 = GetHWnd();
			int isok = MessageBox(hwndInput1, "该类型已存在，请重新输入", "提示", MB_OK);
		}
		else
		{
			break;
		}
	}

	//输入类型说明
	TCHAR InputWindow2[] = _T("请输入该类型的一个属性，如“保质期”");
	InputBox(TypeArry[TypeNum].t_Attrib, 16, InputWindow2, NULL, NULL, 0, 0, false);

	//保存数据
	SaveLastType();
}

//删除类型
void DeleteType()
{
	int TempDelt = DeleteTypePosition();
	if (TempDelt == -1)    //没找到
	{
		//获取窗口句柄
		HWND hndtipsD = GetHWnd();
		int isok = MessageBox(hndtipsD, "该类型不存在!", "提示", MB_OK);
	}
	else    //找到了，确认是否删除
	{
		HWND hndtipsDS = GetHWnd();
		int isok = MessageBox(hndtipsDS, "确定删除？", "提示", MB_OKCANCEL);
		if (isok == IDOK)
		{
			for (int d = TempDelt; d < TypeNum - 1; d++)
			{
				TypeArry[d] = TypeArry[d + 1];
			}
			TypeNum--;
			//更新到文件中
			SaveType();
		}
		else if (isok == IDCANCEL)
		{
			//取消删除不执行任何操作
		}
	}
}

//逐条处理注册申请
void ApplicationPrint()
{
	MOUSEMSG SA;
	settextcolor(BLACK);
	settextstyle(40, 0, "黑体");
	//如果为空就不往下执行
	if (RegisNum == 0)
	{
		outtextxy(200, 200, "暂无新用户注册申请!");
		return;
	}
	for (int q = 1; q <= RegisNum; q++)
	{
		//顺序
		char Nums[5];
		sprintf(Nums, "%d", q);

		//缩放字体
		settextstyle(25, 0, "黑体");
		outtextxy(80, 40 + 40 * q, Regis[q - 1].UserName);
		outtextxy(300, 40 + 40 * q, Regis[q - 1].Addr);
		outtextxy(600, 40 + 40 * q, Regis[q - 1].Email);
	
		while (true)
		{
			SA = GetMouseMsg();
			if (SA.x >= 690 && SA.x <= 850 && SA.y >= 460 && SA.y <= 510)
			{
				//批准，向CommonUserList.txt中添加新记录
				setlinecolor(RED);
				rectangle(690, 460, 850, 510);
				if (SA.uMsg == WM_LBUTTONDOWN)
				{
					SaveNewCommonUser(q - 1);
					break;
				}
			}
			else if (SA.x >= 690 && SA.x <= 850 && SA.y >= 525 && SA.y <= 575)
			{
				//不批准，跳过
				setlinecolor(RED);
				rectangle(690, 525, 850, 575);
				if (SA.uMsg == WM_LBUTTONDOWN)
				{
					break;
				}
			}
			else
			{
				setlinecolor(WHITE);
				rectangle(690, 460, 850, 510);
				rectangle(690, 525, 850, 575);
				rectangle(690, 590, 850, 640);
			}
		}
	}
	outtextxy(80, 80 + 40 * RegisNum, "已无更多注册申请，请点击返回");
	RegisNum = 0;
	remove("ApplicationList.txt");
}

//处理新用户注册申请
void ShowApplication()
{
	MOUSEMSG SA;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 460, 850, 510);
	fillrectangle(690, 525, 850, 575);
	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(750, 475, "批准");
	outtextxy(750, 540, "不批");
	outtextxy(750, 605, "返回");

	ApplicationPrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				AdminPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 460, 850, 510);
			rectangle(690, 525, 850, 575);
			rectangle(690, 590, 850, 640);
		}
	}
}

//管理员：增、删类型
void ManageType()
{
	MOUSEMSG SL;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193, 193, 193));
	fillrectangle(690, 90, 850, 140);    //增
	fillrectangle(690, 190, 850, 240);    //删除
	fillrectangle(690, 590, 850, 640);    //返回

	//表头文字部分
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 0, "黑体");
	outtextxy(740, 0, "选项");
	outtextxy(740, 100, "添加");
	outtextxy(740, 200, "删除");
	outtextxy(740, 600, "返回");

	TypePrint();

	//进入鼠标操作环节
	while (true)
	{
		SL = GetMouseMsg();
		if (SL.x >= 690 && SL.x <= 850 && SL.y >= 90 && SL.y <= 140)
		{
			setlinecolor(RED);
			rectangle(690, 90, 850, 140);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//添加物品
				AddType();
				cleardevice();
				ManageType();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 190 && SL.y <= 240)
		{
			setlinecolor(RED);
			rectangle(690, 190, 850, 240);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//删除物品
				DeleteType();
				ManageType();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 590 && SL.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				//完成操作重新载入——刷新
				AdminPage();
			}
		}
		//鼠标不悬停
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 90, 850, 140);    //增
			rectangle(690, 190, 850, 240);    //删
			rectangle(690, 590, 850, 640);    //返回
		}
	}
}

//普通用户：增、删、查物品
void ManageItem()
{
	MOUSEMSG SL;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);

	setfillcolor(RGB(193,193,193));
	fillrectangle(690, 90, 850, 140);    //增
	fillrectangle(690, 190, 850, 240);    //删
	fillrectangle(690, 290, 850, 340);    //查
	fillrectangle(690, 590, 850, 640);    //返回

	//表头文字部分
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, "黑体");
	outtextxy(740, 0, "选项");
	outtextxy(740, 100, "添加");
	outtextxy(740, 200, "删除");
	outtextxy(740, 300, "查找");
	outtextxy(740, 600, "返回");

	//进入鼠标操作环节
	while (true)
	{
		SL = GetMouseMsg();
		if (SL.x >= 690 && SL.x <= 850 && SL.y >= 90 && SL.y <= 140)
		{
			setlinecolor(RED);
			rectangle(690, 90, 850, 140);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//添加物品
				AddItem();
				cleardevice();
				//完成操作重新载入——刷新
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 190 && SL.y <= 240)
		{
			setlinecolor(RED);
			rectangle(690, 190, 850, 240);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				//删除物品
				DeleteItem();
				//完成操作重新载入——刷新
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 290 && SL.y <= 340)
		{
			setlinecolor(RED);
			rectangle(690, 290, 850, 340);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				FlagFPT = 1;
				//查找物品
				SearchItem();
				//完成操作重新载入——刷新
				ManageItem();
			}
		}
		else if (SL.x >= 690 && SL.x <= 850 && SL.y >= 590 && SL.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SL.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				//完成操作重新载入——刷新
				CommonUserPage();
			}
		}
		//鼠标不悬停
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 90, 850, 140);    //增
			rectangle(690, 190, 850, 240);    //删
			rectangle(690, 290, 850, 340);    //查
			rectangle(690, 590, 850, 640);    //返回
		}
	}
}

//在屏幕上打印找到的物品信息
void ItemPrint(int tot)
{
	MOUSEMSG SA;
	initgraph(1000, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 1000, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(750, 600, "返回");

	for (int q = 1; q <= tot; q++)
	{
		//顺序
		settextstyle(30, 0, "黑体");
		char Nums[5];
		sprintf(Nums, "%d", q);

		//缩放字体
		settextstyle(15, 0, "黑体");
		//名称
		outtextxy(80, 40 + 40 * q, ItemFound[q - 1].i_Name);
		//类型
		outtextxy(200, 40 + 40 * q, ItemFound[q - 1].i_Type);
		//说明
		outtextxy(300, 40 + 40 * q, ItemFound[q - 1].i_Note);
		//联系人手机
		outtextxy(400, 40 + 40 * q, ItemFound[q - 1].i_Tel);
		//联系人地址
		outtextxy(500, 40 + 40 * q, ItemFound[q - 1].i_Addr);
		//联系人邮箱
		outtextxy(600, 40 + 40 * q, ItemFound[q - 1].i_Email);
	}

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				ManageItem();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}

//在屏幕上打印类型信息
void TypePrint()
{
	settextcolor(BLACK);
	settextstyle(30, 0, "黑体");
	//如果为空就不往下执行
	if (TypeNum == 0)
	{
		outtextxy(200, 200, "当前记录为空!");
		return;
	}
	for (int q = 1; q <= TypeNum; q++)
	{
		//顺序
		char Nums[5];
		sprintf(Nums, "%d", q);

		//缩放字体
		settextstyle(25, 0, "黑体");
		//名称
		outtextxy(80, 40 + 40 * q, TypeArry[q - 1].t_Name);
		//属性
		outtextxy(200, 40 + 40 * q, TypeArry[q - 1].t_Attrib);	
	}
}

//显示所有类型
void ShowAllType()
{
	MOUSEMSG SA;
	initgraph(860, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png", 860, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193,193,193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(750, 610, "返回");

	TypePrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				AdminPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}

//在屏幕上打印物品信息
void AllItemPrint()
{
	settextcolor(BLACK);
	settextstyle(30, 0, "黑体");
	//如果为空就不往下执行
	if (ItemNum == 0)
	{
		outtextxy(200, 200, "当前记录为空!");
		return;
	}
	settextcolor(BLACK);
	for (int q = 1; q <= ItemNum; q++)
	{
		//顺序
		char Nums[5];
		sprintf(Nums, "%d", q);

		settextstyle(15, 0, "黑体");
		outtextxy(80, 40 + 40 * q, ItemArry[q - 1].i_Name);
		outtextxy(200, 40 + 40 * q, ItemArry[q - 1].i_Type);
		outtextxy(300, 40 + 40 * q, ItemArry[q - 1].i_Note);
		outtextxy(400, 40 + 40 * q, ItemArry[q - 1].i_Tel);
		outtextxy(500, 40 + 40 * q, ItemArry[q - 1].i_Addr);
		outtextxy(600, 40 + 40 * q, ItemArry[q - 1].i_Email);
	}
}

//显示所有物品
void ShowAllItem()
{
	MOUSEMSG SA;
	initgraph(1000, 760);
	cleardevice();
	IMAGE img;
	loadimage(&img, "./background.png",1000, 760);
	putimage(0, 0, &img);
	setfillcolor(RGB(193, 193, 193));
	setbkmode(TRANSPARENT);

	fillrectangle(690, 590, 850, 640);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, "黑体");
	outtextxy(750, 600, "返回");

	AllItemPrint();

	while (true)
	{
		SA = GetMouseMsg();
		if (SA.x >= 690 && SA.x <= 850 && SA.y >= 590 && SA.y <= 640)
		{
			//返回
			setlinecolor(RED);
			rectangle(690, 590, 850, 640);
			if (SA.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				CommonUserPage();
			}
		}
		else
		{
			setlinecolor(WHITE);
			rectangle(690, 590, 850, 640);
		}
	}
}


int main()
{
	FirstPage();

	return 0;
}